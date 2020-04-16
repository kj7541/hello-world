#ifndef _kalman_h
#define _kalman_h
#include "MPU6050.h" 
#include "sys.h"
#include "motor.h"
#include "USART.h"
#include "stdio.h"
void KALMAN_angel(float *Kalman_pit,float *Kalman_rol);
void TIM1_ch1_init();
void PID_pwm(u16 *pwm1_a,u16 *pwm1_b,u8 *dir1a,u8 *dir1b,u16 *pwm2_a,u16 *pwm2_b,u8 *dir2a,u8 *dir2b);


#endif





