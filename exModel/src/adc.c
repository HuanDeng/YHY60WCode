#include "adc.h"
#include "stm32f2xx.h"
#include "SysTick.h"
static void GPIO_Configuration(void)
{               
      GPIO_InitTypeDef GPIO_InitStructure;       
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);       
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
}
static void Gyro_ADC_Configuration(void)
{
      ADC_InitTypeDef       ADC_InitStructure;
      ADC_CommonInitTypeDef ADC_CommonInitStructure;               
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);       
      /* ADC Common Init **********************************************************/
      ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
      ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
      ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
      ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
      ADC_CommonInit(&ADC_CommonInitStructure);

      /* ADC2 Init ****************************************************************/
      ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
      ADC_InitStructure.ADC_ScanConvMode = DISABLE;
      ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
      ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
      ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
      ADC_InitStructure.ADC_NbrOfConversion = 1;
      ADC_Init(ADC2, &ADC_InitStructure);       
      ADC_ITConfig(ADC2, ADC_IT_EOC, ENABLE);
      /* Enable ADC2 */
      ADC_Cmd(ADC2, ENABLE);
      /* Start ADC2 Software Conversion */
      //ADC_SoftwareStartConv(ADC2);
}
void ADCIO_Init(void)
{
      GPIO_Configuration();
      Gyro_ADC_Configuration();
}
uint16_t Get_ADC(u8 ch)
{
      ADC_RegularChannelConfig(ADC2, ch, 1, ADC_SampleTime_15Cycles);
      ADC_SoftwareStartConv(ADC2);
      while(!(ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC)==SET));
      return(ADC_GetConversionValue(ADC2));
}
