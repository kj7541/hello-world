#include "motor.h"



void TB6612FNG_init(void)
{
	//MOTOR GROUP 1
	RCC->APB2ENR|=0x58;//PE PC PB
	GPIOE->CRH&=0xfffffff0;//PE8
	GPIOE->CRH|=0x00000003;
	GPIOE->CRL&=0x0fffffff;//PE7
	GPIOE->CRL|=0x30000000;
	GPIOB->CRL&=0xfffff0ff;//PB2
	GPIOB->CRL|=0x00000300;
	GPIOC->CRL&=0xff00ffff;//PC5 PC4
	GPIOC->CRL|=0x00330000;
	//MOTOR GROUP 2
	RCC->APB2ENR|=1<<6;//PE
	RCC->APB2ENR|=1<<3;//PB
	GPIOE->CRL&=0xfffffff0;//PE0
	GPIOE->CRL|=0x00000003;
	GPIOB->CRL&=0xff000fff;//PB3 PB4 PB5
	GPIOB->CRL|=0x00333000;
	GPIOB->CRH&=0xfffffff0;//PB8
	GPIOB->CRH|=0x00000003;
}
/*****************MOTOR GROUP 1******************/
/*****************MOTOR GROUP 1******************/
/*****************MOTOR GROUP 1******************/
//TIM3-3,4
void PWM1_init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;//Enable TIM3
	RCC->APB2ENR|=1<<3;//PB
	GPIOB->CRL&=0xffffff00;
	GPIOB->CRL|=0x000000bb;
	TIM3->ARR=arr;
	TIM3->PSC=psc;
	TIM3->CCMR2|=0x7070;//OC4 OC3  PWM_mode1
	TIM3->CCMR2|=0x0808;//Output3,4 compare 3 preload enable
	TIM3->CR1|=1<<7;//Enable ARPE
	TIM3->EGR|=1<<0;
	TIM3->CCER|=0x3300;//OC3,4 enable
	TIM3->CR1|=1<<0;//Enabl TIM3
}

void M1(u16 pwma,u8 cwa,u16 pwmb,u8 cwb)
{
	//////MOTOR A
	if(cwa)//Motor run clock wise
	{
		A1_IN1=0;
		A1_IN2=1;
		STBY1=1;
	}
	else
	{
		A1_IN1=1;
		A1_IN2=0;
		STBY1=1;
	}
	PWM1A=pwma;
	///////MOTOR B
	if(cwb)//Motor run clock wise
	{
		B1_IN1=0;
		B1_IN2=1;
		STBY1=1;
	}
	else
	{
		B1_IN1=1;
		B1_IN2=0;
		STBY1=1;
	}
	PWM1B=pwmb;
}
void M1_mode(u8 mode_a,u8 mode_b)
{
	STBY1=1;
	///////////////MOTOR1 A
	if(mode_a==1)//Short brake
	{
		A1_IN1=1;
		A1_IN2=1;
	}
	if(mode_a==2)//Stop
	{
		A1_IN1=0;
		A1_IN2=0;
	}
	if(mode_a==3) STBY1=0;//Standby
	///////////MOTOR1 B
			if(mode_b==1)//Short brake
	{
		B1_IN1=1;
		B1_IN2=1;
	}
	if(mode_b==2)//Stop
	{
		B1_IN1=0;
		B1_IN2=0;
	}
	if(mode_b==3) STBY1=0;//Stadnby
		
}

/******************MOTOR GROUP 2******************/
/******************MOTOR GROUP 2******************/
/******************MOTOR GROUP 2******************/
//TIM4-1,2
void PWM2_init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<2;//Enable TIM4
	RCC->APB2ENR|=1<<3;//PB
	GPIOB->CRL&=0x00ffffff;
	GPIOB->CRL|=0xbb000000;
	TIM4->ARR=arr;
	TIM4->PSC=psc;
	TIM4->CCMR1|=0x7070;//OC2 OC1  PWM_mode1
	TIM4->CCMR1|=0x0808;//Output1,2 compare 3 preload enable
	TIM4->CR1|=1<<7;//Enable ARPE
	TIM4->EGR|=1<<0;
	TIM4->CCER|=0x0033;//OC1,2 enable
	TIM4->CR1|=1<<0;//Enabl TIM4
}

void M2(u16 pwma,u8 cwa,u16 pwmb,u8 cwb)
{
	STBY2=1;
	//////MOTOR A
	if(cwa)//Motor run clock wise
	{
		A2_IN1=0;
		A2_IN2=1;
	}
	else
	{
		A2_IN1=1;
		A2_IN2=0;
	}
	PWM2A=pwma;
	///////MOTOR B
	if(cwb)//Motor run clock wise
	{
		B2_IN1=0;
		B2_IN2=1;
	}
	else
	{
		B2_IN1=1;
		B2_IN2=0;
	}
	PWM2B=pwmb;
}
void M2_mode(u8 mode_a,u8 mode_b)
{
	///////////////MOTOR1 A
	if(mode_a==1)//Short brake
	{
		A2_IN1=1;
		A2_IN2=1;
		STBY2=1;
	}
	if(mode_a==2)//Stop
	{
		A2_IN1=0;
		A2_IN2=0;
		STBY2=1;
	}
	if(mode_a==3) STBY2=0;//Standby
	///////////MOTOR1 B
			if(mode_b==1)//Short brake
	{
		B2_IN1=1;
		B2_IN2=1;
		STBY2=1;
	}
	if(mode_b==2)//Stop
	{
		B2_IN1=0;
		B2_IN2=0;
		STBY2=1;
	}
	if(mode_b==3) STBY2=0;//Stand by
		
}



void VOLTAGE_check_init(void)
{
	RCC->APB2ENR|=1<<4;
	GPIOC->CRL&=0xfffffff0;
	RCC->APB2ENR|=1<<9;
	RCC->APB2RSTR|=1<<9;
	RCC->APB2RSTR&=~(1<<9);
	RCC->CFGR&=~(3<<14);
	RCC->CFGR|=2<<14;
	
}
//void VOLTAGE_check(void)
//{
//	
//}













