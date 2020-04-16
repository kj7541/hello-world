#include "exti.h"
//void EXTI0_IRQHandler(void)
//{
//	delay_ms(10);
//	if(k0==1)
//	{
//		led7=!led7;
//	}
//	EXTI->PR=1<<0;
//}
//void EXTI2_IRQHandler(void)
//{
//	delay_ms(10);
//	if(k1==0)
//	{
//		led1=!led1;
//	}
//	EXTI->PR=1<<2;
//}
//void EXTI3_IRQHandler(void)
//{
//	delay_ms(10);
//	if(k2==0)
//	{
//		led2=!led2;
//	}
//	EXTI->PR=1<<3;
//}
//void EXTI4_IRQHandler(void)
//{
//	delay_ms(10);
//	if(k3==0)
//	{
//		led4=!led4;
//	}
//	EXTI->PR=1<<4;
//}	

	

void NVIC_group(u8 group)
{
	u32 temp,temp1;	  
	temp1=(~group)&0x07;//ȡ����λ
	temp1<<=8;
	temp=SCB->AIRCR;  //��ȡ��ǰ������
	temp&=0X0000F8FF; //�����ǰ����
	temp|=0X05FA0000; //д��Կ��
	temp|=temp1;	   
	SCB->AIRCR=temp;  //���÷���	 
}
void NVIC_priority(u8 Pre,u8 Sub,u8 channel,u8 group)
{
	u32 temp;	
	NVIC_group(group);//���÷���
	temp=Pre<<(4-group);	  
	temp|=Sub&(0x0f>>group);
	temp&=0xf;								//ȡ����λ  
	NVIC->ISER[channel/32]|=(1<<channel%32);//ʹ���ж�λ(Ҫ����Ļ�,�෴������OK) 
	NVIC->IP[channel]|=temp<<4;		//������Ӧ���ȼ����������ȼ� ;
}


void NVIC_exti(u8 GPIOx,u8 BITx,u8 TRIM)//PU ZHONG KAI FA BAN 
{
	u8 addr,offset;
	addr=BITx/4;
	offset=(BITx%4)*4;
	RCC->APB2ENR|=1<<0;//AFIO Enable
//	AFIO->EXTICR[addr]&=~(0x000f<<offset);//Clear previous data
	AFIO->EXTICR[addr]|=GPIOx<<offset;//Reflect GPIOx
	EXTI->IMR|=1<<BITx;
	if(TRIM==0) EXTI->FTSR|=1<<BITx;//UP->DOWN
	if(TRIM==1) EXTI->RTSR|=1<<BITx;//DOWN->UP
}
