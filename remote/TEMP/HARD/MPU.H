#ifndef _MPU_H
#define _MPU_H
#include "sys.h"
#include "delay.h"


//****************************************
// 定义MPU6050内部地址
//****************************************


#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I			0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0x68	//IIC写入时的地址字节数据，+1为读取



///////////////////////////////////////////
#define SDA PCout(6)
#define SCL PDout(15)
#define sda PCin(6)
#define MPU_SDA_IN()  {GPIOC->CRL&=0XF0FFFFFF;GPIOC->CRL|=0x08000000;}
#define MPU_SDA_OUT() {GPIOC->CRL&=0XF0FFFFFF;GPIOC->CRL|=0x03000000;}




//MPU6050操作函数

//void MPU_iic_init(void);




void  InitMPU6050();													//初始化MPU6050
void  Delay5us();
void  I2C_Start();
void  I2C_Stop();
void  I2C_SendACK(u8 ack);
u8   I2C_RecvACK();
void  I2C_SendByte(u8 dat);
u8 I2C_RecvByte();
void  I2C_ReadPage();
void  I2C_WritePage();
void  display_ACCEL_x();
void  display_ACCEL_y();
void  display_ACCEL_z();
u8 Single_ReadI2C(u8 REG_Address);						//读取I2C数据
void  Single_WriteI2C(u8 REG_Address,u8 REG_data);

u16 GetData(u8 REG_Address);






#endif





