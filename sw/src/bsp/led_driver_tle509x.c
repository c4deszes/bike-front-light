#include "bsp/pinout.h"

#include "bsp/light_control.h"
#include "hal/tcc.h"
#include "hal/gpio.h"
#include "common/swtimer.h"

#include "bsp/config.h"

static const gpio_pin_output_configuration output = {
    .drive = NORMAL,
    .input = false
};

void LIGHTCONTROL_Init(void) {
    // GPIO setup
    // TCC setup
    // DAC setup

    GPIO_PinWrite(TLD509x_PWMI_PORT, TLD509x_PWMI_PIN, LOW);
    GPIO_SetupPinOutput(TLD509x_PWMI_PORT, TLD509x_PWMI_PIN, &output);

    GPIO_PinWrite(TLD509x_ISET_PORT, TLD509x_ISET_PIN, HIGH);
    GPIO_SetupPinOutput(ILD8150_DIM_PORT, ILD8150_DIM_PIN, &output);

    /* Timer setup */
    TCC_Reset(TCC2);

    pwm_channels[TLD509x_PWMI_WO].cc = LIGHTCONTROL_BRIGHTNESS_MAX;
    pwm_channels[TLD509x_PWMI_WO].drv_inv = false;

    TCC_SetupNormalPwm(TCC2, LED_DRIVER_PWM_FREQUENCY - 1, pwm_channels);
    TCC_Enable(TCC2);
}

/*
Timing notes:
    EN startup time is 100us

    PWMI min duty is 6us

    TurnOff delay is 10ms

    PWMO frequency 

*/

void LIGHTCONTROL_Update10ms(void) {
    // 1. Set PWM duty
    // 2. Set Shutdown if PWM is low

    // 3. Poll VMON
        // if vmon is outside boundaries
        // too low -> short circuit
        // too high -> open circuit

    // 4. In case of short or open circuit do shutdown
        // retry every 4 seconds
        // dim pin can be high
}
