#include "MPU6050.h"

//@MPU IIC FUNCTION
//                  MPU IIC FUNCTION
//@MPU IIC FUNCTION
 u16 TIME_GYRO=0;
void MPU_iic_init(void)
{
	RCC->APB2ENR|=3<<4;//PC PD
	GPIOC->CRL&=0x00ffffff;//PC7 PC6
	GPIOC->CRL|=0x33000000;
	GPIOD->CRH&=0x00ffffff;//PD15 PD14
	GPIOD->CRH|=0x33000000;
}

void MPU_sda_in(void)
{
	GPIOC->CRL&=0xf0ffffff;
	GPIOC->CRL|=0x08000000;
}

void MPU_sda_out(void)
{
	GPIOC->CRL&=0xf0ffffff;
	GPIOC->CRL|=0x03000000;
}

void MPU_iic_start(void)
{
	MPU_sda_out();
	MPU_scl=1;
	MPU_sda=1;
//	delay_us(2);
	MPU_sda=0;
}
void MPU_iic_stop(void)
{
		MPU_sda_out();
		MPU_sda=0;
		MPU_scl=1;
	//	delay_us(2);
		MPU_sda=1;
}
//1->ACK; 0->NACK
u8 MPU_iic_wait_ack(void)
{
	u8 ErrTime=0;
		MPU_sda=1;
	MPU_sda_in();
	MPU_scl=1;
	delay_us(2);
	while(MPU_read_sda)
	{
		ErrTime++;
		if(ErrTime>250)
		{
			MPU_iic_stop();
			return 0;
		}
	}
	MPU_scl=0;
	return 1;
}
//1-> Generate ACK; 0-> Generate NACK
void MPU_ack(u8 ack)
{
	MPU_scl=0;
	MPU_sda_out();
	MPU_sda=!ack;
	delay_us(2);
	MPU_scl=1;
	delay_us(2);
	MPU_scl=0;
}
void MPU_iic_send_one_byte(u8 byte)
{
	u8 t;
	MPU_scl=0;
	MPU_sda_out();
	MPU_sda=0;
	for(t=0;t<8;t++)
	{
		MPU_sda=(byte&0x80)>>7;;
		byte<<=1;
//		delay_us(2);
		MPU_scl=1;;;
		MPU_scl=0;
//		delay_us(1);
		MPU_sda=0;;
	}
}
//
//
//
u8 MPU_iic_read_one_byte(u8 ack)
{
	u8 receive=0,t;
	MPU_scl=0;
	MPU_sda_in();
	for(t=0;t<8;t++)
	{
		MPU_scl=0;
		receive<<=1;
		delay_us(2);
		MPU_scl=1;
		receive+=MPU_read_sda;
	}
	MPU_ack(ack);
	return receive;
}
//
//u8 MPU_read_len(u8 reg,u8 len,u8 *buf)
//
u8 MPU_read_len(u8 reg,u8 len,u8 *buf)
{
	MPU_iic_start();
	MPU_iic_send_one_byte((MPU_ADDR<<1)|0);
	if(!MPU_iic_wait_ack())
	{
		MPU_iic_stop();
		return 0;
	}
	MPU_iic_send_one_byte(reg);
	if(!MPU_iic_wait_ack())
	{
		MPU_iic_stop();
		return 0;
	}
	MPU_iic_start();
	MPU_iic_send_one_byte((MPU_ADDR<<1)|1);
	if(!MPU_iic_wait_ack())
	{
		MPU_iic_stop();
		return 0;
	}
	while(len)
	{
		if(len==1) *buf=MPU_iic_read_one_byte(0);
			else *buf=MPU_iic_read_one_byte(1);
		len--;
		buf++;
	}
	MPU_iic_stop();
	return 1;
}
	

// return
// 0->iic fail; 1->iic success;
u8 MPU_write_byte(u8 reg,u8 byte)
{
	MPU_iic_start();
	MPU_iic_send_one_byte((MPU_ADDR<<1)|0);//MPU address & (0) write
	if(!MPU_iic_wait_ack())
	{
		MPU_iic_stop();
		return 0;
	}
	MPU_iic_send_one_byte(reg);
	if(!MPU_iic_wait_ack())
		{
			MPU_iic_stop();
			return 0;
		}
	MPU_iic_send_one_byte(byte);
	if(!MPU_iic_wait_ack())
	{
		MPU_iic_stop();
		return 0;
	}
	MPU_iic_stop();
	return 1;
}


u8 MPU_write_len(u8 reg,u8 len,u8 *buf)
{
	u8 i;
	MPU_iic_start();
	MPU_iic_send_one_byte((MPU_ADDR<<1)|0);
	if(!MPU_iic_wait_ack())
		{
			MPU_iic_stop();
			return 0;
		}
	MPU_iic_send_one_byte(reg);
	if(!MPU_iic_wait_ack())
		{
			MPU_iic_stop();
			return 0;
		}
		for(i=0;i<len;i++)
		{
			MPU_iic_send_one_byte(buf[i]);
			if(!MPU_iic_wait_ack())
				{
					MPU_iic_stop();
					return 0;
				}
		}
		MPU_iic_stop();
		return 1;
}
//Return 
//0->iic fail; ELSE->success;
u8 MPU_read_byte(u8 reg)
{
	u8 data;
	MPU_iic_start();
	MPU_iic_send_one_byte((MPU_ADDR<<1)|0);
	if(!MPU_iic_wait_ack())
	{
		MPU_iic_stop();
		return 0;
	}
	MPU_iic_send_one_byte(reg);
	if(!MPU_iic_wait_ack())
	{
		MPU_iic_stop();
		return 0;
	}
	MPU_iic_start();
	MPU_iic_send_one_byte((MPU_ADDR<<1)|1);
	if(!MPU_iic_wait_ack())
	{
		MPU_iic_stop();
		return 0;
	}
	data=MPU_iic_read_one_byte(0);//Read data and generate NACK signal	
	MPU_iic_stop();
	return data;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//                     MPU OPERATE FUNCTION
//#####################################################




//Return 
//ELSE->init success; 0->init fail;
u8 MPU_init(void)
{
	u8 MPU_adr;
	MPU_iic_init();
	MPU_write_byte(MPU_PWR_MGMT1_REG,0x80);//Resert MPU6050
	delay_ms(100);
	MPU_write_byte(MPU_PWR_MGMT1_REG,0x00);//Wake up MPU6050
	
	
  MPU_set_gyro(3);//+-2000
	MPU_set_accel(0);//+-2g
	MPU_set_sample_rate(100);
	MPU_write_byte(MPU_INT_EN_REG,0x00);//Disable all interrupt
	MPU_write_byte(MPU_USER_CTRL_REG,0X00);//Close IIC master mode
	MPU_write_byte(MPU_FIFO_EN_REG,0X00);//Close FIFO
	MPU_write_byte(MPU_INTBP_CFG_REG,0X80);//Int pin L level  valid
	
	MPU_adr=MPU_read_byte(MPU_DEVICE_ID_REG);
	if(MPU_adr==0x68)
		{
			MPU_write_byte(MPU_PWR_MGMT1_REG,0x01);//
			MPU_write_byte(MPU_PWR_MGMT2_REG,0x00);
			return MPU_adr;
		}
		else
			return 0;
//	return MPU_adr;
}


//Config MPU gyro
//scal->0:+-250; 1:+-500; 2:+-1000; 3:+-2000; 
//return 1->success; 0->fail;
u8 MPU_set_gyro(u8 scal)
{
	 return MPU_write_byte(MPU_GYRO_CFG_REG,scal<<3);
}


//Config MPU accel
//scal->0:+-2g; 1:+-4g; 2:+-8g; 3:+-16g; 
//return 1->success; 0->fail;
u8 MPU_set_accel(u8 scal)
{
	return MPU_write_byte(MPU_ACCEL_CFG_REG,scal<<3);
}


//Config MPU sample rate
u8 MPU_set_dlpf(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_write_byte(MPU_CFG_REG,data);
}
//MPU sample rate set
//rate:4~1000(Hz)
//return 1->success; 0->fail;
u8 MPU_set_sample_rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_write_byte(MPU_SAMPLE_RATE_REG,data);
	return MPU_set_dlpf(rate/2);
}
//MPU GET tempperature
u16 MPU_temperature(void)
{
	u16 temp;
	temp=MPU_read_byte(MPU_TEMP_OUTH_REG);
	temp=(temp<<8)|MPU_read_byte(MPU_TEMP_OUTL_REG);
	return temp;
}



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//      MPU GET ACCELX,Y,Z & MPU GET GYROX,Y,Z
//###########################################################
//Get gyroX,gyroY,gyroZ;
//return 1:OK, 0:fail
//u8 MPU_gyro(short *gx,short *gy,short *gz)
//{
//	u8 buf[6],flag;
//	flag=MPU_read_len(MPU_GYRO_XOUTH_REG,6,buf);
//	if(flag)
//	{
//		*gx=((u16)buf[0]<<8)|buf[1];  
//		*gy=((u16)buf[2]<<8)|buf[3];  
//		*gz=((u16)buf[4]<<8)|buf[5];
//	}
//	return flag;
//}
u8 MPU_gyro(short *gx,short *gy,short *gz)
{
	u16 temp=0;
	//GYRO X
	temp=MPU_read_byte(MPU_GYRO_XOUTH_REG);
	temp=(temp<<8)|MPU_read_byte(MPU_GYRO_XOUTL_REG);
	*gx=temp;
	//GYRO Y
	temp=0;
	temp=MPU_read_byte(MPU_GYRO_YOUTH_REG);
	temp=(temp<<8)|MPU_read_byte(MPU_GYRO_YOUTL_REG);
	*gy=temp;
	//GYRO Z
	temp=0;
	temp=MPU_read_byte(MPU_GYRO_ZOUTH_REG);
	temp=(temp<<8)|MPU_read_byte(MPU_GYRO_ZOUTL_REG);
	*gz=temp;
	return 0;
}
//Get accelX,accelY,accelZ;
//return 1:OK, 0:fail
//u8 MPU_accel(short *ax,short *ay,short *az)
//{
//	u8 buf[6],flag;
//	flag=MPU_read_len(MPU_ACCEL_XOUTH_REG,6,buf);
//	if(flag)
//	{
//		*ax=((u16)buf[0]<<8)|buf[1];  
//		*ay=((u16)buf[2]<<8)|buf[3];  
//		*az=((u16)buf[4]<<8)|buf[5];
//	}
//	return flag;
//}
u8 MPU_accel(short *ax,short *ay,short *az)
{
	u16 temp=0;
	//ACCEL X
	temp=MPU_read_byte(MPU_ACCEL_XOUTH_REG);
	temp=(temp<<8)|MPU_read_byte(MPU_ACCEL_XOUTL_REG);
	*ax=temp;
	//ACCEL Y
	temp=0;
	temp=MPU_read_byte(MPU_ACCEL_YOUTH_REG);
	temp=(temp<<8)|MPU_read_byte(MPU_ACCEL_YOUTL_REG);
	*ay=temp;
	//ACCEL Z
	temp=0;
	temp=MPU_read_byte(MPU_ACCEL_ZOUTH_REG);
	temp=(temp<<8)|MPU_read_byte(MPU_ACCEL_ZOUTL_REG);
	*az=temp;
	return 0;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//                      MPU GET ACCELX,Y,Z
//###########################################################
u16 MPU_accel_X(void)
{
	u16 X;
	X=MPU_read_byte(MPU_ACCEL_XOUTH_REG);
	X=(X<<8)|MPU_read_byte(MPU_ACCEL_XOUTL_REG);
	return X;
}

//Get MPU accel Y axis
u16 MPU_accel_Y(void)
{
	u16 Y;
	Y=MPU_read_byte(MPU_ACCEL_YOUTH_REG);
	Y<<=8;
	Y|=MPU_read_byte(MPU_ACCEL_YOUTL_REG);
//	Y=(Y<<8)|MPU_read_byte(MPU_ACCEL_YOUTL_REG);
	return Y;
}
//Get MPU accel Z axis
u16 MPU_accel_Z(void)
{
	u16 Z;
	Z=MPU_read_byte(MPU_ACCEL_ZOUTH_REG);
	Z=(Z<<8)|MPU_read_byte(MPU_ACCEL_ZOUTL_REG);
	return Z;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//               MPU GET GYROX,Y,Z
//###########################################################
u16 MPU_gyro_X(void)
{
	u16 X;
	X=MPU_read_byte(MPU_GYRO_XOUTH_REG);
	X=(X<<8)|MPU_read_byte(MPU_GYRO_XOUTL_REG);
	return X;

}
u16 MPU_gyro_Y(void)
{
	u16 Y;
	Y=MPU_read_byte(MPU_GYRO_YOUTH_REG);
	Y=(Y<<8)|MPU_read_byte(MPU_GYRO_YOUTL_REG);
	return Y;
}
u16 MPU_gyro_Z(void)
{
	u16 Z;
	Z=MPU_read_byte(MPU_GYRO_ZOUTH_REG);
	Z=(Z<<8)|MPU_read_byte(MPU_GYRO_ZOUTL_REG);
	return Z;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//               MPU GET  PITCH ROLL YAW
//###########################################################
//1 ACCEL_PITCH ROLL YAW
void MPU_accel_angel(float *pit,float *rol,float *yaw)
{
	u8 k;
	float temp,temp1;
	static float Ay_offset,Ax_offset;
	static u8 i=1;
	if(i==1)
	{
		i=0;
			temp=0;
			for(k=0;k<10;k++)
			{
				temp+=MPU_accel_Y();
			}
			temp=temp/10.0/16384.0;
			if(temp>2) 
			{
				temp-=4;
			}
			temp1=0;
			for(k=0;k<10;k++)
			{
				temp1+=MPU_accel_Z();
			}
			temp1=temp1/10.0/16384.0;
			temp=atan(temp/temp1);
			Ay_offset=temp*180/3.14;
			temp=0;
			for(k=0;k<10;k++)
			{
				temp+=MPU_accel_X();
			}
			temp=temp/10.0/16384.0;
			if(temp>2) 
			{
				temp-=4;
			}
			temp1=0;
			for(k=0;k<10;k++)
			{
				temp1+=MPU_accel_Z();
			}
			temp1=temp1/10.0/16384.0;
			temp=atan(temp/temp1);
			Ax_offset=temp*180/3.14;	
	}
	temp1=MPU_accel_Z();
	temp1=temp1/16384.0;
	
	
	//PIT MPU6050-Y
	//    if FCS forerake angel>0
	//     	 FCS contrary angel<0;

	temp=MPU_accel_Y();
		temp/=16384.0;
		if(temp>2) 
		{
			temp-=4;
		}
		temp=atan(temp/temp1);
		*pit=temp*180/3.14-Ay_offset;
	
	//ROLL    MPU6050_X 
	//    if FCS left     angel<0
	//     	 FCS contrary angel>0	
		temp=MPU_accel_X();
		temp/=16384.0;
		if(temp>2) 
		{
			temp-=4;
		}
		temp=atan(temp/temp1);
		*rol=-1*(temp*180/3.14-Ax_offset);
		
	//YAW  cann't test by accel 
	*yaw=0;

	
}







//2 GYRO PITCH ROLL YAW
//2.1 TIM5_CH2

void TIM5_Ch2_Init(void)
{
	RCC->APB1ENR|=1<<3;
	TIM5->PSC=71;//Rate=72MHz/72=1000Khz;
//	TIM5->ARR=4;//4+1    ->5us
		TIM5->ARR=49999;//4+1    ->5us

	TIM5->DIER|=1<<0;//Update Interrupt proved
	TIM5->CR1|=1<<0;
	NVIC_priority(3,2,TIM5_IRQn,2);     
}
void TIM5_IRQHandler(void)
{
	if(TIM5->SR&0X0001)//溢出中断
	{
		TIME_GYRO++;
	}				   
	TIM5->SR&=~(1<<0);//清除中断标志位 	    
}

void MPU_gyro_offset(float *Gx_offset,float *Gy_offset,float *Gz_offset)
{
	short Gx,Gy,Gz;
	float gx_offset,gy_offset,gz_offset;
	float temp;
	u8 count;
	for(count=0;count<60;count++)
		{
			delay_ms(10);
			MPU_gyro(&Gx,&Gy,&Gz);//Get Gx,Gy,Gz value from register MPU6050
			if(count>9)// drop 0-10 value, they are  not stable,
			{
				temp=Gx;
				gx_offset+=temp;
			}
			
			if(count>9)// drop 0-10 value, they are  not stable,
			{ 
				temp=Gy;
				gy_offset+=temp;
			}
			if(count>9)// drop 0-10 value, they are  not stable,
			{
				temp=Gz;
				gz_offset+=temp;
			}
		}
		gx_offset/=50;//Average gx_offset
		gy_offset/=50;
		gz_offset/=50;
		
		gx_offset*=-1.0;//Contrary value
		gy_offset*=-1.0;
		gz_offset*=-1.0;		
		*Gx_offset=gx_offset;
		*Gy_offset=gy_offset;
		*Gz_offset=gz_offset;
		TIME_GYRO=0;	
}





















void MPU_gyro_angel(float *pit,float *rol,float *yaw)
{	
	short Gx,Gy,Gz;
	float temp,count;
	static u8 i=1;
	static float gx_offset,gy_offset,gz_offset;
//	u8 str[10];
	if(i)//Revise gyro_x,gyro_y,gyro_z
	{
		for(count=0;count<60;count++)
		{
			delay_ms(10);
			MPU_gyro(&Gx,&Gy,&Gz);//Get Gx,Gy,Gz value from register MPU6050
			if(count>9)// drop 0-10 value, they are  not stable,
			{
				temp=Gx;
				gx_offset+=temp;
			}
			
			if(count>9)// drop 0-10 value, they are  not stable,
			{ 
				temp=Gy;
				gy_offset+=temp;
			}
			if(count>9)// drop 0-10 value, they are  not stable,
			{
				temp=Gz;
				gz_offset+=temp;
			}
		}
		gx_offset/=50;//Average gx_offset
		gy_offset/=50;
		gz_offset/=50;
		
		gx_offset*=-1.0;//Contrary value
		gy_offset*=-1.0;
		gz_offset*=-1.0;		

		i=0;	
		TIME_GYRO=0;		
	}
	if(!i)//Gyro angel
	{
		MPU_gyro(&Gx,&Gy,&Gz);
			//PIT
			//  MPU forerake    angel>0;
			//  else            angel>0;	
			//		OLED_string_value(0,0,temp);
			temp=(Gx+gx_offset)/16.4;
			if((-0.1<temp)&&(temp<0.1))
			{temp=0;}
//		OLED_string_value(0,0,temp);
			*pit=*pit+TIME_GYRO*temp*(TIM5->ARR+1)/1000000;
		
			//ROl
			//  MPU left   angel>0; 
			//  else   right       angel<0;/angel>180
			temp=(Gy+gy_offset)/16.4;
				if((-0.1<temp)&&(temp<0.1))
			{temp=0;}
	//	OLED_string_value(0,1,temp);	
			*rol=*rol+TIME_GYRO*temp*(TIM5->ARR+1)/1000000;
			
			//YAW
			//  MPU left   angel>0;
			//  else   right       angel<0;/angel>180
			
			temp=(Gz+gz_offset)/16.4;
				if((-0.1<temp)&&(temp<0.1 ))
			{temp=0;}
	//	OLED_string_value(0,2,temp);	
			*yaw=*yaw+TIME_GYRO*temp*(TIM5->ARR+1)/1000000;



	TIME_GYRO=0;
	}	
}


 void MPU_calculus_gyro_angel(float *pit,float *rol,float *yaw)
{
	//加补偿
	
		short Gx,Gy,Gz;
	float temp,count;
	static u8 i=1;
	static float gx_offset,gy_offset,gz_offset;
//	u8 str[10];
	if(i)//Revise gyro_x,gyro_y,gyro_z
	{
		for(count=0;count<60;count++)
		{
			delay_ms(10);
			MPU_gyro(&Gx,&Gy,&Gz);//Get Gx,Gy,Gz value from register MPU6050
			if(count>9)// drop 0-10 value, they are  not stable,
			{
				temp=Gx;
				gx_offset+=temp;
			}
			
			if(count>9)// drop 0-10 value, they are  not stable,
			{ 
				temp=Gy;
				gy_offset+=temp;
			}
			if(count>9)// drop 0-10 value, they are  not stable,
			{
				temp=Gz;
				gz_offset+=temp;
			}
		}
		gx_offset/=50;//Average gx_offset
		gy_offset/=50;
		gz_offset/=50;
		
		gx_offset*=-1.0;//Contrary value
		gy_offset*=-1.0;
		gz_offset*=-1.0;		

		i=0;	
		TIME_GYRO=0;		
	}
	if(!i)//Gyro angel
	{
		MPU_gyro(&Gx,&Gy,&Gz);
			//PIT
			//  MPU forerake    angel>0;
			//  else            angel>0;	
			//		OLED_string_value(0,0,temp);
			temp=(Gx+gx_offset)/16.4;
			if((-0.1<temp)&&(temp<0.1))
			{temp=0;}
//		OLED_string_value(0,0,temp);
			*pit=TIME_GYRO*temp*(TIM5->ARR+1)/1000000;
		
			//ROl
			//  MPU left   angel>0; 
			//  else   right       angel<0;/angel>180
			temp=(Gy+gy_offset)/16.4;
				if((-0.1<temp)&&(temp<0.1))
			{temp=0;}
	//	OLED_string_value(0,1,temp);	
			*rol=TIME_GYRO*temp*(TIM5->ARR+1)/1000000;
			
			//YAW
			//  MPU left   angel>0;
			//  else   right       angel<0;/angel>180
			
			temp=(Gz+gz_offset)/16.4;
				if((-0.1<temp)&&(temp<0.1 ))
			{temp=0;}
	//	OLED_string_value(0,2,temp);	
			*yaw=TIME_GYRO*temp*(TIM5->ARR+1)/1000000;



	TIME_GYRO=0;
	}
}




