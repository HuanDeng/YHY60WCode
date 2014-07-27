   #include "stm32f2xx.h"
   
   
void RTC_Config(void)
{
      RTC_InitTypeDef RTC_InitStructure;
      RTC_TimeTypeDef RTC_TimeStructure;
      RTC_DateTypeDef RTC_DateStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
  PWR_BackupAccessCmd(ENABLE);//?????????
  
  RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro();
  
  if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x9527)   //????,??RTC????
  {
    
    RTC_WriteProtectionCmd(DISABLE);
  
    RTC_EnterInitMode();
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_InitStructure.RTC_AsynchPrediv = 0x7D-1;
    RTC_InitStructure.RTC_SynchPrediv = 0xFF-1;
    RTC_Init(&RTC_InitStructure);
  
    RTC_TimeStructure.RTC_Seconds = 0x00;
    RTC_TimeStructure.RTC_Minutes = 0x01;
    RTC_TimeStructure.RTC_Hours = 0x01;
    RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
    RTC_SetTime(RTC_Format_BCD,&RTC_TimeStructure);
  
    RTC_DateStructure.RTC_Date = 30;
    RTC_DateStructure.RTC_Month = 5;
    RTC_DateStructure.RTC_WeekDay= RTC_Weekday_Thursday;
    RTC_DateStructure.RTC_Year = 12;
    RTC_SetDate(RTC_Format_BCD,&RTC_DateStructure);
  
    RTC_ExitInitMode();
    RTC_WriteBackupRegister(RTC_BKP_DR0,0X9527);
    RTC_WriteProtectionCmd(ENABLE);
    RTC_WriteBackupRegister(RTC_BKP_DR0,0x9527);  //?????,????
  }
  PWR_BackupAccessCmd(DISABLE);
}
