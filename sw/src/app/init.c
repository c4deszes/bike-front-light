#include "bsp/board.h"
//#include "hal/wdt.h"
#include "hal/nvic.h"
#include "hal/tcc.h"

#include "common/scheduler.h"

#include "bsp/light_control.h"

#include "app/comm.h"
#include "app/strobe.h"
#include "app/brightness.h"
#include "app/sys_state.h"

#include <stddef.h>

// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************
// #pragma config NVMCTRL_BOOTPROT = SIZE_0BYTES
// #pragma config NVMCTRL_EEPROM_SIZE = SIZE_0BYTES
// #pragma config BOD33USERLEVEL = 0x7 // Enter Hexadecimal value
// #pragma config BOD33_EN = ENABLED
// #pragma config BOD33_ACTION = RESET

// #pragma config BOD33_HYST = DISABLED
// #pragma config NVMCTRL_REGION_LOCKS = 0xffff // Enter Hexadecimal value

void APP_Initialize() {
    // Low level init
    BSP_ClockInitialize();
    //WDT_InitializeNormal(&wdt_config);
    //EIC_Initialize(NULL);
    LIGHTCONTROL_Init();

    // Initializing application services
    SYSSTATE_Init();
    BRIGHTNESS_Init();
    STROBE_Init();

    // Initializing communication
    COMM_Initialize();

    // Setting up scheduler
    SCH_Init();
    TCC_Reset(TCC0);
    TCC_SetupTrigger(TCC0, 1000);   // 1000us period
    TCC_Enable(TCC0);

    NVIC_Initialize();
}

void TCC0_Interrupt(void) {
    SCH_Trigger();
}
