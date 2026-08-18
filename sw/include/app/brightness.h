#if !defined(APP_BRIGHTNESS_H_)
#define APP_BRIGHTNESS_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    brightness_mode_off,
    brightness_mode_standard,
    brightness_mode_adaptive,
    brightness_mode_emergency,
    brightness_mode_safety,
    brightness_mode_max
} brightness_mode_t;

typedef enum {
    brightness_output_main
} brightness_output_t;

void BRIGHTNESS_Init(void);

void BRIGHTNESS_LoadConfig(void);

void BRIGHTNESS_SetMode(brightness_mode_t mode);

void BRIGHTNESS_SetTarget(uint16_t target);

void BRIGHTNESS_SetStrobe(bool strobe);

uint16_t BRIGHTNESS_GetOutput(brightness_output_t output);

void BRIGHTNESS_Update10ms(void);

#endif // APP_BRIGHTNESS_H_
