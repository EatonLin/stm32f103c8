#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "dma.h"
#include "input_source.h"

u8 USART1_SEND_DATA[UART_BUFFER_SIZE] = {0x0a, 0x8b, 0x00, 0x00, 0x00, 0x9c};

int main(void)
{
    u16 i = 0;
    //u8 j = 0;
    //u8 irqChannel = 0;
    delay_init();
    NVIC_Configuration();
    uart_init(115200);
    LED_Init();
	InputSourceGPIO_Init();
	/*
    irqChannel = DMA1_Channel4_IRQn;
    MYDMA_Config(DMA1_Channel4, (u32)&USART1->DR, (u32)USART1_SEND_DATA, DMA_DIR_PeripheralDST, DMA_Mode_Circular, NULL, UART_BUFFER_SIZE);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    DMA_Cmd(DMA1_Channel4, DISABLE);
	*/
	/*
    irqChannel = DMA1_Channel5_IRQn;
    MYDMA_Config(DMA1_Channel5, (u32)&USART1->DR, (u32)USART1_RECEIVE_DATA, DMA_DIR_PeripheralSRC, DMA_Mode_Circular, NULL, UART_BUFFER_SIZE);
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
    */
    USART_Cmd(USART1, ENABLE);
	
    while (1)
    {
		if (i % 10 == 0)
		{
			USART1_SEND_DATA[2] = InputSource_Detected();
			USART1_SEND_DATA[4] = USART1_SEND_DATA[2] + USART1_SEND_DATA[3];
            SendUartData(USART1_SEND_DATA, UART_BUFFER_SIZE);
			i = 0;
		}
		
        if (i % 80 == 0)
		{
            LED0 = !LED0;
		}
		i++;
		delay_ms(10);
    }
}
