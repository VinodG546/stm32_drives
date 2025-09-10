/*
 * i2c.h
 *
 *  Created on: Sep 10, 2025
 *      Author: VINOD
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32l4xx.h"


void i2c1_init();
void i2c1_master_send(uint8_t slave_addr, uint8_t *data, uint32_t size);
void i2c1_master_receive(uint8_t slave_addr, uint8_t *buffer, uint32_t size);

#endif /* I2C_H_ */
