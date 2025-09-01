/*
 * stm32l4xx_i2c_driver.c
 *
 *  Created on: Aug 28, 2025
 *      Author: VINOD
 */
#include"stm32l4xx_i2c_driver.h"
#include"stm32l4xx.h"

void I2C_PclkControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(pI2Cx == I2C1)
        {
            I2C1_PCLK_EN();
        }
        else if(pI2Cx == I2C2)
        {
            I2C2_PCLK_EN();
        }
        else if(pI2Cx == I2C3)
        {
            I2C3_PCLK_EN();
        }
    }
    else
    {

    }
}
/*
 * I2C Init & Deinit
 */
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	//enable pclk clock for spi
	I2C_PclkControl(pI2CHandle->pI2Cx,ENABLE);

	//disable the peripheral before configuring
	I2C_PeripheralControl(pI2CHandle->pI2Cx,DISABLE);

	//configure timing register
	pI2CHandle->pI2Cx->TIMINGR =(uint32_t) 0x10420A28;

	//configure own address

	pI2CHandle->pI2Cx->OAR1 &= ~(1<<I2c_OAR1_OA1EN);//disable EN bit first
	pI2CHandle->pI2Cx->OAR1 |= (0<<10) | (pI2CHandle->I2C_Config.I2C_DeviceAddress<<1);
	pI2CHandle->pI2Cx->OAR1 |= (1<<I2c_OAR1_OA1EN);

	//enable ACK will be used only for slave mode
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE)
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_NACK);

	//enable the peripheral
	I2C_PeripheralControl(pI2CHandle->pI2Cx, ENABLE);
}
void I2C_Deinit(I2C_RegDef_t *pI2Cx)
{
	if(pI2Cx == I2C1)
	{
		I2C1_REG_RESET();
	}
	else if(pI2Cx == I2C2)
	{
		I2C2_REG_RESET();
	}
	else if(pI2Cx == I2C3)
	{
		I2C3_REG_RESET();
	}
}

void I2C_GenerateStart(I2C_RegDef_t  *pI2Cx)
{
	pI2Cx->CR2 |= 1<<I2C_CR2_START;
}
void I2C_GenerateStop(I2C_RegDef_t  *pI2Cx)
{
	pI2Cx->CR2 |= 1<<I2C_CR2_STOP;
}
/*
 * Data Send and Receive
 */
uint32_t I2C_MasterSendData(I2C_RegDef_t *pI2Cx, uint8_t *pTxBuffer, uint32_t len, uint8_t SlaveAddr)
{
    // 1. Wait until bus is not busy
    uint32_t timeout = 100000;
    while(I2C_GetFlagStatus(pI2Cx,I2C_ISR_BUSY));
    // 2. Clear control register and configure transfer
    pI2Cx->CR2 = 0;
    pI2Cx->CR2 |= (SlaveAddr << 1);          // Set slave address
    pI2Cx->CR2 |= (len << 16); // Number of bytes
    pI2Cx->CR2 &= ~(1<<I2C_CR2_RD_WRN);           // Write mode
    I2C_GenerateStart(pI2Cx);         // Generate start

    // 3. Transmit data
    for(uint32_t i = 0; i < len; i++) {
        timeout = 100000;
        while(!(pI2Cx->ISR & I2C_ISR_TXIS) && timeout--) {
            if(timeout == 0) return 1; // Timeout error
        }
        pI2Cx->TXDR = pTxBuffer[i];
    }

    // 4. Wait for transfer complete
    timeout = 100000;
    while(!(pI2Cx->ISR & I2C_ISR_TC) && timeout--) {
        if(timeout == 0) return 1; // Timeout error
    }

    // 5. Generate stop condition
    pI2Cx->CR2 |= (1<<I2C_CR2_STOP);

    // 6. Wait for stop flag and clear it
    timeout = 100000;
    while(!(pI2Cx->ISR & I2C_ISR_STOPF) && timeout--) {
        if(timeout == 0) return 1; // Timeout error
    }
    pI2Cx->ICR |= (1<<I2C_ICR_STOPCF);
    return 0;
}
void I2C_MasterReceiveData(I2C_RegDef_t *pI2Cx, uint8_t *pRxBuffer, uint32_t len,uint8_t SlaveAddr)
{
	//wait until bus is free
	while(I2C_GetFlagStatus(pI2Cx, I2C_FLAG_BUSY));

	//configure CR2 with slave address
	pI2Cx->CR2 = 0;

	pI2Cx->CR2 |= (SlaveAddr<<1);
	pI2Cx->CR2 |= (len<<16);
	pI2Cx->CR2 |= (1<<I2C_CR2_RD_WRN);     // Set read mode
	pI2Cx->CR2 |= (1<<I2C_CR2_AUTOEND);
	I2C_GenerateStart(pI2Cx);


	while(len > 0)
	{
		while(!(I2C_GetFlagStatus(pI2Cx,I2C_FLAG_RXNE)));
		*pRxBuffer++=pI2Cx->RXDR;
		len--;
	}
	while(!(I2C_GetFlagStatus(pI2Cx, I2C_FLAG_STOPF)));
	pI2Cx->ICR |= I2C_ICR_STOPCF;
}
/*
 * Interrupt mode Transmission and Reception
 */

/*
 * IRQ Configuration and ISR handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{

}
uint8_t I2C_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{

}


void I2C_IRQHandling(I2C_RegDef_t *pI2CHandle)
{

}

/*
 * Other Peripheral Control API's
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
		pI2Cx->CR1 |= (1<<I2C_CR1_PE);
	else
		pI2Cx->CR1 &= ~(1<<I2C_CR1_PE);
}

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName)
{
	if((pI2Cx->ISR >> FlagName)&1)
	{
		return SET;
	}
	return RESET;
}

