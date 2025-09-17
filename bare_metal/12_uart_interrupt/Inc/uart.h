/*
 * uart.h
 *
 *  Created on: Sep 9, 2025
 *      Author: VINOD
 */

#ifndef UART_H_
#define UART_H_
#include<stdint.h>
#include "stm32l4xx.h"


#define ISR_RXNE    (1U << 5)
#define ISR_TXE     (1U << 7)

#define GPIOAEN     (1U << 0)
#define USART2EN    (1U << 17)

#define SYS_FREQ        16000000
#define UART_BAUDRATE   115200

#define CR1_TE      (1U << 3)
#define CR1_RE      (1U << 2)
#define CR1_UE      (1U << 0)



#define CR1_RXNEIE	(1U << 5)
#define CR1_TXEIE   (1U << 7)

void USART2_tx_rx_init(void);
void uart2_write(int ch);
char uart2_read(void);

void USART2_tx_rx_interrupt_init(void);
#endif /* UART_H_ */
