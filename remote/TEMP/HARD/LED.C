#include "led.h"


void led_init()
{
	RCC->APB2ENR|=3<<2;
	GPIOA->CRL&=0xffff00ff;
	GPIOA->CRL|=0x00003300;
	GPIOB->CRH&=0xffffff0f;
	GPIOB->CRH|=0x00000030;
}

