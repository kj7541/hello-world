#ifndef _usart_h
#define _usart_h
#include "sys.h"
#include "exti.h"
#include "stdio.h"
#include "string.h"
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;
extern u8 USART_flag;
void USART1_init(u32 pclk2,u32 bound);
void USART1_send_one_byte(u8 byte);
//void USART1_Send(u8 *buf,u8 len);
//void USART1_send(u8 *buf);
void USART1_send_one_value(float value);
void USART1_send(u8 *buf);

void USART1_rxbuf_send(u8 flag);
void USART1_rxbuf_clear(void);
void USART1_copy_rxbuf(u8 *buf,u8 num);

#endif


