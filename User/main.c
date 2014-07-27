/**
  ******************************************************************************
  * @file    GPIO/IOToggle/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "SysTick.h"
#include "oled.h"
#include "adc.h"
#include "stdio.h"
#include "RTC.h"


int main(void)
{
GPIO_InitTypeDef  GPIO_InitStructure;
char strbuf[10];

  /* GPIOE Periph clock enable */
  SysTick_Configuration();
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOB, ENABLE);

  /* Configure PG6 and PG8 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  OLED_Init_I();
  ADCIO_Init();
     
  while (1)
  {
        
      sprintf(strbuf,"CH 0=%.2fV",Get_ADC(Pch0)*0.000806);
      Display5x8Str(0,0,"        ");
      Display5x8Str(0,0,strbuf); 
      Display5x8Str(1,0,"        ");
      sprintf(strbuf,"CH 1=%.2fV",Get_ADC(Pch1)*0.000806);
      Display5x8Str(1,0,strbuf);
      Display5x8Str(2,0,"        ");
      sprintf(strbuf,"CH 2=%.2fV",Get_ADC(Pch2)*0.000806);
      Display5x8Str(2,0,strbuf);        
      delay_ms(100);
  }
}



#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 

  while (1)
  {
  }
}
#endif


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
