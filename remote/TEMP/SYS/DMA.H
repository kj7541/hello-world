#ifndef _dma_h
#define _dma_h
#include "delay.h"
void DMA_init(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);
void DMA_enable(DMA_Channel_TypeDef*DMA_CHx);



#endif




