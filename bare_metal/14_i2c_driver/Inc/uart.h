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

void USART2_tx_rx_init(void);
void uart2_write(int ch);
char uart2_read(void);

#endif /* UART_H_ */
