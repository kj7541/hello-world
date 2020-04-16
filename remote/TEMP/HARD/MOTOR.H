#ifndef _motor_h
#define _motor_h
#include "sys.h"

#define short_brake 1
#define stop 2
#define standby 3
//MOTOR GROUP 1
#define PWM1B TIM3->CCR3
#define PWM1A TIM3->CCR4
#define A1_IN1 PEout(7)
#define A1_IN2 PEout(8)
#define B1_IN1 PCout(5)
#define B1_IN2 PCout(4)
#define STBY1 PBout(2)
//MOTRO GROUP 2
#define PWM2B TIM4->CCR1
#define PWM2A TIM4->CCR2
#define A2_IN2 PEout(0)
#define A2_IN1 PBout(8)
#define B2_IN1 PBout(4)
#define B2_IN2 PBout(3)
#define STBY2 PBout(5)

void PWM1_init(u16 arr,u16 psc);
void TB6612FNG_init(void);
void M1(u16 pwma,u8 cwa,u16 pwmb,u8 cwb);
void M1_mode(u8 mode_a,u8 mode_b);

void PWM2_init(u16 arr,u16 psc);
void M2(u16 pwma,u8 cwa,u16 pwmb,u8 cwb);
void M2_mode(u8 mode_a,u8 mode_b);



#endif





