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
	SDA = 1;                    //����������
	SCL = 1;                    //����ʱ����
	Delay5us();                 //��ʱ
	SDA = 0;                    //�����½���
	Delay5us();                 //��ʱ
	SCL = 0;                    //����ʱ����
}

void I2C_Stop()
{
	MPU_SDA_OUT();
    SDA = 0;                    //����������
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SDA = 1;                    //����������
    Delay5us();                 //��ʱ
}

void I2C_SendACK(u8 ack)
{
		MPU_SDA_OUT();
    SDA = ack;                  //дӦ���ź�
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ
}
u8 I2C_RecvACK()
{
	u8 CY;
	MPU_SDA_IN();
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    CY = sda;                   //��Ӧ���ź�
    SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ
    return CY;
}

void I2C_SendByte(u8 dat)
{
    u8 i;
    for (i=0; i<8; i++)         //8λ������
    {
			SDA=((dat&0x80)>>7);
        dat <<= 1;              //�Ƴ����ݵ����λ
                      //�����ݿ�
        SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    I2C_RecvACK();
}
u8 I2C_RecvByte()
{
    u8 i;
    u8 dat = 0;
    SDA = 1;                    //ʹ���ڲ�����,׼����ȡ����,
		MPU_SDA_IN();
	for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        dat |= sda;             //������               
        SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    return dat;
}

void Single_WriteI2C(u8 REG_Address,u8 REG_data)
{
    I2C_Start();                  //��ʼ�ź�
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
    I2C_SendByte(REG_data);       //�ڲ��Ĵ������ݣ�
    I2C_Stop();                   //����ֹͣ�ź�
}
u8 Single_ReadI2C(u8 REG_Address)
{
	u8 REG_data;
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress);    //�����豸��ַ+д�ź�
	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ����0��ʼ	
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress+1);  //�����豸��ַ+���ź�
	REG_data=I2C_RecvByte();       //�����Ĵ�������
	I2C_SendACK(1);                //����Ӧ���ź�
	I2C_Stop();                    //ֹͣ�ź�
	return REG_data;
}

void InitMPU6050()
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//�������״̬
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
	return (H<<8)+L;   //�ϳ�����
}
//void display_temp()
//{ 
//	Temp_h=Single_ReadI2C(TEMP_OUT_H); //��ȡ�¶�
//	Temp_l=Single_ReadI2C(TEMP_OUT_L); //��ȡ�¶�
//	Temperature=Temp_h<<8|Temp_l;     //�ϳ��¶�
//	Temperature = 35+ ((double) (Temperature + 13200)) / 280; // ������¶�
//	lcd_printf(dis,Temperature);     //ת��������ʾ
//	DisplayListChar(11,1,dis,4);     //��ʼ�У��У���ʾ���飬��ʾλ��
//}