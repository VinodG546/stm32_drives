/*
 * i2c.c
 *
 *  Created on: Sep 10, 2025
 *      Author: VINOD
 */

#include"i2c.h"


/*pinout
 * pb8 - scl
 * pb9 - sda
 * */
#define GPIOBEN  (1U<<1)
#define I2C1EN  (1U<<21)

void i2c1_init()
{
	RCC->AHB2ENR |= GPIOBEN;
	RCC->APB1ENR1 |= I2C1EN;

	// PB8 AF mode
	GPIOB->MODER &= ~(3U << 16);
	GPIOB->MODER |=  (2U << 16);

	// PB9 AF mode
	GPIOB->MODER &= ~(3U << 18);
	GPIOB->MODER |=  (2U << 18);

	GPIOB->OTYPER |= (1<<8);
	GPIOB->OTYPER |= (1<<9);

	GPIOB->PUPDR &= ~(3U << 16);   // clear
	GPIOB->PUPDR |=  (1U << 16);   // PB8 pull-up

	GPIOB->PUPDR &= ~(3U << 18);   // clear
	GPIOB->PUPDR |=  (1U << 18);   // PB9 pull-up

	GPIOB->AFR[1] &= ~((0xF << 0) | (0xF << 4));   // clear AF for PB8, PB9
	GPIOB->AFR[1] |=  ((4 << 0) | (4 << 4));       // AF4 = I2C1

	I2C1 -> CR1 &= ~(1<<0);

	I2C1 -> TIMINGR = 0x10420A28;

	I2C1 -> OAR1 &= ~(1<<15);

	I2C1 ->OAR1 &= (1<<10);

	I2C1 -> OAR1 &=~(0x3ff);

	I2C1 -> OAR1 |= (1<<15);

	I2C1 -> CR2 &= ~(1<<15);

	I2C1 -> CR1 |= (1<<0);
}

void i2c1_master_send(uint8_t slave_addr, uint8_t *data, uint32_t size)
{
    uint32_t tmpreg;

    // 1. Wait until I2C is not busy
    while (I2C1->ISR & I2C_ISR_BUSY);

    // 2. Configure slave address and transfer size
    tmpreg  = I2C1->CR2;
    tmpreg &= ~(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RD_WRN | I2C_CR2_AUTOEND);

    tmpreg |= (slave_addr << 1) & I2C_CR2_SADD;   // 7-bit slave addr
    tmpreg |= (size << 16);                       // NBYTES
    tmpreg &= ~I2C_CR2_RD_WRN;                    // Write
    tmpreg |= I2C_CR2_START;                      // Generate START

    I2C1->CR2 = tmpreg;

    // 3. Send data bytes
    for (uint32_t i = 0; i < size; i++)
    {
        while (!(I2C1->ISR & I2C_ISR_TXIS));      // Wait until TXIS set
        I2C1->TXDR = data[i];
    }

    // 4. Wait until transfer complete
    while (!(I2C1->ISR & I2C_ISR_TC));

    // 5. Generate STOP
    I2C1->CR2 |= I2C_CR2_STOP;

    // 6. Wait until STOP flag set
    while (!(I2C1->ISR & I2C_ISR_STOPF));

    // 7. Clear STOP flag
    I2C1->ICR |= I2C_ICR_STOPCF;
}


void i2c1_master_receive(uint8_t slave_addr, uint8_t *buffer, uint32_t size)
{
    uint32_t tmpreg;

    // 1. Wait until I2C not busy
    while (I2C1->ISR & I2C_ISR_BUSY);

    // 2. Configure CR2 for slave address, number of bytes, read mode
    tmpreg  = I2C1->CR2;
    tmpreg &= ~(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RD_WRN | I2C_CR2_AUTOEND);

    tmpreg |= (slave_addr << 1) & I2C_CR2_SADD;   // 7-bit address
    tmpreg |= (size << 16);                       // NBYTES
    tmpreg |= I2C_CR2_RD_WRN;                     // Set for Read
    tmpreg |= I2C_CR2_START;                      // Generate START

    I2C1->CR2 = tmpreg;

    // 3. Read incoming bytes
    for (uint32_t i = 0; i < size; i++)
    {
        while (!(I2C1->ISR & I2C_ISR_RXNE));      // Wait until RXNE
        buffer[i] = (uint8_t)I2C1->RXDR;
    }

    // 4. Wait until transfer complete
    while (!(I2C1->ISR & I2C_ISR_TC));

    // 5. Generate STOP
    I2C1->CR2 |= I2C_CR2_STOP;

    // 6. Wait until STOP flag set
    while (!(I2C1->ISR & I2C_ISR_STOPF));

    // 7. Clear STOP flag
    I2C1->ICR |= I2C_ICR_STOPCF;
}

