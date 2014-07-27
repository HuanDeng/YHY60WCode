#include "stm32f2xx.h"
#ifndef _ADCC_H__
#define _ADCC_H__

#define Pch0 0
#define Pch1 1
#define Pch2 3

#define BATch 10

extern void ADCIO_Init(void);
extern uint16_t Get_ADC(u8 ch);
#endif
