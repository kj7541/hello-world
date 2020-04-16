#ifndef _oled_h
#define _oled_h
#include "sys.h"
#include "delay.h"
#include "stdio.h"

//#define OLED_iic_sda PCout(11)
//#define OLED_read_sda PCin(11) // ‰»ÎSDA	
//#define OLED_iic_scl PCout(12) 
//#define MAX_column 128
//#define MAX_row 64

#define OLED_iic_sda PDout(3)
#define OLED_read_sda PDin(3) // ‰»ÎSDA	
#define OLED_iic_scl PDout(4) 
#define MAX_column 128
#define MAX_row 64


void OLED_iic_sda_out(void);
void OLED_iic_sda_in(void);
void OLED_iic_start(void);
void OLED_iic_stop(void);
void OLED_iic_wait_ack(void);
void OLED_iic_ack(void);
void OLED_iic_Nack(void);
void OLED_iic_send_byte(u8 byte);
void OLED_cmd(u8 cmd);
void OLED_dat(u8 dat);
void OLED_byte(u8 byte,u8 cmd);
void fill_picture(u8 fill_dat);
void OLED_pos(u8 x,u8 y);
void OLED_display_on(void);
void OLED_display_off(void);
void OLED_clear(void);
void OLED_on(void);
void OLED_char(u8 x,u8 y,u8 chr,u8 char_size);
void OLED_number(u8 x,u8 y,u8 num);
u32 OLED_pow(u8 x,u8 y);

void OLED_value(u8 x,u8 y,float value);
void OLED_string(u8 x,u8 y,u8 *chr,u8 char_size);
void OLED_string_value(u8 x,u8 y,float value);







void OLED_init(void);
	
















#endif



