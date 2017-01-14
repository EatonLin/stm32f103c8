#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"

#define EN_USART1_RX 1

#define UART_BUFFER_SIZE 6
extern u8 USART1_SEND_DATA[UART_BUFFER_SIZE];
extern u8 USART1_RECEIVE_DATA[UART_BUFFER_SIZE];

void uart_init(u32 bound);
void SendUartData(u8 *buffer, u8 size);
#endif
