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


int main(void)
{
GPIO_InitTypeDef  GPIO_InitStructure;


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
      


Testmain();
//OLEDPortInit();
  while (1)
  {
    /* Set PG6 and PG8 */
    //GPIOE->BSRRL = GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_14;
        //GPIO_SetBits(GPIOE,GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
        //GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_5);
            delay_ms(1000);
        
        //GPIO_ResetBits(GPIOE,GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
         //GPIO_ResetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_5);
    //GPIOE->BSRRH = GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_14;    
            delay_ms(1000);

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
