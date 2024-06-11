#include "app/comm.h"
#include "bsp/usart.h"

#include "line_protocol.h"
#include "line_api.h"
#include "flash_line_api.h"
#include "flash_line_diag.h"
//#include "bl/api.h"
//#include "hal/dsu.h"
#include "bsp/light_control.h"

#include "app/tmon.h"
#include "app/config.h"

#include "common/swtimer.h"
#include "common/ringbuffer.h"

RINGBUFFER_8(COMM_UsartBufferTx, 128);
RINGBUFFER_8(COMM_UsartBufferRx, 128);

// TODO: data should be dynamic based on actual current figures 
static LINE_Diag_PowerStatus_t power_status = {
    .U_status = LINE_DIAG_POWER_STATUS_VOLTAGE_OK,
    .BOD_status = LINE_DIAG_POWER_STATUS_BOD_NONE,
    .I_operating = LINE_DIAG_POWER_STATUS_OP_CURRENT(100),
    .I_sleep = LINE_DIAG_POWER_STATUS_SLEEP_CURRENT(100)
};

// TODO: data should be dynamic based CMake config (copy from samd21-line-bootloader)
static LINE_Diag_SoftwareVersion_t sw_version = {
    .major = 0,
    .minor = 1,
    .patch = 0
};

static swtimer_t* comm_lightrequest_timer;

// TODO: support op. mode (error in case all drivers fail, warning if IMU or single segment fails)
uint8_t LINE_Diag_GetOperationStatus(void) {
    return LINE_DIAG_OP_STATUS_OK;
}

LINE_Diag_PowerStatus_t* LINE_Diag_GetPowerStatus(void) {
    return &power_status;
}

uint32_t LINE_Diag_GetSerialNumber(void) {
    //return DSU_GetSerialNumber32();
    return 0xABCDEF01;
}

LINE_Diag_SoftwareVersion_t* LINE_Diag_GetSoftwareVersion(void) {
    return &sw_version;
}

void COMM_Initialize(void) {
    USART_Initialize(LINE_NETWORK_BicycleNetwork1_BAUDRATE, &COMM_UsartBufferTx, &COMM_UsartBufferRx);
    USART_Enable();

    LINE_Transport_Init(true);
    LINE_App_Init();
    LINE_Diag_SetAddress(LINE_NODE_FrontLight_DIAG_ADDRESS);
    FLASH_LINE_Init(FLASH_LINE_APPLICATION_MODE);

    comm_lightrequest_timer = SWTIMER_Create();
}

void COMM_UpdatePhy(void) {
    uint8_t length = USART_Available();
    while (length > 0) {
        uint8_t data = USART_Read();
        LINE_Transport_Receive(data);
        length--;
    }

    LINE_Transport_Update(1);

    if (LINE_Request_LightSynchronization_flag() || LINE_Request_FrontLightSetting_flag()) {
        SWTIMER_Setup(comm_lightrequest_timer, FEATURE_COMM_LIGHTREQUEST_TIMEOUT);
    }
}

void LINE_Transport_WriteResponse(uint8_t size, uint8_t* payload, uint8_t checksum) {
    const uint8_t fix = 69;
    USART_WriteData(&size, sizeof(uint8_t));
    // TODO: fix for skipped 3rd byte
    USART_WriteData(payload, 1);
    USART_WriteData(&fix, 1);
    USART_WriteData(payload+1, size-1);
    USART_WriteData(&checksum, sizeof(uint8_t));
    USART_FlushOutput();
}

static bool comm_bootrequest = false;

uint8_t FLASH_BL_EnterBoot(void) {

    // TODO: when do we reject boot entry requests?
    comm_bootrequest = true;

    return FLASH_LINE_BOOT_ENTRY_SUCCESS;
}

bool COMM_BootRequest(void) {
    return comm_bootrequest;
}

uint16_t COMM_GetTargetBrightness(void) {
    return LINE_Request_LightSynchronization_data.fields.TargetBrightness * 10U;
}

bool COMM_LightRequestTimeout(void) {
    return SWTIMER_Elapsed(comm_lightrequest_timer);
}

uint8_t COMM_LightMode(void) {
    return LINE_Request_LightSynchronization_data.fields.LightMode;
}

uint8_t COMM_LightBehavior(void) {
    return LINE_Request_FrontLightSetting_data.fields.Behavior;
}

static uint8_t COMM_EncodeLightStatus(lightcontrol_feature_state_t state) {
    if (state == lightcontrol_feature_state_off) {
        return LINE_ENCODER_LightStatusEncoder_Off;
    }
    else if (state == lightcontrol_feature_state_ok) {
        return LINE_ENCODER_LightStatusEncoder_Ok;
    }
    else if(state == lightcontrol_feature_state_partial_error) {
        return LINE_ENCODER_LightStatusEncoder_PartialError;
    }
    else if(state == lightcontrol_feature_state_error) {
        return LINE_ENCODER_LightStatusEncoder_Error;
    }
    else {
        return LINE_ENCODER_LightStatusEncoder_Error;
    }
}

void COMM_UpdateSignals(void) {
    // TODO: check errors in LightController, report off if disabled
    //LINE_Request_RearLightStatus_data.fields.BrakeLightStatus = COMM_EncodeLightStatus(LIGHTCONTROL_GetDiagnosticState(lightcontrol_feature_brake_segment));
    //LINE_Request_RearLightStatus_data.fields.TailLightStatus = COMM_EncodeLightStatus(LIGHTCONTROL_GetDiagnosticState(lightcontrol_feature_tail_segment));

    //LINE_Request_RearLightStatus_data.fields.SignalLightStatus = LINE_ENCODER_LightStatusEncoder_Off;   // Not present on Gen1.0

    // TODO: measure MCU temp. and return accordingly
    //LINE_Request_RearLightStatus_data.fields.ThermalStatus = LINE_ENCODER_ThermalStatusEncoder_NotMeasured;
    LINE_Request_FrontLightStatus_data.fields.ThermalStatus = LINE_ENCODER_ThermalStatusEncoder_NotMeasured;
}

void COMM_UpdateDebugSignals(void) {
    
    LINE_Request_FrontLightDebug_data.fields.DriveTemperature = LINE_ENCODER_TemperatureEncoder_Encode(TMON_DriveTemperature);
}
