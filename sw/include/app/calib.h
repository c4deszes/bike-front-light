#ifndef APP_CALIB_H
#define APP_CALIB_H

#include <stdint.h>
#include <stdbool.h>

void CALIB_Init(void);

void CALIB_Save(void);

bool CALIB_GetVoltageCalib(uint16_t* slope_calib, int16_t* offset_calib);

bool CALIB_SetVoltageCalib(uint16_t slope_calib, int16_t offset_calib);

#endif
