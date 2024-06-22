#include "sam.h"

#include <stdint.h>
#include <stdbool.h>

#include "bsp/pinout.h"

#include "hal/gpio.h"

bool BUTTON_LastState;
bool BUTTON_State;
uint8_t BUTTON_StateCounter;

uint8_t BUTTON_CycleCounter;

#define BUTTON_MINIMUM_SAMPLES 2
#define BUTTON_CYCLES 6

static const gpio_pin_input_configuration input = {
    .pull = PULLUP,
    .sample = CONTINUOUS
};

void BUTTON_Init() {
    // Setup EIC
    GPIO_SetupPinInput(BOARD_BUTTON_PORT, BOARD_BUTTON_PIN, &input);

    BUTTON_State = false;
    BUTTON_LastState = false;
    BUTTON_StateCounter = 0;
    BUTTON_CycleCounter = 0;
}

void BUTTON_OnClick() {
    BUTTON_CycleCounter += 1;

    if (BUTTON_CycleCounter >= BUTTON_CYCLES) {
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

// void EIC_Handler() {

// }

