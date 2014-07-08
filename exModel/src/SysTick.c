#include "stm32f2xx.h"

vu32 TimingDelay=0;

void SysTick_Configuration(void)
{
	if (SysTick_Config(SystemCoreClock/1000))
	{ 
		while (1);
	}
}


void delay_ms(vu32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);
}

