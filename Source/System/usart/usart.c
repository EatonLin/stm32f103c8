#include "sys.h"
#include "usart.h"
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"
#endif
#if 1
#pragma import(__use_no_semihosting)
struct __FILE
{
    int handle;
};

FILE __stdout;
_sys_exit(int x)
{
    x = x;
}
int fputc(int ch, FILE *f)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;
    USART_SendData(USART1, (uint8_t)ch);
    return ch;
}
#endif

/*
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}

    return ch;
}
int GetKey (void)  {

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/

#if EN_USART1_RX
u8 USART1_RECEIVE_DATA[UART_BUFFER_SIZE];
u8 USART1_TX_Finish = 1;
u32 correct_count = 0;
u32 uncorrect_count = 0;

void uart_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    USART_DeInit(USART1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
#if EN_USART1_RX

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(USART1, USART_IT_TC, DISABLE);
    //USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    //USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
#endif
    //USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void)
{
    u16 DATA_LEN;
    u16 i;
    u8 percent = 0;
#ifdef OS_TICKS_PER_SEC
    OSIntEnter();
#endif
    if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        DMA_Cmd(DMA1_Channel5, DISABLE);
        DATA_LEN = UART_BUFFER_SIZE - DMA_GetCurrDataCounter(DMA1_Channel5);
        percent = (correct_count * 100) / (correct_count + uncorrect_count);
        printf("DATA_LEN:%d, correct_count:%d, uncorrect_count:%d, %d\%\n", DATA_LEN, correct_count, uncorrect_count, percent);
        if (DATA_LEN > 0)
        {
            /*
			while(USART1_TX_Finish==0)
			{
				;
			}
			*/
            for (i = 0; i < DATA_LEN; i++)
            {
                if (USART1_RECEIVE_DATA[i] != (0x90 + i)) break;
            }
            if (i == DATA_LEN)
                correct_count++;
            else
            {
                uncorrect_count++;
                printf("Failed:%d, %d\n", i, USART1_RECEIVE_DATA[i]);
            }

            /*
            if (USART1_TX_Finish == 1)
            {
                USART1_TX_Finish = 0;
                DMA_Cmd(DMA1_Channel4, DISABLE);
                DMA_SetCurrDataCounter(DMA1_Channel4, DATA_LEN);
                DMA_Cmd(DMA1_Channel4, ENABLE);
            }
            */

            DMA_ClearFlag(DMA1_FLAG_GL5 | DMA1_FLAG_TC5 | DMA1_FLAG_TE5 | DMA1_FLAG_HT5);
            DMA_SetCurrDataCounter(DMA1_Channel5, UART_BUFFER_SIZE);
            DMA_Cmd(DMA1_Channel5, ENABLE);
        }
        if (USART_GetITStatus(USART1, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)
        {
            USART_ClearITPendingBit(USART1, USART_IT_PE | USART_IT_FE | USART_IT_NE);
        }
        USART_ClearITPendingBit(USART1, USART_IT_TC);
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
        i = USART1->SR;
        i = USART1->DR;
    }
#ifdef OS_TICKS_PER_SEC
    OSIntExit();
#endif
}
#endif

void SendUartData(u8 *buffer, u8 size)
{
	int i = 0;
    for (i = 0; i < size; i++)
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART_SendData(USART1, buffer[i]);
    }
}
