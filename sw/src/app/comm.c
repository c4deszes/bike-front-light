#include "app/comm.h"

// Hardware abstraction layer
#include "common/swtimer.h"

// Board support package
#include "bsp/light_control.h"

// Communication protocol
#include "line_protocol.h"
#include "line_api.h"
#include "flash_line_api.h"
#include "flash_line_diag.h"
#include "uds_gen.h"

// Application components
#include "app/feature.h"
#include "app/temp.h"
#include "app/current.h"
#include "app/brightness.h"
#include "app/strobe.h"
#include "app/button.h"

static swtimer_t* COMM_LightRequestTimer;
static swtimer_t* COMM_SpeedStatusTimer;

static bool COMM_FrontLightSettingReceivedOnce;

void COMM_Init(void) {
    LINE_App_Init();
    UDS_Init();
    FLASH_LINE_Init(LD_FrontLight_CHANNEL, FLASH_LINE_APPLICATION_MODE);

    COMM_LightRequestTimer = SWTIMER_Create();
    COMM_SpeedStatusTimer = SWTIMER_Create();

    COMM_FrontLightSettingReceivedOnce = false;

#if FEATURE_COMM_ENABLE_DEBUG_SIGNALS == 0
    l_FrontLightTemperatureDebug.enabled = false;
#endif
}

void COMM_Update10ms(void) {
    if (l_flg_tst_LightSynchronization()) {
        l_flg_clr_LightSynchronization();

        if (!COMM_FrontLightSettingReceivedOnce) {
            COMM_FrontLightSettingReceivedOnce = true;
        }

        SWTIMER_Setup(COMM_LightRequestTimer, FEATURE_COMM_LIGHTREQUEST_TIMEOUT);
    }

    // if (l_flg_tst_SpeedStatus()) {
    //     l_flg_clr_SpeedStatus();
    //     SWTIMER_Setup(COMM_SpeedStatusTimer, FEATURE_COMM_SPEEDSTATUS_TIMEOUT);
    // }
}

uint16_t COMM_GetTargetBrightness(void) {
    uint16_t target = l_rd_LightSynchronization_TargetBrightness() * 10U;

    /* Limit the target brightness */
    if (target >= LIGHTCONTROL_BRIGHTNESS_MAX) {
        target = LIGHTCONTROL_BRIGHTNESS_MAX;
    }

    return target;
}

brightness_mode_t COMM_LightMode(void) {
    uint8_t light_mode = l_rd_LightSynchronization_LightMode();
    if (light_mode == L_LightModeEncoder_Adaptive) {
        return brightness_mode_adaptive;
    }
    else if (light_mode == L_LightModeEncoder_DLR) {
        return brightness_mode_standard;
    }
    else if (light_mode == L_LightModeEncoder_Emergency) {
        return brightness_mode_emergency;
    }
    else if (light_mode == L_LightModeEncoder_Off) {
        return brightness_mode_off;
    }
    return brightness_mode_safety;
}

strobe_source_t COMM_LightBehavior(strobe_source_t default_source, strobe_source_t primary_source) {
    uint8_t behavior = l_rd_LightSynchronization_FrontBehavior();
    if (behavior == L_LightBehaviorEncoder_Solid) {
        return strobe_source_disabled;
    }
    else if (behavior == L_LightBehaviorEncoder_Blink) {
        return primary_source;
    }
    return strobe_source_disabled;
}

bool COMM_LightRequestTimeout(void) {
    return SWTIMER_Elapsed(COMM_LightRequestTimer);
}

// bool COMM_SpeedStatusTimeout(void) {
//     return SWTIMER_Elapsed(COMM_SpeedStatusTimer);
// }

// bool COMM_SpeedValid(void) {
//     uint8_t speed_state = l_rd_RideStatus_SpeedState();
//     return (speed_state == L_SpeedStateEncoder_Ok || speed_state == L_SpeedStateEncoder_SlowResponse);
// }

// uint16_t COMM_GetSpeed(void) {
//     return l_rd_RideStatus_Speed();
// }

static uint8_t COMM_EncodeLightState(lightcontrol_feature_state_t state) {
    if (state == lightcontrol_feature_state_ok) {
        return L_LightStateEncoder_Ok;
    }
    else if(state == lightcontrol_feature_state_partial_error) {
        return L_LightStateEncoder_PartialError;
    }
    else if(state == lightcontrol_feature_state_error) {
        return L_LightStateEncoder_Error;
    }
    return L_LightStateEncoder_Error;
}

static uint8_t COMM_EncodeThermalState(temp_status_t status) {
    if (status == temp_status_not_measured) {
        return L_ThermalStateEncoder_NotMeasured;
    }
    else if (CURRENT_ThermalShutdownActive()) {
        return L_ThermalStateEncoder_Shutdown;
    }
    else if (CURRENT_ThermalDeratingActive()) {
        return L_ThermalStateEncoder_Derating;
    }
    return L_ThermalStateEncoder_Ok;
}

void COMM_UpdateSignals(void) {
    /* Tail light state equals the diagnostic state if there were errors */
    lightcontrol_feature_state_t main_state = LIGHTCONTROL_GetDiagnosticState(lightcontrol_segment_main);
    uint8_t main_status = COMM_EncodeLightState(main_state);
    l_wr_FrontLightStatus_MainLightState(main_status);
    l_wr_FrontLightStatus_HighBeamState(L_LightStateEncoder_Off);
    l_wr_FrontLightStatus_TurnSignalState(L_LightStateEncoder_Off);

    temp_status_t thermal_status = TEMP_GetStatus();
    uint8_t encoded_thermal_status = COMM_EncodeThermalState(thermal_status);
    l_wr_FrontLightStatus_ThermalState(encoded_thermal_status);

    uint8_t drive_temp = L_TemperatureEncoder_Encode(TEMP_GetDriveTemperature());
    uint8_t mcu_temp = L_TemperatureEncoder_Encode(TEMP_GetMcuTemperature());
    l_wr_FrontLightStatus_McuTemperature(mcu_temp);
    l_wr_FrontLightStatus_DriveTemperature(drive_temp);

    uint8_t control_cycle_count = BUTTON_CycleCounter;
    l_wr_FrontLightStatus_ModeSwitchCycles(control_cycle_count);
}


void COMM_UpdateDebugSignals(void) {
    // No debug signals to update in this implementation
}
