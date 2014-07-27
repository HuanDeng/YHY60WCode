#ifndef __SYSTICK_H_
#define __SYSTICK_H_

extern void SysTick_Configuration(void);
extern void delay_ms(volatile unsigned long);
extern void delay_us(vu32 nTime);
#endif
