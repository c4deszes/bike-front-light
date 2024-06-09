#if !defined(APP_STROBE_H_)
#define APP_STROBE_H_

#include <stdint.h>

typedef enum {
    strobe_source_disabled,
    strobe_source_internal_single,
    strobe_source_external_positive,
    strobe_source_external_negative
} strobe_source_t;

void STROBE_Init(void);

void STROBE_SetSource(strobe_source_t source);

strobe_source_t STROBE_ConvertSource(uint8_t config);

/**
 * @brief Uses the current strobe signal source to blink the lights, this
 *        function also decides whether blinking is needed
 */
void STROBE_Update1ms(void);

#endif // APP_STROBE_H_
