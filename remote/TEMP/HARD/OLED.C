///////////////////////////////////////////////////
//  THE OLED.c for 4 line IIC mode
//  THE OLED.c for stm32
//  Left to right
//  PIN:1,VCC.2,GND.3,SCL.4,SDA
//  VERSION:V1.0.2
//  Show right value range FLOAT any
//  Function the coordinate Y (0-3)
///////////////////////////////////////////////////
#include "oled.h"
#include "font.h"
void OLED_iic_sda_out(void)
{
	GPIOD->CRL&=0xffff0fff;
	GPIOD->CRL|=0x00003000;
}
void OLED_iic_sda_in(void)
{
	GPIOD->CRL&=0xffff0fff;
	GPIOD->CRL|=0x00008000;
}
void OLED_iic_start(void)
{
	OLED_iic_scl=1;
	OLED_iic_sda=1;
	OLED_iic_sda=0;
	OLED_iic_scl=0;/*   */
}
void OLED_iic_stop(void)
{
	OLED_iic_scl=1;
	OLED_iic_sda=0;
	OLED_iic_sda=1;
	OLED_iic_scl=0;
}
//OLED IIC ACK
// RETURN: 0->ACK   1->NACK
////u8 OLED_iic_wait_ack(void)
////{
////	u8 ErrTime=0;
////	OLED_iic_sda_in();
////	while(OLED_read_sda)
////	{
////		ErrTime++;
////		if(ErrTime>200)
////		{
////			OLED_iic_stop();
////			return 1;
////		}
////	}	
////	OLED_iic_scl=0;
////	return 0;
////	
////	
////	
////}
void OLED_iic_wait_ack(void)
{
	OLED_iic_scl=1;
	OLED_iic_scl=0;
}


//Generate OLED IIC ACK
void OLED_iic_ack(void)
{
	OLED_iic_scl=0;
//	OLED_iic_sda_out();
	OLED_iic_sda=0;
	OLED_iic_scl=1;
	delay_us(1);
	OLED_iic_scl=0;
}
//Generate OLED IIC NACK
void OLED_iic_Nack(void)
{
	OLED_iic_scl=0;
	OLED_iic_sda=1;
	OLED_iic_scl=1;
	delay_us(1);
	OLED_iic_scl=0;
}

void OLED_iic_send_byte(u8 byte)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		OLED_iic_scl=0;
		OLED_iic_sda=(byte&0x80)>>7;
		OLED_iic_scl=1;
		byte<<=1;
		OLED_iic_scl=0;
	}
}

void OLED_cmd(u8 cmd)
{
	OLED_iic_start();
	OLED_iic_send_byte(0x78);
	OLED_iic_wait_ack();
	OLED_iic_send_byte(0x00);
	OLED_iic_wait_ack();
	OLED_iic_send_byte(cmd);
	OLED_iic_wait_ack();
	OLED_iic_stop();
}
void OLED_dat(u8 dat)
{
	OLED_iic_start();
	OLED_iic_send_byte(0x78);
	OLED_iic_wait_ack();
	OLED_iic_send_byte(0x40);
	OLED_iic_wait_ack();
	OLED_iic_send_byte(dat);
	OLED_iic_wait_ack();
	OLED_iic_stop();
}
//cmd=1,byte->cmd
//cmd=0,byte->dat
void OLED_byte(u8 byte,u8 cmd)
{
	if(cmd)
	{
		OLED_cmd(byte);
	}
	else
	{
		OLED_dat(byte);
	}
}
//fill picture                                     
void fill_picture(u8 fill_dat)
{
	u8 m,n;
	for(m=0;m<8;m++)
	{
		OLED_byte(0xb0+m,1);//page0-page1
		OLED_byte(0x00,1);//low column start address  0x00-0x0f
		OLED_byte(0x10,1);//high column start address  0x10-0x1f
		for(n=0;n<128;n++)
		{
			OLED_byte(fill_dat,0);
		}
	}
}
//Set coordiante
void OLED_pos(u8 x,u8 y)
{
	OLED_byte(0xb0+y,1);
	OLED_byte(((x&0xf0)>>4)|0x10,1);
	OLED_byte((x&0x0f),1);
}
//Display On
void OLED_display_on(void)
{
	OLED_byte(0x8d,1);//set DCDC
	OLED_byte(0x14,1);//DCDC ON
	OLED_byte(0xaf,1);//Display On
}
void OLED_display_off(void)
{
	OLED_byte(0x8d,1);//set DCDC
	OLED_byte(0x10,1);//DCDC off
	OLED_byte(0xae,1);//Display off
}
//the OLED light off
void OLED_clear(void)
{
	u8 m,n;
	for(n=0;n<8;n++)
	{
		OLED_byte(0xb0+n,1);//Set page0-
		OLED_byte(0x00,1);//Set lower column
		OLED_byte(0x10,1);//Set upper column
		for(m=0;m<128;m++)
		{
			OLED_byte(0,0);
		}
	}
}
//the OLED light on 
void OLED_on(void)
{
	u8 m,n;
	for(n=8;n<8;n++)
	{
		OLED_byte(0xb0+n,1);//Set page0-
		OLED_byte(0x00,1);//Set lower column
		OLED_byte(0x10,1);//Set upper column
		for(m=0;m<128;m++)
		{
			OLED_byte(1,0);
		}
	}
}
//x:0-127
//y:0-63
//char_size:16*8->X8Y6  6*8->X6Y8
//char_size:
void OLED_char(u8 x,u8 y,u8 chr,u8 char_size)
{
	u8 m=0;
	u8 n=0;
	m=chr-' ';// n=(chr->ASCII)-(' '->ASCII=32)
	if(char_size==16*8)   //char_size X8,Y6
	{
		OLED_pos(x,y);
		for(n=0;n<8;n++)
		{
			OLED_byte(X8Y16[m][n],0);
		}
		OLED_pos(x,y+1);
		for(n=0;n<8;n++)
		{
			OLED_byte(X8Y16[m][n+8],0);
		}
	}
	else //char_size X6Y8
	{
		OLED_pos(x,y);
		for(n=0;n<6;n++)
		{
			OLED_byte(X6Y8[m][n],0);
		}
	}
}


//show  numbers 
//num 
void OLED_number(u8 x,u8 y,u8 num)
	{
		u8 i,j;
		y=y*8;
		for(j=0;j<2;j++)
		{
			OLED_pos(x,y+j);
			for(i=0;i<8;i++)
			{
				OLED_byte(NUMx8y16[num][i+j*8],0);
			}
		}
}
//Return:   x^y
u32 OLED_pow(u8 x,u8 y)
{
	u32 e;
	for(e=1;y>0;y--)
	{
		e=x*e;
	}
	return e;
}
//Size of value :X8Y16
void OLED_value(u8 x,u8 y,float value)
{
	u8 t,i;
	u8 flag1=0;
	u32 temp;
	u32 temp1;
	u8 flag3=0;
	temp=(u32)value;//integer part of value
	//Show the integer part
	for(t=0;t<12;t++)
	{
		
		if(temp/OLED_pow(10,11-t)!=0) flag1=1;    //if flag1=0; number 0 will  be hiden 7541->000000000007541->           7541
		if(flag1==0)
		{
			flag3++;
		}
		if(flag1==1)	
		{
			OLED_number(x+t*8-flag3*8,y,temp/(OLED_pow(10,11-t)));
		}
		temp=temp%OLED_pow(10,11-t);
	}
	OLED_number(x+(t)*8-flag3*8,y,10);
	temp1=(u32)value;//integer part of value
	temp=(u32)((value-temp1)*1000);//decimal part of value
	for(i=0;i<3;i++)
	{
		OLED_number(x+(t+i+1)*8-flag3*8,y,temp/(OLED_pow(10,(2-i))));
		temp=temp%OLED_pow(10,(2-i));
	}
	
}


/**********************************************************************/
void OLED_string(u8 x,u8 y,u8 *chr,u8 char_size)//8*16
{
	y=y*2;
	while(*chr!='\0')
	{
		if(x>120) {x=0;y+=2;}
		OLED_char(x,y,*chr,char_size);
		x+=8;
		chr++;
	}
}
void OLED_string_value(u8 x,u8 y,float value)
{
	unsigned char temp[10];
	sprintf((char *)temp,"%0.2f",value);
	OLED_string(x,y,temp,8*16);
}




void OLED_init(void)
{
	RCC->APB2ENR|=1<<5;
	GPIOD->CRL&=0xfff00fff;
	GPIOD->CRL|=0x00033000;
	GPIOD->ODR|=3<<3;
	delay_ms(200);
	
	OLED_byte(0xAE,1);//--display off
	OLED_byte(0x00,1);//---set low column address
	OLED_byte(0x10,1);//---set high column address
	OLED_byte(0x40,1);//--set start line address  
	OLED_byte(0xB0,1);//--set page address
	OLED_byte(0x81,1); // contract control
	OLED_byte(0xFF,1);//--128   
	OLED_byte(0xA1,1);//set segment remap 
	OLED_byte(0xA6,1);//--normal / reverse
	OLED_byte(0xA8,1);//--set multiplex ratio(1 to 64)
	OLED_byte(0x3F,1);//--1/32 duty
	OLED_byte(0xC8,1);//Com scan direction
	OLED_byte(0xD3,1);//-set display offset
	OLED_byte(0x00,1);//
	
	OLED_byte(0xD5,1);//set osc division
	OLED_byte(0x80,1);//
	
	OLED_byte(0xD8,1);//set area color mode off
	OLED_byte(0x05,1);//
	
	OLED_byte(0xD9,1);//Set Pre-Charge Period
	OLED_byte(0xF1,1);//
	
	OLED_byte(0xDA,1);//set com pin configuartion
	OLED_byte(0x12,1);//
	
	OLED_byte(0xDB,1);//set Vcomh
	OLED_byte(0x30,1);//
	
	OLED_byte(0x8D,1);//set charge pump enable
	OLED_byte(0x14,1);//
	
	OLED_byte(0xAF,1);//--turn on oled panel
	
	OLED_on();
	OLED_clear();
}










