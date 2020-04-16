#include "MPU.h"

//void MPU_iic_init(void)
//{
//	RCC->APB2ENR|=3<<4;//PC PD
//	GPIOC->CRL&=0x00ffffff;//PC7 PC6
//	GPIOC->CRL|=0x33000000;
//	GPIOD->CRH&=0x00ffffff;//PD15 PD14
//	GPIOD->CRH|=0x33000000;
//}

void Delay5us()
{
	delay_us(5);
}

void I2C_Start()
{
	MPU_SDA_OUT();
	SDA = 1;                    //拉高数据线
	SCL = 1;                    //拉高时钟线
	Delay5us();                 //延时
	SDA = 0;                    //产生下降沿
	Delay5us();                 //延时
	SCL = 0;                    //拉低时钟线
}

void I2C_Stop()
{
	MPU_SDA_OUT();
    SDA = 0;                    //拉低数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 1;                    //产生上升沿
    Delay5us();                 //延时
}

void I2C_SendACK(u8 ack)
{
		MPU_SDA_OUT();
    SDA = ack;                  //写应答信号
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
}
u8 I2C_RecvACK()
{
	u8 CY;
	MPU_SDA_IN();
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    CY = sda;                   //读应答信号
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
    return CY;
}

void I2C_SendByte(u8 dat)
{
    u8 i;
    for (i=0; i<8; i++)         //8位计数器
    {
			SDA=((dat&0x80)>>7);
        dat <<= 1;              //移出数据的最高位
                      //送数据口
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    I2C_RecvACK();
}
u8 I2C_RecvByte()
{
    u8 i;
    u8 dat = 0;
    SDA = 1;                    //使能内部上拉,准备读取数据,
		MPU_SDA_IN();
	for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        dat |= sda;             //读数据               
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    return dat;
}

void Single_WriteI2C(u8 REG_Address,u8 REG_data)
{
    I2C_Start();                  //起始信号
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
    I2C_SendByte(REG_Address);    //内部寄存器地址，
    I2C_SendByte(REG_data);       //内部寄存器数据，
    I2C_Stop();                   //发送停止信号
}
u8 Single_ReadI2C(u8 REG_Address)
{
	u8 REG_data;
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress);    //发送设备地址+写信号
	I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始	
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号
	REG_data=I2C_RecvByte();       //读出寄存器数据
	I2C_SendACK(1);                //接收应答信号
	I2C_Stop();                    //停止信号
	return REG_data;
}

void InitMPU6050()
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG, 0x06);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x01);
}

u16 GetData(u8 REG_Address)
{
	u8 H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return (H<<8)+L;   //合成数据
}
//void display_temp()
//{ 
//	Temp_h=Single_ReadI2C(TEMP_OUT_H); //读取温度
//	Temp_l=Single_ReadI2C(TEMP_OUT_L); //读取温度
//	Temperature=Temp_h<<8|Temp_l;     //合成温度
//	Temperature = 35+ ((double) (Temperature + 13200)) / 280; // 计算出温度
//	lcd_printf(dis,Temperature);     //转换数据显示
//	DisplayListChar(11,1,dis,4);     //启始列，行，显示数组，显示位数
//}
