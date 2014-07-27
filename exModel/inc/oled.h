
#ifndef __OLED_H__
#define __OLED_H__


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Global Variables
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF


extern unsigned char const Ascii5x8[];
extern unsigned char const nAsciiDot8x16[];
extern void OLED_Init_I(void);
extern void Display8x16Str(unsigned char page, unsigned char column, unsigned char *str);
extern void Display5x8Str(unsigned char page, unsigned char column, unsigned char *str);
#endif



