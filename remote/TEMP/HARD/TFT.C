//V1.0
//BUG1:accuracy ->6 bit number
//AS:Input 75417542 Show 75417544
//BUG2:accuracy ->float number a little mistake
//AS:Input 75036.752 Show 75036.757



#include "TFT.h"
//LCD write byte
//dc=1 data;dc=0 command
void LCD_1byte(u8 byte,u8 dc)
{
	u8 i;
	LCD_scl=0;
	LCD_dc=dc;
	LCD_cs=0;
	delay_us(1);
	for(i=0;i<8;i++)
	{
		LCD_scl=0;
		LCD_sda=(byte&0x80)>>7;
		byte<<=1;
		delay_us(1);
		LCD_scl=1;;;
	}
}
void LCD_1byte2(u16 byte,u8 dc)
{
	LCD_1byte((byte>>8)&0xff,dc);
	LCD_1byte(byte&0x00ff,dc);
}
void LCD_init(void)
{
	RCC->APB2ENR|=1<<5;
	GPIOD->CRL&=0xfff00000;
	GPIOD->CRL|=0x00033333;
	LCD_1byte(0x11,cmd);//Sleep out
	delay_ms(130);
	LCD_1byte(0xb1,cmd);//LCD Frame rate *
	LCD_1byte(0x00,dat);
	LCD_1byte(0x00,dat);
	LCD_1byte(0x00,dat);
	LCD_1byte(0xb2,cmd);//LCD Frame rate 
	LCD_1byte(0x00,dat);
	LCD_1byte(0x00,dat);
	LCD_1byte(0x00,dat);
	LCD_1byte(0xb3,cmd);//LCD Frame rate 
	LCD_1byte(0x00,dat);
	LCD_1byte(0x00,dat);
	LCD_1byte(0x00,dat);
	LCD_1byte(0x00,dat);
	LCD_1byte(0x00,dat);
	LCD_1byte(0x00,dat);
	LCD_1byte(0xb4,cmd);//Display inversion ctrl*
	LCD_1byte(0x07,dat);
	LCD_1byte(0xc0,cmd);//Power ctrl1
	LCD_1byte(0xa2,dat);
	LCD_1byte(0x02,dat);
	LCD_1byte(0x84,dat);
	LCD_1byte(0xc1,cmd);//Power ctrl2
	LCD_1byte(0xc5,dat);
	LCD_1byte(0xc2,cmd);//Power ctrl3
	LCD_1byte(0x0a,dat);
	LCD_1byte(0x00,dat);
	LCD_1byte(0xc3,cmd);//Power ctrl4
	LCD_1byte(0x8a,dat);
	LCD_1byte(0x2a,dat);
	LCD_1byte(0xc4,cmd);//Power ctrl5
	LCD_1byte(0x8a,dat);
	LCD_1byte(0xee,dat);
	LCD_1byte(0xc5,cmd);//Vcom ctrl1
	LCD_1byte(0x0e,dat);
	LCD_1byte(0x36,cmd);//MX MY RGB
	LCD_1byte(0xc0,dat);
	LCD_1byte(0xe0,cmd);//Gamma
	LCD_1byte(0x0f,dat); 
	LCD_1byte(0x1a,dat); 
	LCD_1byte(0x0f,dat);
	LCD_1byte(0x18,dat); 
	LCD_1byte(0x2f,dat); 
	LCD_1byte(0x28,dat); 
	LCD_1byte(0x20,dat); 
	LCD_1byte(0x22,dat); 
	LCD_1byte(0x1f,dat); 
	LCD_1byte(0x1b,dat); 
	LCD_1byte(0x23,dat); 
	LCD_1byte(0x37,dat); 
	LCD_1byte(0x00,dat); 	
	LCD_1byte(0x07,dat); 
	LCD_1byte(0x02,dat); 
	LCD_1byte(0x10,dat);
	LCD_1byte(0xe1,cmd);//Gamma
	LCD_1byte(0x0f,dat); 
	LCD_1byte(0x1b,dat); 
	LCD_1byte(0x0f,dat); 
	LCD_1byte(0x17,dat); 
	LCD_1byte(0x33,dat); 
	LCD_1byte(0x2c,dat); 
	LCD_1byte(0x29,dat); 
	LCD_1byte(0x2e,dat); 
	LCD_1byte(0x30,dat); 
	LCD_1byte(0x30,dat); 
	LCD_1byte(0x39,dat); 
	LCD_1byte(0x3f,dat); 
	LCD_1byte(0x00,dat); 
	LCD_1byte(0x07,dat); 
	LCD_1byte(0x03,dat); 
	LCD_1byte(0x10,dat); 
	LCD_1byte(0x2a,cmd);//Cloumn address set 128
	LCD_1byte(0x00,dat);
	LCD_1byte(0x00,dat);
	LCD_1byte(0x00,dat);
	LCD_1byte(0x7f,dat);
	LCD_1byte(0x2b,cmd);//Row address set 160
	LCD_1byte(0x00,dat);
	LCD_1byte(0x00,dat);
	LCD_1byte(0x00,dat);
	LCD_1byte(0x9f,dat);
	LCD_1byte(0xf0,cmd);//Enable test command
	LCD_1byte(0x01,dat);
	LCD_1byte(0xf6,cmd);//Disable ram power save mode
	LCD_1byte(0x00,dat);
	LCD_1byte(0x3a,cmd);//65k mode 
	LCD_1byte(0x05,dat);
	
	LCD_1byte(0x29,0);//Display on
//	LCD_1byte(0x)
	
}
void LCD_set_region(u8 xs,u8 xe,u8 ys,u8 ye)
{
	LCD_1byte(0x2a,cmd);
	LCD_1byte(0x00,dat);
	LCD_1byte(xs+2,dat);
	LCD_1byte(0x00,dat);
	LCD_1byte(xe+2,dat);
	LCD_1byte(0x2b,cmd);
	LCD_1byte(0x00,dat);
	LCD_1byte(ys+1,dat);
	LCD_1byte(0x00,dat);
	LCD_1byte(ye+1,dat);
	LCD_1byte(0xc0,cmd);
}

void LCD_draw_point(u8 x,u8 y,u16 data)
{
	LCD_set_region(x,x+1,y,y+1);
	LCD_1byte(0x2c,cmd);
	LCD_1byte2(data,dat);
}
	
void LCD_show_bit(u8 x,u8 y,u16 color,u8 byte)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(byte>>i&0x01)
			LCD_draw_point(x,y+i,color);
		else
			LCD_draw_point(x,y+i,BLACK);
	}
}




void LCD_clear(u16 color)
{
	u8 i,m;
	LCD_set_region(0,127,0,159);
	LCD_1byte(0x2c,cmd);
	for(i=0;i<128;i++)
	{
		for(m=0;m<160;m++)
		{
			LCD_1byte2(color,dat);
		}
	}
		
}
void LCD_char(u8 x,u8 y,u16 color,u8 chr)
{
	u8 i,t;
	chr-=' ';
	x*=8;
	y*=16;
		for(i=0;i<2;i++)//First floor
		{
			for(t=0;t<8;t++)//8 byte
			{
				LCD_show_bit(x+t,y+i*8,color,X8Y16[chr][t+i*8]);
			}
		}
}	
void LCD_string(u8 x,u8 y,u16 color,u8 *buf)
{
	u8 i=0;
	while(*buf!='\0')
	{
		LCD_char(x+i,y,color,*buf);
		buf++;
		i++;
	}
}
void LCD_value_(u8 x,u8 y,u8 color,float value)
{
	unsigned char temp[10];
	sprintf((char *)temp,"%0.2f",value);
	LCD_string(x,y,color,temp);
}



void LCD_number(u8 x,u8 y,u16 color,u8 num)
{
	u8 i;
	x*=8;
	y*=16;
	for(i=0;i<8;i++)
	{
		LCD_show_bit(x+i,y,color,NUMx8y16[num][i]);
	}
	for(i=0;i<8;i++)
	{
		LCD_show_bit(x+i,y+8,color,NUMx8y16[num][i+8]);
	}
}
u32 LCD_pow(u8 x,u8 y)
{
	u32 e;
	for(e=1;y>0;y--)
	{
		e=x*e;
	}
	return e;
}
//value MAX 10^11

//void LCD_value(u8 x,u8 y,u16 color,float value)// value +-E34
//{
//	u8 i=12,k=0,t;
//	u32 temp;
//	u8 z,flag1;
//	if(value>LCD_pow(10,13))
//	{
//		LCD_string(x,y,RED,"ERROR->MAX");
//	}
//	//Show integer part value
//	else
//	{
//		temp=value;//Integer part value
//		while((i>=0)&&(i<20))//Set flag1; the first number of value isn't 0;
//		{
//			if(temp>LCD_pow(10,i))
//			{
//				flag1=1;//The first number of value isn't 0;
//				t=i;
//				i=30;//break out the loop;
//			}
//			i--;
//		}
//		while(flag1)
//		{
//			while((t>=0)&&(i<40))//Show integer part of value 
//			{
//				z=temp/LCD_pow(10,t);
//				LCD_number(x+k,y,color,z);
//				k++;
//				if(t==0)//break out the loop;
//				{i=50;flag1=0;}
//				temp=temp-z*LCD_pow(10,t);
//				if(t!=0)
//					t--;
//			}
//		}
//	}
////show decimal part of value
//	temp=(u16)((value-(u32)value)*1000);
//	if(temp>0)
//	{
//		LCD_char(x+k,y,color,'.');
//		flag1=1;
//		k++;
//		i=2;
//			while(flag1)
//			{
//				while(i>=0)
//				{
//					LCD_number(x+k,y,color,(temp/LCD_pow(10,i)));
//					k++;
//					temp%=LCD_pow(10,i);
//					if(i==0)
//						break;
//					i--;
//				}
//				break;
//			}
//	}
//}














