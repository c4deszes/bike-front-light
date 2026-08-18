#if !defined(APP_STROBE_H_)
#define APP_STROBE_H_

#include <stdint.h>

typedef enum {
    strobe_source_disabled,
    strobe_source_internal_single,
    strobe_source_internal_rapid,
    strobe_source_external_positive,
    strobe_source_external_negative
} strobe_source_t;

void STROBE_Init(void);

/**
 * @brief Loads the strobe settings from the configuration manager
 */
void STROBE_LoadConfig(void);

void STROBE_SetSource(strobe_source_t source);

/**
 * @brief Uses the current strobe signal source to blink the lights, this
 *        function also decides whether blinking is needed
 */
void STROBE_Update1ms(void);

/**
 * @brief Updates less important signals, such as:
 *       - duty cycle value for current measurement
 */
void STROBE_Update100ms(void);

#endif // APP_STROBE_H_
