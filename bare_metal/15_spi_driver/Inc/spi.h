/*
 * spi.h
 *
 *  Created on: Sep 10, 2025
 *      Author: VINOD
 */

#ifndef SPI_H_
#define SPI_H_
#include"stm32l4xx.h"
#include<stdint.h>

void spi1_transmit_receive(uint8_t *tx_buf, uint8_t *rx_buf, uint32_t len);
void spi1_init(void);


#endif /* SPI_H_ */
