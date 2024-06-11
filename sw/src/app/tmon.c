
#include "sam.h"

#include "app/tmon.h"
#include "bsp/pinout.h"
#include "hal/gpio.h"

int8_t TMON_DriveTemperature;
int8_t TMON_McuTemperature;

uint16_t TMON_DriveTemperatureRaw;

static const uint16_t adcToTempTable[13] = {
    401, 547, 748, 1020, 1374, 1807, 2295, 2785, 3221, 3560, 3793, 3938, 4019
}; 

static const int8_t tempTable[13] = {
    -40, -30, -20, -10, 0, 10, 20, 30, 40, 50,
    60, 70, 80
};

static int8_t TMON_Calculate(uint16_t adcValue) {
    if (adcValue <= adcToTempTable[0]) return tempTable[0];
    if (adcValue >= adcToTempTable[12]) return tempTable[12];

    // Find the indices in the lookup table
    uint8_t index = 0;
    while (adcValue > adcToTempTable[index + 1]) {
        index++;
    }

    // Linear interpolation
    uint16_t lowerAdcValue = adcToTempTable[index];
    uint16_t upperAdcValue = adcToTempTable[index + 1];
    int16_t lowerTemp = tempTable[index];
    int16_t upperTemp = tempTable[index + 1];

    int16_t temperature = lowerTemp + ((upperTemp - lowerTemp) * (adcValue - lowerAdcValue)) / (upperAdcValue - lowerAdcValue);

    return (int8_t)temperature;
}

void TMON_Init() {
    GPIO_EnableFunction(BOARD_NTC_PORT, BOARD_NTC_PIN, BOARD_NTC_PINMUX);

    /* Write the calibration data. */
    uint8_t bias_comp = (OTP5_FUSES_REGS->FUSES_OTP5_WORD_0 & FUSES_OTP5_WORD_0_ADC0_BIASCOMP_Msk) >> FUSES_OTP5_WORD_0_ADC0_BIASCOMP_Pos;
    uint8_t bias_ref = (OTP5_FUSES_REGS->FUSES_OTP5_WORD_0 & FUSES_OTP5_WORD_0_ADC0_BIASREFBUF_Msk) >> FUSES_OTP5_WORD_0_ADC0_BIASREFBUF_Pos;

    ADC0_REGS->ADC_CALIB = ADC_CALIB_BIASCOMP(bias_comp) | ADC_CALIB_BIASREFBUF(bias_ref);

    ADC0_REGS->ADC_REFCTRL = ADC_REFCTRL_REFSEL_INTVCC2;

    ADC0_REGS->ADC_AVGCTRL = ADC_AVGCTRL_SAMPLENUM_8;

    ADC0_REGS->ADC_CTRLB = ADC_CTRLB_PRESCALER_DIV4;
    ADC0_REGS->ADC_CTRLC = ADC_CTRLC_RESSEL_12BIT | ADC_CTRLC_FREERUN_Msk;

    ADC0_REGS->ADC_INPUTCTRL = ADC_INPUTCTRL_MUXNEG_GND | ADC_INPUTCTRL_MUXPOS_AIN5;

    ADC0_REGS->ADC_CTRLA = ADC_CTRLA_ENABLE_Msk;

    while ((ADC0_REGS->ADC_SYNCBUSY & ADC_SYNCBUSY_ENABLE_Msk) != 0);

    // Start ADC0 in free running mode
    ADC0_REGS->ADC_SWTRIG = ADC_SWTRIG_START_Msk | ADC_SWTRIG_FLUSH_Msk;
}

void TMON_Update100ms() {
    if ((ADC0_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk) != 0) {
        ADC0_REGS->ADC_INTFLAG = ADC_INTFLAG_RESRDY_Msk;
        uint16_t adc_result = ADC0_REGS->ADC_RESULT;
        TMON_DriveTemperatureRaw = adc_result;
        TMON_DriveTemperature = TMON_Calculate(adc_result);
    }
}

// int8_t TMON_GetTemperature() {
//     return 0;
// }
