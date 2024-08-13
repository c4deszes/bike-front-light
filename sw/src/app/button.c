#include "sam.h"

#include <stdint.h>
#include <stdbool.h>

#include "bsp/pinout.h"
#include "app/config.h"

#include "hal/gpio.h"

static bool BUTTON_LastState;
static bool BUTTON_State;
static uint8_t BUTTON_StateCounter;

uint8_t BUTTON_CycleCounter;

#define BUTTON_MINIMUM_SAMPLES 2

static const gpio_pin_input_configuration input = {
    .pull = PULLUP,
    .sample = CONTINUOUS
};

void BUTTON_Init() {
    GPIO_SetupPinInput(BOARD_BUTTON_PORT, BOARD_BUTTON_PIN, &input);

    BUTTON_State = false;
    BUTTON_LastState = false;
    BUTTON_StateCounter = 0;
    BUTTON_CycleCounter = 0;
}

void BUTTON_OnClick() {
    BUTTON_CycleCounter += 1;

    if (BUTTON_CycleCounter >= FEATURE_CONTROL_BUTTON_CYCLES) {
        BUTTON_CycleCounter = 0;
    }
}

void BUTTON_Update10ms() {
    gpio_state current_state = GPIO_PinRead(BOARD_BUTTON_PORT, BOARD_BUTTON_PIN);

    // Determining a pressed state
    if (current_state == LOW) {
        BUTTON_StateCounter += 1;

        if (!BUTTON_State && BUTTON_StateCounter > BUTTON_MINIMUM_SAMPLES) {
            BUTTON_StateCounter = 0;
            BUTTON_State = true;
        }
    }
    else {
        BUTTON_StateCounter = 0;
        BUTTON_State = false;
    }

    // Detecting a new press
    if (BUTTON_State && !BUTTON_LastState) {
        BUTTON_OnClick();
        BUTTON_LastState = true;
    }
    // Clearing button pressed state
    else if (!BUTTON_State) {
        BUTTON_LastState = false;
    }
}
