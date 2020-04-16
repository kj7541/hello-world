#include "key.h"

//PU ZHONG KAI FA BAN

void key_init(void)
{
	RCC->APB2ENR|=1<<2;
	RCC->APB2ENR|=1<<3;
	RCC->APB2ENR|=1<<4;
	GPIOA->CRL&=0xfffff000;
	GPIOA->CRL|=0x00000888;//k0,k1,k2
	GPIOB->CRL&=0xffffff0f;
	GPIOB->CRL|=0x00000080;//sw2
	GPIOC->CRL&=0xffff0fff;
	GPIOC->CRL|=0x00008000;//sw1
}

u8 key_scan(void)
{
	u8 flag=0;
	if(k2==0)
	{
		delay_ms(50);
		if(k2==0)
		{
			 flag=1;
		}
	}
	if(k0==0)
	{
		delay_ms(50);
		if(k0==0)
		{
			flag=1;
		}
	}
		if(k1==0)
	{
		delay_ms(50);
		if(k1==0)
		{
			flag=1;
		}
	}
		if(sw1==0)
	{
		delay_ms(50);
		if(sw1==0)
		{
			 flag=1;
		}
	}
		if(sw2==0)
	{
		delay_ms(50);
		if(sw2==0)
		{
			flag=1;
		}
	}
	return flag; 
}

