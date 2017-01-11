#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "dma.h"

extern u8 USART1_SEND_DATA[UART_BUFFER_SIZE];
extern u8 USART1_RECEIVE_DATA[UART_BUFFER_SIZE];
extern u8 USART1_TX_Finish;

int main(void)
{
    u16 i = 0;
    //u8 j = 0;
    u8 irqChannel = 0;
    delay_init();
    NVIC_Configuration();
    uart_init(115200);
    LED_Init();
	/*
    irqChannel = DMA1_Channel4_IRQn;
    MYDMA_Config(DMA1_Channel4, (u32)&USART1->DR, (u32)USART1_SEND_DATA, DMA_DIR_PeripheralDST, DMA_Mode_Circular, NULL, UART_BUFFER_SIZE);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    DMA_Cmd(DMA1_Channel4, DISABLE);
	*/
    irqChannel = DMA1_Channel5_IRQn;
    MYDMA_Config(DMA1_Channel5, (u32)&USART1->DR, (u32)USART1_RECEIVE_DATA, DMA_DIR_PeripheralSRC, DMA_Mode_Circular, NULL, UART_BUFFER_SIZE);
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
    USART_Cmd(USART1, ENABLE);
    while (1)
    {
	if (USART1_TX_Finish == 0)
	{
	    /*
	    for (j = 0; j < 10; j++)
	    {
		printf("%d ", USART1_SEND_DATA[i]);
	    }
	    printf("\n");
		*/
	    /*
		DMA_Cmd(DMA1_Channel4, DISABLE);
		DMA_SetCurrDataCounter(DMA1_Channel4, 10);
		DMA_Cmd(DMA1_Channel4, ENABLE);
	    USART1_TX_Finish = 1;
		*/
	}
	if (i == 20)
	{
	    LED0 = !LED0;
	    i = 0;
	}
	i++;
	delay_ms(20);
    }
}
