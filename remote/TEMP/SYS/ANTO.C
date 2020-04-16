#include "ANTO.h"

#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

u8 data_to_send[50];	//发送数据缓存

u8 ANO_Data[50];

void ANO_Send(u8 *buf,u8 len)
{
	while(len)
	{
		USART1_send_one_byte(*buf);
		buf++;
		len--;
	}
}
 
void ANO_Send_Sensor(u16 ax,u16 ay,u16 az,u16 gx,u16 gy,u16 gz)
{
	u8 i=0,sum=0;
	ANO_Data[i++]=0xaa;
	ANO_Data[i++]=0xaa;
	ANO_Data[i++]=0x02;
	ANO_Data[i++]=0x18;
	ANO_Data[i++]=(ax>>8)&0xff;//ax
	ANO_Data[i++]=ax&0xff;
	ANO_Data[i++]=(ay>>8)&0xff;//ay
	ANO_Data[i++]=ay&0xff;
	ANO_Data[i++]=(az>>8)&0xff;//az
	ANO_Data[i++]=az&0xff;
	ANO_Data[i++]=(gx>>8)&0xff;//gx
	ANO_Data[i++]=gx&0xff;
	ANO_Data[i++]=(gy>>8)&0xff;//gy
	ANO_Data[i++]=gy&0xff;
	ANO_Data[i++]=(gz>>8)&0xff;//gz
	ANO_Data[i++]=gz&0xff;
	ANO_Data[i++]=0x00;//mx
	ANO_Data[i++]=0x00;
	ANO_Data[i++]=0x00;//my
	ANO_Data[i++]=0x00;
	ANO_Data[i++]=0x00;//mz
	ANO_Data[i++]=0x00;//i=22
	for(i=0;i<=21;i++)
	{
		sum+=ANO_Data[i];
	}
	ANO_Data[22]=sum;
	ANO_Send(ANO_Data,23);
}

void ANO_Send_Status(float rol,float pit,float yaw)
{
	u8 i=0,sum=0;
	u16 temp;
	float PIT,YAW,ROL;
	ANO_Data[i++]=0xaa;
	ANO_Data[i++]=0xaa;
	ANO_Data[i++]=0x01;
	ANO_Data[i++]=12;
	//Gx
	if(pit<-7){PIT=360+pit;}
	else{PIT=pit;}
	temp=(u16)(PIT*100);
	ANO_Data[i++]=(temp>>8)&0xff;
	ANO_Data[i++]=temp&0xff;
	//Gy
	if(rol<-7){ROL=360+rol;}
	else{ROL=rol;}
	temp=(u16)(ROL*100);
	ANO_Data[i++]=(temp>>8)&0xff;//ay
	ANO_Data[i++]=temp&0xff;
	//Gz
	if(yaw<-7){YAW=360+yaw;}
	else{YAW=yaw;}
	temp=(u16)(YAW*100);
	ANO_Data[i++]=(temp>>8)&0xff;//az
	ANO_Data[i++]=temp&0xff;
	// u32 Height  
	ANO_Data[i++]=1;
	ANO_Data[i++]=0;
	ANO_Data[i++]=0;
	ANO_Data[i++]=0;
	// u8 MODE
	ANO_Data[i++]=0;
	// u8 LOCK
	ANO_Data[i++]=1;//i=16
	for(i=0;i<=15;i++)
	{
		sum+=ANO_Data[i]; 
	}
	ANO_Data[15]=sum;
	ANO_Send(ANO_Data,17);
	
}


/////////////////////////////////////////////////////////////////////////////////////
//Send_Data函数是协议中所有发送数据功能使用到的发送函数
//移植时，用户应根据自身应用的情况，根据使用的通信方式，实现此函数
//void ANO_DT_Send_Data(u8 *dataToSend , u8 length)
//{
//	USART1_Send(data_to_send, length);
//}

//static void ANO_DT_Send_Check(u8 head, u8 check_sum)
//{
//	u8 sum = 0,i;
//	data_to_send[0]=0xAA;
//	data_to_send[1]=0xAA;
//	data_to_send[2]=0xEF;
//	data_to_send[3]=2;
//	data_to_send[4]=head;
//	data_to_send[5]=check_sum;
//	for(i=0;i<6;i++)
//		sum+= data_to_send[i];
//	data_to_send[6]=sum;

//	ANO_DT_Send_Data(data_to_send, 7);
//}










/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
























