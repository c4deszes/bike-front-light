#if !defined(BSP_LIGHT_CONTROL_H_)
#define BSP_LIGHT_CONTROL_H_

#include <stdint.h>

#define LIGHTCONTROL_BRIGHTNESS_MIN 0
#define LIGHTCONTROL_BRIGHTNESS_MAX 1000

typedef enum {
    lightcontrol_feature_state_off,
    lightcontrol_feature_state_ok,
    lightcontrol_feature_state_partial_error,
    lightcontrol_feature_state_error
} lightcontrol_feature_state_t;

void LIGHTCONTROL_Init(void);

void LIGHTCONTROL_Update10ms(void);

/**
 * @brief Sets the brightness of the given feature, the brightness `LIGHTCONTROL_BRIGHTNESS_MIN` is
 *        equivalent to the feature being off, the value `LIGHTCONTROL_BRIGHTNESS_MAX` being the
 *        maximum brightness and anything in between is mapped 
 * 
 * @param brightness 
 */
void LIGHTCONTROL_SetBrightness(uint16_t brightness);

lightcontrol_feature_state_t LIGHTCONTROL_GetMainBeamState(void);

#endif // BSP_LIGHT_CONTROL_H_
