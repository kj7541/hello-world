#include "dma.h"
u16 DMA1_MEM_len;
//DMA_chx:DMA channel x
//cpar:Peripheral address register
//cmar:Memory address register
//cndtr:Number of data to transfer register
void DMA_init(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	RCC->AHBENR|=1<<0;
	delay_ms(5);
	DMA_CHx->CPAR=cpar;
	DMA_CHx->CMAR=(u32)cmar;
	DMA1_MEM_len=cndtr;
	DMA_CHx->CNDTR=cndtr;
	DMA_CHx->CCR=0x00000000;
	DMA_CHx->CCR|=1<<4;
	DMA_CHx->CCR|=0<<5;
	DMA_CHx->CCR|=0<<6;
	DMA_CHx->CCR|=1<<7;
	DMA_CHx->CCR|=0<<8;
	DMA_CHx->CCR|=0<<10;
	DMA_CHx->CCR|=1<<12;
	DMA_CHx->CCR|=0<<14;
}
void DMA_enable(DMA_Channel_TypeDef*DMA_CHx)
{
	DMA_CHx->CCR&=~(1<<0);
	DMA_CHx->CNDTR=DMA1_MEM_len;
	DMA_CHx->CCR|=1<<0;
}

