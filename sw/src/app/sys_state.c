#include "app/sys_state.h"
#include "common/swtimer.h"
#include "app/config.h"

#include "app/brightness.h"
#include "app/strobe.h"
#include "app/comm.h"

#include "line_api.h"

//#include "bl/api.h"
#include "sam.h"

#include "app/button.h"

typedef enum {
    sys_state_init,         /* When starting up */
    sys_state_normal,       /* When target signal is valid */
    sys_state_safety,       /* When communication issues occur */
    sys_state_goto_boot,    /* When boot entry is requested */
    sys_state_goto_reset,   /* When reset is requested */
    sys_state_goto_sleep    /* When sleep is requested */
} sys_state_t;

static sys_state_t sys_state;
static swtimer_t* sys_transition_timer; 

void SYSSTATE_Init(void) {
    sys_state = sys_state_init;
    sys_transition_timer = SWTIMER_Create();
    SWTIMER_Setup(sys_transition_timer, FEATURE_SYSTEM_TIME_INIT);
}

uint64_t boot_entry_key __attribute__((section(".bl_shared_ram")));
static void SYSSTATE_BootEntry(void) {
    //boot_entry_key = BOOT_ENTRY_MAGIC;

    //NVIC_SystemReset();
}

static void SYSSTATE_Reset(void) {
    // NVIC_SystemReset();
}

void SYSSTATE_Update10ms(void) {
    if (sys_state == sys_state_init && SWTIMER_Elapsed(sys_transition_timer)) {
        // transition into safety or normal

        // if (COMM_LightRequestTimeout()) {
        //     sys_state = sys_state_safety;
        // }
        // else if (COMM_BootRequest()) {
        //     sys_state = sys_state_goto_boot;
        // }
        // else {
            sys_state = sys_state_normal;
        //}
    }
    else if (sys_state == sys_state_normal) {
        if (BUTTON_CycleCounter == 0) {
            BRIGHTNESS_SetMode(brightness_mode_off);
            BRIGHTNESS_SetTarget(0);
            STROBE_SetSource(strobe_source_disabled);
        }
        else if (BUTTON_CycleCounter == 1) {
            BRIGHTNESS_SetMode(brightness_mode_max);
            BRIGHTNESS_SetTarget(1000);
            STROBE_SetSource(strobe_source_disabled);
        }
        else if (BUTTON_CycleCounter == 2) {
            BRIGHTNESS_SetMode(brightness_mode_adaptive);
            BRIGHTNESS_SetTarget(800);
            STROBE_SetSource(strobe_source_disabled);
        }
        else if (BUTTON_CycleCounter == 3) {
            BRIGHTNESS_SetMode(brightness_mode_adaptive);
            BRIGHTNESS_SetTarget(500);
            STROBE_SetSource(strobe_source_disabled);
        }
        else if (BUTTON_CycleCounter == 4) {
            BRIGHTNESS_SetMode(brightness_mode_adaptive);
            BRIGHTNESS_SetTarget(200);
            STROBE_SetSource(strobe_source_disabled);
        }
        else if (BUTTON_CycleCounter == 5) {
            BRIGHTNESS_SetMode(brightness_mode_adaptive);
            BRIGHTNESS_SetTarget(1000);
            STROBE_SetSource(strobe_source_internal_single);
        }
        else {
            BRIGHTNESS_SetMode(brightness_mode_max);
            BRIGHTNESS_SetTarget(1000);
        }
    }
    // else if (sys_state == sys_state_safety) {
    //     STROBE_SetSource(STROBE_ConvertSource(CONFIG_SAFETY_STROBE_SOURCE));
    //     BRIGHTNESS_SetMode(brightness_mode_safety);

    //     if (!COMM_LightRequestTimeout()) {
    //         sys_state = sys_state_normal;
    //     }
    //     else if (COMM_BootRequest()) {
    //         sys_state = sys_state_goto_boot;
    //     }
    // }
    // // TODO: go to boot state, self reset
    // else if (sys_state == sys_state_goto_boot) {
    //     SYSSTATE_BootEntry();
    // }
}

