#ifndef _usart_h
#define _usart_h
#include "sys.h"
#include "exti.h"
#include "stdio.h"
#include "string.h"
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
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

