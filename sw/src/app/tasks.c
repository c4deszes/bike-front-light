#include "common/scheduler.h"
#include "common/swtimer.h"

#include <stdint.h>

#include "app/comm.h"
#include "app/brightness.h"
#include "app/strobe.h"
#include "app/sys_state.h"
#include "app/tmon.h"
#include "app/button.h"

#include "bsp/light_control.h"

#include "line_api.h"

void SCH_Task1ms(void) {
    SWTIMER_Update1ms();

    STROBE_Update1ms();

    COMM_UpdatePhy();
}

uint16_t counter = 0;
uint16_t brightness = 100;
bool state = false;

void SCH_Task10ms_A(void) {

    // if (counter > 10) {
    //     // brightness += 1;
    //     //if (brightness > 1000) {
    //     //    brightness = 0;
    //     //}
    //     counter = 0;
    //     BRIGHTNESS_SetMode(brightness_mode_adaptive);
    //     BRIGHTNESS_SetTarget(brightness);
    // }
    // counter++;

    SYSSTATE_Update10ms();

    BRIGHTNESS_Update10ms();

    BUTTON_Update10ms();

    // TODO: enable watchdog
    //WDT_Acknowledge();

    COMM_UpdateSignals();
    COMM_UpdateDebugSignals();

    LIGHTCONTROL_Update10ms();

    TMON_Update100ms();
}
