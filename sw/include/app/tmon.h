#if !defined(APP_TMON_H_)
#define APP_TMON_H_

#include <stdint.h>

extern int8_t TMON_DriveTemperature;
extern uint16_t TMON_DriveTemperatureRaw;

void TMON_Init(void);

void TMON_Update100ms(void);

#endif // APP_TMON_H_
