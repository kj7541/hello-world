#include "stm32f10x.h"
#include "oled.h"
#include "dma.h"
#include "usart.h"
#include "led.h"
#include "NRF.h"
#include "motor.h" 
#include "kalman.h"
#include "ANTO.h"
#include "TFT.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
const u16 POWER=400;
extern u8 ANO_Data[50];
extern u16 PID_time;
int main()
{		
	u8 temp,nrf_buf[33];
	u16 i,j;
	u32 a;
	
	
	Stm32_Clock_Init(9);
	delay_init(72);
	led_init();
	NRF24L01_Init(); 
	PWM2_init(99,0);//MOTOR GROUP 2
	PWM1_init(99,0);//MOTOR GROUP 1
	TB6612FNG_init();
	M1(20,0,20,0);
	USART1_init(72,115200);
	OLED_init();
	temp = 0xff;
	
	
	while(NRF24L01_Check())
	{
		OLED_string(0,0,"NRF ERROR",8*16);
	}
	OLED_string(0,0,"NRF OK",8*16);
	NRF24L01_TX_Mode();

	NRF24L01_TxPacket("kj7541");
	
	#if 0
	while(1)
	{
		printf("kj7541");
		delay_ms(1500);
	}
	
	#endif 
	
	
	while(1)
	{
		i++;
		if(i==20)
			printf("kj7541");
		if(USART_RX_BUF[0]!=0)
		{
			memset(nrf_buf,0,sizeof(nrf_buf));
			USART1_copy_rxbuf(nrf_buf,8);
			USART1_rxbuf_send(0);
			printf(nrf_buf);
			NRF24L01_TxPacket(nrf_buf);
		}
		

		led6=1;
		led7=1;
		led8=1;
		delay_ms(100);
			led6=0;
		led7=0;
		led8=0;
		delay_ms(100);
//		for(pwm1a=300;pwm1a<900;pwm1a++)
//		{
//			NRF24L01_TxPacket(&temp);
//			delay_ms(500);
//		}
		
	}
	

	
	
	
	
	
	
	
	while(!MPU_init())//MPU_init=0 MPU fail
	{
		OLED_string(0,0,"MPU_ERROR",8*16);
	}
		OLED_string(0,0,"MPU_OK",8*16);
		delay_ms(100);//The MPU6050 is not stable,wait for 100 ms
		OLED_string(0,0,"kj7541",8*16);
		TIM5_Ch2_Init();
//		TIM1_ch1_init();

		while(1)
	{	 

	}
}

