#include "oled.h"
#include "stm32f2xx.h"
#include "SysTick.h"
                                    
void OLEDPortInit(void)
{
      GPIO_InitTypeDef  GPIO_InitStructure;
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOB, ENABLE);
      GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
      GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
      GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
      GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_6;
      GPIO_Init(GPIOB,&GPIO_InitStructure);
      
      GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2;
      GPIO_Init(GPIOE,&GPIO_InitStructure);

}
void SCL(u8 i)
{
      if(i)GPIO_SetBits(GPIOB,GPIO_Pin_6);
      else GPIO_ResetBits(GPIOB,GPIO_Pin_6);
}
void SDA(u8 i)
{
      if(i)GPIO_SetBits(GPIOB,GPIO_Pin_7);
      else GPIO_ResetBits(GPIOB,GPIO_Pin_7);
}
void RES(u8 i)
{
      if(i)GPIO_SetBits(GPIOE,GPIO_Pin_0);
      else GPIO_ResetBits(GPIOE,GPIO_Pin_0);
}
void SA0(u8 i)
{
      if(i)GPIO_SetBits(GPIOE,GPIO_Pin_1);
      else GPIO_ResetBits(GPIOE,GPIO_Pin_1);
}




//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void uDelay(unsigned char l)
{
	//while(l--);
      delay_us(l);
}


void Delay(unsigned char n)
{
unsigned char i,j,k;

	for(k=0;k<n;k++)
	{
		for(i=0;i<131;i++)
		{
			for(j=0;j<15;j++)
			{
				uDelay(203);	
			}
		}
	}
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Read/Write Sequence
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void I2C_O(unsigned char mcmd)
{
unsigned char length = 8;			// Send Command

	while(length--)
	{
		if(mcmd & 0x80)
		{
			SDA(1);
		}
		else
		{
			SDA(0);
		}
		uDelay(2);
		SCL(1);
		uDelay(2);
		SCL(0);
		uDelay(2);
		mcmd = mcmd << 1;
	}
}


void I2C_Ack()
{
	SDA(1);
	uDelay(2);
	SCL(1);
	uDelay(2);
	SCL(0);
	uDelay(2);
}


void I2C_NAck()
{
	SDA(0);
	uDelay(2);
	SCL(1);
	uDelay(2);
	SCL(0);
	uDelay(2);
}


void I2C_Start()
{
	SDA(0);
	uDelay(2);
	SCL(1);
	uDelay(2);
	SCL(0);
	uDelay(2);
	I2C_O(0x78);
	I2C_Ack();
}


void I2C_Stop()
{
	SCL(1);
	uDelay(2);
	SDA(0);
	uDelay(2);
	SDA(1);
	uDelay(2);
}


void Write_Command(unsigned char Data)
{
	I2C_Start();
	I2C_O(0x00);
	I2C_Ack();
	I2C_O(Data);
	I2C_Ack();
	I2C_Stop();
}


void Write_Data(unsigned char Data)
{
	I2C_Start();
	I2C_O(0x40);
	I2C_Ack();
	I2C_O(Data);
	I2C_Ack();
	I2C_Stop();
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Instruction Setting
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Start_Column(unsigned char d)
{
	Write_Command(0x00+d%16);		// Set Lower Column Start Address for Page Addressing Mode
						//   Default => 0x00
	Write_Command(0x10+d/16);		// Set Higher Column Start Address for Page Addressing Mode
						//   Default => 0x10
}


void Set_Addressing_Mode(unsigned char d)
{
	Write_Command(0x20);			// Set Memory Addressing Mode
	Write_Command(d);			//   Default => 0x02
						//     0x00 => Horizontal Addressing Mode
						//     0x01 => Vertical Addressing Mode
						//     0x02 => Page Addressing Mode
}


void Set_Column_Address(unsigned char a, unsigned char b)
{
	Write_Command(0x21);			// Set Column Address
	Write_Command(a);			//   Default => 0x00 (Column Start Address)
	Write_Command(b);			//   Default => 0x7F (Column End Address)
}


void Set_Page_Address(unsigned char a, unsigned char b)
{
	Write_Command(0x22);			// Set Page Address
	Write_Command(a);			//   Default => 0x00 (Page Start Address)
	Write_Command(b);			//   Default => 0x07 (Page End Address)
}


void Set_Start_Line(unsigned char d)
{
	Write_Command(0x40|d);			// Set Display Start Line
						//   Default => 0x40 (0x00)
}


void Set_Contrast_Control(unsigned char d)
{
	Write_Command(0x81);			// Set Contrast Control
	Write_Command(d);			//   Default => 0x7F
}


void Set_Charge_Pump(unsigned char d)
{
	Write_Command(0x8D);			// Set Charge Pump
	Write_Command(d);			//   Default => 0x10
						//     0x10 => Disable Charge Pump
						//     0x14 => Enable Charge Pump
}


void Set_Segment_Remap(unsigned char d)
{
	Write_Command(d);			// Set Segment Re-Map
						//   Default => 0xA0
						//     0xA0 => Column Address 0 Mapped to SEG0
						//     0xA1 => Column Address 0 Mapped to SEG127
}


void Set_Entire_Display(unsigned char d)
{
	Write_Command(d);			// Set Entire Display On / Off
						//   Default => 0xA4
						//     0xA4 => Normal Display
						//     0xA5 => Entire Display On
}


void Set_Inverse_Display(unsigned char d)
{
	Write_Command(d);			// Set Inverse Display On/Off
						//   Default => 0xA6
						//     0xA6 => Normal Display
						//     0xA7 => Inverse Display On
}


void Set_Multiplex_Ratio(unsigned char d)
{
	Write_Command(0xA8);			// Set Multiplex Ratio
	Write_Command(d);			//   Default => 0x3F (1/64 Duty)
}


void Set_Display_On_Off(unsigned char d)	
{
	Write_Command(d);			// Set Display On/Off
						//   Default => 0xAE
						//     0xAE => Display Off
						//     0xAF => Display On
}


void Set_Start_Page(unsigned char d)
{
	Write_Command(0xB0|d);			// Set Page Start Address for Page Addressing Mode
						//   Default => 0xB0 (0x00)
}


void Set_Common_Remap(unsigned char d)
{
	Write_Command(d);			// Set COM Output Scan Direction
						//   Default => 0xC0
						//     0xC0 => Scan from COM0 to 63
						//     0xC8 => Scan from COM63 to 0
}


void Set_Display_Offset(unsigned char d)
{
	Write_Command(0xD3);			// Set Display Offset
	Write_Command(d);			//   Default => 0x00
}


void Set_Display_Clock(unsigned char d)
{
	Write_Command(0xD5);			// Set Display Clock Divide Ratio / Oscillator Frequency
	Write_Command(d);			//   Default => 0x80
						//     D[3:0] => Display Clock Divider
						//     D[7:4] => Oscillator Frequency
}


void Set_Precharge_Period(unsigned char d)
{
	Write_Command(0xD9);			// Set Pre-Charge Period
	Write_Command(d);			//   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
						//     D[3:0] => Phase 1 Period in 1~15 Display Clocks
						//     D[7:4] => Phase 2 Period in 1~15 Display Clocks
}


void Set_Common_Config(unsigned char d)
{
	Write_Command(0xDA);			// Set COM Pins Hardware Configuration
	Write_Command(d);			//   Default => 0x12
						//     Alternative COM Pin Configuration
						//     Disable COM Left/Right Re-Map
}


void Set_VCOMH(unsigned char d)
{
	Write_Command(0xDB);			// Set VCOMH Deselect Level
	Write_Command(d);			//   Default => 0x20 (0.77*VCC)
}


void Set_NOP()
{
	Write_Command(0xE3);			// Command for No Operation
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_RAM(unsigned char Data)
{
unsigned char i,j;

	for(i=0;i<8;i++)
	{
		Set_Start_Page(i);
		Set_Start_Column(0x00);

		I2C_Start();
		I2C_O(0x40);
		I2C_Ack();

		for(j=0;j<128;j++)
		{
			I2C_O(Data);
			I2C_Ack();
		}

		I2C_Stop();
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Partial or Full Screen)
//
//    a: Start Page
//    b: End Page
//    c: Start Column
//    d: Total Columns
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_Block(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
unsigned char i,j;
	
	for(i=a;i<(b+1);i++)
	{
		Set_Start_Page(i);
		Set_Start_Column(c);

		I2C_Start();
		I2C_O(0x40);
		I2C_Ack();

		for(j=0;j<d;j++)
		{
			I2C_O(Data);
			I2C_Ack();
		}

		I2C_Stop();
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Checkboard (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Checkerboard()
{
unsigned char i,j;
	
	for(i=0;i<8;i++)
	{
		Set_Start_Page(i);
		Set_Start_Column(0x00);

		I2C_Start();
		I2C_O(0x40);
		I2C_Ack();

		for(j=0;j<64;j++)
		{
			I2C_O(0x55);
			I2C_Ack();
			I2C_O(0xaa);
			I2C_Ack();
		}

		I2C_Stop();
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Frame (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Frame()
{
unsigned char i,j;
	
	Set_Start_Page(0x00);
	Set_Start_Column(XLevel);

	I2C_Start();
	I2C_O(0x40);
	I2C_Ack();

	for(i=0;i<Max_Column;i++)
	{
		I2C_O(0x01);
		I2C_Ack();
	}

	I2C_Stop();

	Set_Start_Page(0x07);
	Set_Start_Column(XLevel);

	I2C_Start();
	I2C_O(0x40);
	I2C_Ack();

	for(i=0;i<Max_Column;i++)
	{
		I2C_O(0x80);
		I2C_Ack();
	}

	I2C_Stop();

	for(i=0;i<8;i++)
	{
		Set_Start_Page(i);

		for(j=0;j<Max_Column;j+=(Max_Column-1))
		{
			Set_Start_Column(XLevel+j);

			Write_Data(0xFF);
		}
	}
}
void DisplayDot(unsigned char page, unsigned char column, unsigned char dot)
{
      Set_Start_Page(page);
      Set_Start_Column(column);
          
      I2C_Start();
	I2C_O(0x40);
	I2C_Ack();
      if(dot)I2C_O(0xff);
      else I2C_O(0);
      I2C_Ack();
      I2C_O(0x00);
      I2C_Ack();
      I2C_Stop();
      
}
/******************************************************************************
  * @function name : Display8x5
  * @brief  Display Area 8*5
  * @param  :
  * @retval :
  */
void Display5x8(unsigned char page, unsigned char column, unsigned char ascii)
{
    unsigned char i = 0;
    unsigned int temp = 0;
    
    temp = (ascii-32)*16;
    
      Set_Start_Page(page);
      Set_Start_Column(column);
          
      I2C_Start();
	I2C_O(0x40);
	I2C_Ack(); 
    for (i=0; i<5; i++)
    {
           I2C_O(Ascii5x8[temp++]);
           I2C_Ack();
    }
    I2C_O(0x00);
    I2C_Ack();
    I2C_Stop();
}

/******************************************************************************
  * @function name : Display8x5
  * @brief  Display Area 8*5
  * @param  :
  * @retval :
  */
void Display5x8Str(unsigned char page, unsigned char column, unsigned char *str)
{
    unsigned char i = 0;
    unsigned int temp = 0;
    
      Set_Start_Page(page);
      Set_Start_Column(column);
          
      I2C_Start();
	I2C_O(0x40);
	I2C_Ack(); 
    
    while(*str != '\0')
    {
        temp = (*str-32)*5;
        for (i=0; i<5; i++)
        {
           I2C_O(Ascii5x8[temp++]);
           I2C_Ack();
        }
        str++;
    }
      I2C_O(0x00);
	I2C_Ack();
	I2C_Stop();
}
void Display8x16Str(unsigned char page, unsigned char column, unsigned char *str)
{
    unsigned char i = 0;
    //unsigned char temp = 0;
    unsigned int padd= 0;

    while(*str != '\0')
    {
      Set_Start_Page(page);
      Set_Start_Column(column);
          
      I2C_Start();
	I2C_O(0x40);
	I2C_Ack();  
          
      padd = (*str-32)*16;
      for (i=0; i<8; i++)
      {
           I2C_O(nAsciiDot8x16[padd+i]);
           I2C_Ack();
      }
      I2C_O(0x00);
	I2C_Ack();
	I2C_Stop();
        
      Set_Start_Page(page+1);
      Set_Start_Column(column);
      I2C_Start();
	I2C_O(0x40);
	I2C_Ack(); 
        for (i=8; i<16; i++)
        {
           I2C_O(nAsciiDot8x16[padd+i]);
           I2C_Ack();
        }
      I2C_O(0x00);
	I2C_Ack();
	I2C_Stop();
      str++;
      column += 8;
   }
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Pattern (Partial or Full Screen)
//
//    a: Start Page
//    b: End Page
//    c: Start Column
//    d: Total Columns
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Pattern(const unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
 const unsigned char *Src_Pointer;
unsigned char i,j;

	Src_Pointer=Data_Pointer;
	for(i=a;i<(b+1);i++)
	{
		Set_Start_Page(i);
		Set_Start_Column(c);

		I2C_Start();
		I2C_O(0x40);
		I2C_Ack();

		for(j=0;j<d;j++)
		{
			I2C_O(*Src_Pointer);
			I2C_Ack();
			Src_Pointer++;
		}

		I2C_Stop();
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Vertical / Fade Scrolling (Partial or Full Screen)
//
//    a: Scrolling Direction
//       "0x00" (Upward)
//       "0x01" (Downward)
//    b: Set Top Fixed Area
//    c: Set Vertical Scroll Area
//    d: Set Numbers of Row Scroll per Step
//    e: Set Time Interval between Each Scroll Step
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e)
{
unsigned int i,j;	

	Write_Command(0xA3);			// Set Vertical Scroll Area
	Write_Command(b);			//   Default => 0x00 (Top Fixed Area)
	Write_Command(c);			//   Default => 0x40 (Vertical Scroll Area)

	switch(a)
	{
		case 0:
			for(i=0;i<c;i+=d)
			{
				Set_Start_Line(i);
				for(j=0;j<e;j++)
				{
					uDelay(200);
				}
			}
			break;
		case 1:
			for(i=0;i<c;i+=d)
			{
				Set_Start_Line(c-i);
				for(j=0;j<e;j++)
				{
					uDelay(200);
				}
			}
			break;
	}
	Set_Start_Line(0x00);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Continuous Horizontal Scrolling (Partial or Full Screen)
//
//    a: Scrolling Direction
//       "0x00" (Rightward)
//       "0x01" (Leftward)
//    b: Define Start Page Address
//    c: Define End Page Address
//    d: Set Time Interval between Each Scroll Step in Terms of Frame Frequency
//    e: Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Horizontal_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e)
{
	Write_Command(0x26|a);			// Horizontal Scroll Setup
	Write_Command(0x00);			//           => (Dummy Write for First Parameter)
	Write_Command(b);
	Write_Command(d);
	Write_Command(c);
	Write_Command(0x00);			//           => (Dummy Write for Fifth Parameter)
	Write_Command(0xFF);			//           => (Dummy Write for Sixth Parameter)
	Write_Command(0x2F);			// Activate Scrolling
	Delay(e);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Continuous Vertical / Horizontal / Diagonal Scrolling (Partial or Full Screen)
//
//    a: Scrolling Direction
//       "0x00" (Vertical & Rightward)
//       "0x01" (Vertical & Leftward)
//    b: Define Start Row Address (Horizontal / Diagonal Scrolling)
//    c: Define End Page Address (Horizontal / Diagonal Scrolling)
//    d: Set Top Fixed Area (Vertical Scrolling)
//    e: Set Vertical Scroll Area (Vertical Scrolling)
//    f: Set Numbers of Row Scroll per Step (Vertical / Diagonal Scrolling)
//    g: Set Time Interval between Each Scroll Step in Terms of Frame Frequency
//    h: Delay Time
//    * d+e must be less than or equal to the Multiplex Ratio...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Continuous_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f, unsigned char g, unsigned char h)
{
	Write_Command(0xA3);			// Set Vertical Scroll Area
	Write_Command(d);			//   Default => 0x00 (Top Fixed Area)
	Write_Command(e);			//   Default => 0x40 (Vertical Scroll Area)
	Write_Command(0x29+a);			// Continuous Vertical & Horizontal Scroll Setup
	Write_Command(0x00);			//           => (Dummy Write for First Parameter)
	Write_Command(b);
	Write_Command(g);
	Write_Command(c);
	Write_Command(f);
	Write_Command(0x2F);			// Activate Scrolling
	Delay(h);
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Deactivate Scrolling (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Deactivate_Scroll()
{
	Write_Command(0x2E);			// Deactivate Scrolling
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade In (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_In()
{
      unsigned int i;	
	Set_Display_On_Off(0xAF);
	for(i=0;i<(Brightness+1);i++)
	{
		Set_Contrast_Control(i);
		uDelay(200);
		uDelay(200);
		uDelay(200);
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade Out (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_Out()
{
      unsigned int i;	
	for(i=(Brightness+1);i>0;i--)
	{
		Set_Contrast_Control(i-1);
		uDelay(200);
		uDelay(200);
		uDelay(200);
	}
	Set_Display_On_Off(0xAE);
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Sleep Mode
//
//    "0x00" Enter Sleep Mode
//    "0x01" Exit Sleep Mode
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Sleep(unsigned char a)
{
	switch(a)
	{
		case 0:
			Set_Display_On_Off(0xAE);
			Set_Entire_Display(0xA5);
			break;
		case 1:
			Set_Entire_Display(0xA4);
			Set_Display_On_Off(0xAF);
			break;
	}
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Connection Test
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Test()
{
unsigned char i;

	RES(0);
	for(i=0;i<200;i++)
	{
		uDelay(200);
	}
	RES(1);
	Set_Entire_Display(0xA5);		// Enable Entire Display On (0xA4/0xA5)
	while(1)
	{
		Set_Display_On_Off(0xAF);	// Display On (0xAE/0xAF)
		Delay(2);
		Set_Display_On_Off(0xAE);	// Display Off (0xAE/0xAF)
		Delay(2);
	}
}
void OLED_Init_I(void)				// VCC Generated by Internal DC/DC Circuit
{
      unsigned char i;
      OLEDPortInit();
	RES(0);
	for(i=0;i<200;i++)
	{
		uDelay(200);
	}
	RES(1);
	Set_Display_On_Off(0xAE);		// Display Off (0xAE/0xAF)
	Set_Display_Clock(0x80);		// Set Clock as 100 Frames/Sec
	Set_Multiplex_Ratio(0x3F);		// 1/64 Duty (0x0F~0x3F)
	Set_Display_Offset(0x00);		// Shift Mapping RAM Counter (0x00~0x3F)
	Set_Start_Line(0x00);			// Set Mapping RAM Display Start Line (0x00~0x3F)
	Set_Charge_Pump(0x14);			// Enable Built-in DC/DC Converter (0x10/0x14)
	Set_Addressing_Mode(0x02);		// Set Page Addressing Mode (0x00/0x01/0x02)
	Set_Segment_Remap(0xA1);		// Set SEG/Column Mapping (0xA0/0xA1)
	Set_Common_Remap(0xC8);			// Set COM/Row Scan Direction (0xC0/0xC8)
	Set_Common_Config(0x12);		// Set Sequential Configuration (0x02/0x12)
	Set_Contrast_Control(Brightness);	// Set SEG Output Current
	Set_Precharge_Period(0xF1);		// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	Set_VCOMH(0x40);			// Set VCOM Deselect Level
	Set_Entire_Display(0xA4);		// Disable Entire Display On (0xA4/0xA5)
	Set_Inverse_Display(0xA6);		// Disable Inverse Display On (0xA6/0xA7)
	Fill_RAM(0x00);				// Clear Screen
	Set_Display_On_Off(0xAF);		// Display On (0xAE/0xAF)
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Main Program
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//void Testmain()
//{

//	OLED_Init_I();
//	Display8x16Str(0,0,"sdadsdsa");
//      Display5x8Str(1,0,"i love you");
//}

