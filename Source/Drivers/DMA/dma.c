#include "dma.h"
#include "usart.h"

DMA_InitTypeDef DMA_InitStructure;

u16 DMA1_MEM_LEN;

void MYDMA_Config(DMA_Channel_TypeDef *DMA_CHx, u32 cpar, u32 cmar, u32 dma_dir, u32 dma_mode, u8 *irqChannel, u16 cndtr)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA_CHx);
    DMA1_MEM_LEN = cndtr;
    DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;
    DMA_InitStructure.DMA_MemoryBaseAddr = cmar;
    DMA_InitStructure.DMA_DIR = dma_dir;
    DMA_InitStructure.DMA_BufferSize = cndtr;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = dma_mode;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA_CHx, &DMA_InitStructure);
    DMA_ITConfig(DMA_CHx, DMA_IT_TC, ENABLE);
    DMA_ITConfig(DMA_CHx, DMA_IT_TE, ENABLE);

	if (irqChannel != NULL)
		NVIC_IRQChannel_ENABLE(irqChannel, 2, 0);
}

void MYDMA_Enable(DMA_Channel_TypeDef *DMA_CHx)
{
    DMA_Cmd(DMA_CHx, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel4, DMA1_MEM_LEN);
    DMA_Cmd(DMA_CHx, ENABLE);
}

void DMA1_Channel4_IRQHandler(void)
{
    DMA_ClearITPendingBit(DMA1_IT_TC4);
    DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel4, UART_BUFFER_SIZE);
	//USART1_TX_Finish = 1;
}

void DMA1_Channel5_IRQHandler(void)
{
    DMA_ClearITPendingBit(DMA1_IT_TC5);
    DMA_ClearITPendingBit(DMA1_IT_TE5);
    DMA_Cmd(DMA1_Channel5, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel5, UART_BUFFER_SIZE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
}
