#include "kalman.h"
u16 PID_time=0;
void KALMAN_angel(float *Kalman_pit,float *Kalman_rol)
{
	float G_pit_,G_rol_;
	float Pk_pit_,Pk_rol_;
	float Kg_pit,Kg_rol;
	static float Q=1.5,R=2;
	static float Pk_1_pit=0,Pk_1_rol=0;
	float Delta_pit,Delta_rol,Delta_yaw;//delta_pit
	float A_pit,A_rol,A_yaw;
	MPU_accel_angel(&A_pit,&A_rol,&A_yaw);
	//===================================
	//X(k|k-1)=AX(k-1|k-1)+Delta(k)	------------	      (1)
	// A=1 
//	MPU_gyro_angel(&Delta_pit,&Delta_rol,&Delta_yaw);
	MPU_calculus_gyro_angel(&Delta_pit,&Delta_rol,&Delta_yaw);//  Delta(k)
	G_pit_=*Kalman_pit+Delta_pit;//ok
	G_rol_=*Kalman_rol+Delta_rol;//ok
	//===================================
	//P(k|k-1)=AP(k-1|k-1)A'+Q	--------------	(2)
	//Q=
	Pk_pit_=Pk_1_pit+Q*Q;
	Pk_rol_=Pk_1_rol+Q*Q;
	//===================================
	//Kg=P(k|k-1)*H'/(H*P(k|k-1)H'+R)--------------	(3)
	Kg_pit=Pk_pit_/(Pk_pit_+R*R);
	Kg_rol=Pk_rol_/(Pk_rol_+R*R);
	//===================================
	//X(k|k)=X(k|k-1)+Kg*(Zk-HX(k|k-1))--------------	(4)
 	*Kalman_pit=G_pit_+Kg_pit*(A_pit-G_pit_);
	*Kalman_rol=G_rol_+Kg_rol*(A_rol-G_rol_);
	 //===================================
	//P(k|k)=(I-Kg*H)*P(k|k-1)--------------	(5)
	Pk_1_pit=(1-Kg_pit)*Pk_pit_*Pk_pit_;
	Pk_1_rol=(1-Kg_rol)*Pk_rol_*Pk_rol_;
}



void TIM1_ch1_init()
{
	RCC->APB2ENR|=1<<11;
	TIM1->ARR=99;
	TIM1->PSC=71;
	TIM1->DIER=0x01;
	TIM1->CR1=0x01;
	NVIC_priority(3,2,TIM1_UP_IRQn,2);
	
}
void TIM1_UP_IRQHandler(void)
{
	if(TIM1->SR&0X0001)//溢出中断
	{
		PID_time++;	
		PBout(4)=!PBout(4);
	}				   
	TIM1->SR&=~(1<<0);//清除中断标志位 	   
}
//#################################################################
//												PID 单环控制
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void PID_pwm(u16 *pwm1_a,u16 *pwm1_b,u8 *dir1a,u8 *dir1b,
						 u16 *pwm2_a,u16 *pwm2_b,u8 *dir2a,u8 *dir2b)
{ 
	//PID_time=1,  time is 1mm sec
	unsigned char str[10];
	static float  Kp_pit=150,
								Ti_pit=0,
								Td_pit=0,
	
								Kp_roll,
								Ti_roll,
								Td_roll ;//PID三个系数
	//E_*_angel=C_*_angel-R_*_angel;
	//R_*_angel=0,C_*_angel=E_*_angel; 
	float C_pit_angel,C_roll_angel;//这一次的角度
	float pid1a,pid1b,pid2a,pid2b;
//	static float C_pit_angel_,C_roll_angel_;//上一次的角度
	float delta;
	float P_pit,I_pit,D_pit,P_roll,I_roll,D_roll;//PID三个参数
	KALMAN_angel(&C_pit_angel,&C_roll_angel);
	
	// PID ------------------pit
	delta = 0 - C_pit_angel;//Angel_curren - Angel_lasttime  for PID.D
	
//	C_pit_angel_ = C_pit_angel;//update angel_last_time 
	P_pit=Kp_pit*delta;
	I_pit+=(Kp_pit/Ti_pit)*delta*1;
	D_pit=Kp_pit*Td_pit*delta/1;//time period equal the program run once PID_PWM function;
	
	
	// PID -----------------roll
//	delta = C_roll_angel - C_roll_angel_;
//	C_roll_angel_ = C_roll_angel;
	P_roll=Kp_roll*delta;
	I_roll+=Kp_roll/Ti_roll*(delta*PID_time);
	D_roll=Kp_roll*Td_roll*delta/PID_time; 
	//PID parameter to motor_pwm
	pid1a =P_pit+I_pit+D_pit;
	pid1b =P_pit+I_pit+D_pit;
	pid2a =P_roll+I_roll+D_roll;
	pid2b =P_roll+I_roll+D_roll;
//		sprintf(str,"%.2f",pid1a);
//		USART1_send(str);
//		USART1_send("\n");
//		memset(str,0,sizeof(str));
	if(pid1a>=0)		//		pwm1_a
	{
		*dir1a=0;
		*pwm1_a = (u16)pid1a;
		if(*pwm1_a>1000)
			*pwm1_a=999;
	}
	else
	{
		*dir1a=1;
		*pwm1_a = (u16)(-pid1a);
		if(*pwm1_a>1000)
			*pwm1_a=999;
	}
	
		if(pid1b>=0)	//		pwm1_b
	{
		*dir1b=0;
		*pwm1_b = (u16)pid1b;
		if(*pwm1_b>1000)
			*pwm1_b=999;
	}
	else
	{
		*dir1b=1;
		*pwm1_b = (u16)(-pid1b);
		if(*pwm1_b>1000)
			*pwm1_b=999;
	}
	//		pwm2_a
	if(pid2a>=0)		//		pwm2_a
	{
		*dir2a=0;
		*pwm2_a = (u16)pid2a;
		if(*pwm2_a>1000)
			*pwm2_a=999;
	}
	else
	{
		*dir2a=1;
		*pwm2_a = (u16)(-pid2a);
		if(*pwm2_a>1000)
			*pwm2_a=999;
	}
	
		if(pid2b>=0)	//		pwm2_b
	{
		*dir2b=0;
		*pwm2_b = (u16)pid2b;
		if(*pwm2_b>1000)
			*pwm2_b=999;
	}
	else
	{
		*dir2b=1;
		*pwm2_b = (u16)(-pid2b);
		if(*pwm2_b>1000)
			*pwm2_b=999;
	}
	
//	PID_time = 0;

}



                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       

       
