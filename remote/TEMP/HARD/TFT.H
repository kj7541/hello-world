#ifndef _TFT_H
#define _TFT_H
#include "sys.h"
#include "delay.h"
#include "stdio.h"
#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//»ÒÉ«0 3165 00110 001011 00101
#define GRAY1   0x8410      	//»ÒÉ«1      00000 000000 00000
#define GRAY2   0x4208      	//»ÒÉ«2  1111111111011111




#define dat 1
#define cmd 0
#define LCD_scl PDout(4)
#define LCD_sda PDout(3)
#define LCD_dc PDout(1)
#define LCD_cs PDout(0)

extern const unsigned char X8Y16[][16];
extern const unsigned char X6Y8[][6];
extern u8 NUMx8y16[][16];
void LCD_1byte(u8 byte,u8 dc);
void LCD_1byte2(u16 byte,u8 dc);

void LCD_init(void);

void LCD_set_region(u8 xs,u8 xe,u8 ys,u8 ye);
void LCD_draw_point(u8 x,u8 y,u16 data);
void LCD_show_bit(u8 x,u8 y,u16 color,u8 byte);

void LCD_clear(u16 color);
void LCD_char(u8 x,u8 y,u16 color,u8 chr);
void LCD_string(u8 x,u8 y,u16 color,u8 *buf);
void LCD_value_(u8 x,u8 y,u8 color,float value);
void LCD_number(u8 x,u8 y,u16 color,u8 num);
u32 LCD_pow(u8 x,u8 y);
void LCD_value(u8 x,u8 y,u16 color,float value);







#endif




