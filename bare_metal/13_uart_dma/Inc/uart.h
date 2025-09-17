/*
 * uart.h
 *
 *  Created on: Sep 9, 2025
 *      Author: VINOD
 */
#ifndef UART_H_
#define UART_H_

#include "stm32l4xx.h"

#define ISR_RXNE    (1 << 5)
#define CR1_RXINE   (1U<<5)
void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
void uart2_write(int ch);
void USART2_rxtx_init(void);
char uart2_read(void) ;
void USART2_rx_interrupt_init(void) ;
void dma1_stream6_init(uint32_t  src  , uint32_t dst , int len  );


#define ISR_TCIF7      (1<<25)
#define IFCR_CTCIF     (1<<25)

#endif /* UART_H_ */
