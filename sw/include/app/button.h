#if !defined(APP_BUTTON_H_)
#define APP_BUTTON_H_

#include <stdint.h>

extern uint8_t BUTTON_CycleCounter;

void BUTTON_Init();

void BUTTON_Update10ms();

#endif // APP_BUTTON_H_
