#include "usart.h"
#include "led.h"


#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定向fputc函数
//printf的输出，指向fputc，由fputc输出到串口
//这里使用串口1(USART1)输出printf信息
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//等待上一次串口数据发送完成  
	USART1->DR = (u8) ch;      	//写DR,串口1将发送数据
	return ch;
}
#endif 



#if EN_USART1_RX //如果使能了接收
//串口1中断服务程序
//读取USARTx->SR能避免莫名奇妙的错误
u8 USART_RX_BUF[USART_REC_LEN]; //接收缓冲，最大USART_REC_LEN字节
//接收状态
//bit15:接收完成标志位
//bit14:接收0x0d标志位
//bit13~0:接收到的有效数据个数
u16 USART_RX_STA=0;//接收状态标志位
//void USART1_IRQHandler(void)
//{
//		u8 res;
////	#if SYSTEM_SUPPORT_OS //如果 SYSTEM_SUPPORT_OS 为真，则需要支持 OS.
////OSIntEnter();
////#endif
//	if(USART1->SR&(1<<5))
//	{
//		res=USART1->DR;
//		if((USART_RX_STA&0x8000)==0)//接收未完成
//		{ 
//			if(USART_RX_STA&0x4000)//接收到0x0d
//			{
//				if(res!=0x0a) USART_RX_STA=0;//接收错误，重新开始
//				else USART_RX_STA|=0x8000;//接收完成
//			}else//还没有接收到0x0d
//				{
//					if(res==0x0d) USART_RX_STA|=0x4000;//接收到了0x0d,标记USART_RX_STA bit14位
//						else
//						{
//							USART_RX_BUF[USART_RX_STA&0x3fff]=res;
//							USART_RX_STA++;
//							if(USART_RX_STA>(USART_REC_LEN-1)) USART_RX_STA=0;
//							//接收数据错误，重新开始接收	
//						}
//				}
//		}
//	}
////	#if SYSTEM_SUPPORT_OS //如果 SYSTEM_SUPPORT_OS 为真，则需要支持 OS.
////OSIntExit();
////#endif
//}
#endif



void USART1_IRQHandler(void)
{
	u8 res;
	
	if(USART1->SR&(1<<5))
	{
		res=USART1->DR;
		USART_RX_BUF[USART_RX_STA]=res;
		USART_RX_STA++;
	}
	
}
void USART1_rxbuf_send(u8 flag)
{
	printf(USART_RX_BUF);
	if(flag==0)
		memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));
	USART_RX_STA=0;

}
void USART1_rxbuf_clear(void)
{
		memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));
}

void USART1_copy_rxbuf(u8 *buf,u8 num)
{
	u8 i;
	for(i=0;i<num;i++)
	{
		*buf=USART_RX_BUF[i];
		buf++;
	}
}










//pclk2:PCLK2 时钟频率
//bound:波特率
void USART1_init(u32 pclk2,u32 bound)
{
	/*
	float temp;
	u16 mantissa,fraction;
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;//USRTDIV整数部分
	fraction=(temp-mantissa)*16;//USARTDIV小数部分

	RCC->APB2ENR|=1<<2;//GPIOA使能
	RCC->APB2ENR|=1<<14;//使能串口1
	GPIOA->CRH&=0XFFFFF00F;//IO 状态设置
	GPIOA->CRH|=0X000008B0;//输入模式，推挽50M输出
	RCC->APB2RSTR|=1<<14; //复位串口 1
	RCC->APB2RSTR&=~(1<<14);//停止复位
	//波特率设置
	USART1->BRR|=mantissa<<4;
	USART1->BRR|=fraction;
	USART1->CR1|=0X200C; //USART1模块/发送/接收使能
	#if EN_USART1_RX  //如果使能了接收
	//使能接收中断
	USART1->CR1|=1<<5;//接收缓冲区非空中断使
	NVIC_priority(3,3,USART1_IRQn,2);//组2，最低优先级
	#endif
	*/
	
		float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH&=0XFFFFF00F;//IO状态设置
	GPIOA->CRH|=0X000008B0;//IO状态设置 
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.
#if EN_USART1_RX		  //如果使能了接收
	//使能接收中断 
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	NVIC_priority(3,3,USART1_IRQn,2);//组2，最低优先级 
#endif
	
	
	
	
	
}


void USART1_send_one_byte(u8 byte)
{
	while((USART1->SR&0x40)==0);
	USART1->DR=byte;
	
}
void USART1_send_one_value(float value)
{
	while((USART1->SR&0x40)==0);
	USART1->DR=value;
}


//void USART1_Send(u8 *buf,u8 len)
//{
//	while(len)
//	{
//		len--;
//		USART1_send_one_byte(*buf);
//	}
//}
void USART1_send(u8 *buf)
{
	while(*buf!='\0')
	{
			USART1_send_one_byte(*buf);
		buf++;
	}
}


