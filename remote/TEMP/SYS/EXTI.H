#ifndef _exti_h
#define _exti_h
#include "sys.h"
#include "delay.h"
#include "key.h"
#include "led.h"
#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6
#define UP 1
#define DOWN 0

void NVIC_group(u8 group);
void NVIC_priority(u8 Pre,u8 Sub,u8 channel,u8 group);
void NVIC_exti(u8 GPIOx,u8 BITx,u8 TRIM);//PU ZHONG KAI FA BAN 



#endif 

