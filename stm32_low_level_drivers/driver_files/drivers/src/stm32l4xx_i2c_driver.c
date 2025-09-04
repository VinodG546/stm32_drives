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
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed == I2C_SCL_SPEED_SM) {
		pI2CHandle->pI2Cx->TIMINGR = 0x10420A28; // Standard mode
	} else {
		pI2CHandle->pI2Cx->TIMINGR = 0x00B1112E; // Fast mode (example value)
	}
	//configure own address

	pI2CHandle->pI2Cx->OAR1 &= ~(1<<I2c_OAR1_OA1EN);//disable EN bit first
	pI2CHandle->pI2Cx->OAR1 |= (0<<10) | (pI2CHandle->I2C_Config.I2C_DeviceAddress<<1);
	pI2CHandle->pI2Cx->OAR1 |= (1<<I2c_OAR1_OA1EN);

	//enable ACK will be used only for slave mode
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE)
	    pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_NACK);  // clear NACK -> allow ACK
	else
	    pI2CHandle->pI2Cx->CR2 |=  (1 << I2C_CR2_NACK);  // set NACK -> disable ACK

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
	pI2Cx->CR2 |= (1<<I2C_CR2_AUTOEND);

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
uint8_t I2C_MasterSendData_IT(I2C_Handle_t *pI2CHandle,uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr)
{
	if(pI2CHandle->TxRxState != I2C_READY)
		return I2C_BUSY;

	pI2CHandle->pTxBuffer = pTxBuffer;
	pI2CHandle->TxLen = Len;
	pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
	pI2CHandle->DevAddr = SlaveAddr;

	// 1. Configure CR2
	pI2CHandle->pI2Cx->CR2 = 0;
	pI2CHandle->pI2Cx->CR2 |= (SlaveAddr << 1);
	pI2CHandle->pI2Cx->CR2 |= (Len << 16);
	//pI2CHandle->pI2Cx->CR2 |= (1<<I2C_CR2_AUTOEND);

	pI2CHandle->pI2Cx->CR2 |= 1 << I2C_CR2_START;

	// 2. Enable TXIE and STOPIE
	pI2CHandle->pI2Cx->CR1 |= (1<<I2C_CR1_TXIE) | (1<<I2C_CR1_STOPIE) | (1<<I2C_CR1_ERRIE) | (1<<I2C_CR1_TCIE);

	return 0;
}

uint8_t I2C_MasterReceiveData_IT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr)
{
	if(pI2CHandle->TxRxState != I2C_READY)
		return I2C_BUSY;

	pI2CHandle->pRxBuffer = pRxBuffer;
	pI2CHandle->RxLen = Len;
	pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
	pI2CHandle->DevAddr = SlaveAddr;

	// 1. Configure CR2
	pI2CHandle->pI2Cx->CR2 = 0;
	pI2CHandle->pI2Cx->CR2 |= (SlaveAddr << 1);
	pI2CHandle->pI2Cx->CR2 |= (Len << 16);
	pI2CHandle->pI2Cx->CR2 |= (1<<I2C_CR2_RD_WRN);
	pI2CHandle->pI2Cx->CR2 |= (1<<I2C_CR2_AUTOEND);

	pI2CHandle->pI2Cx->CR2 |= 1 << I2C_CR2_START;

	// 2. Enable RXIE and STOPIE
	pI2CHandle->pI2Cx->CR1 |= (1<<I2C_CR1_RXIE) | (1<<I2C_CR1_STOPIE) | (1<<I2C_CR1_ERRIE);

	return 0;
}

/*
 * IRQ Configuration and ISR handling
 */
void I2C_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31){
			*NVIC_ISER0 |= (1<<IRQNumber);
		}
		else if(IRQNumber >=32 && IRQNumber < 64){
			*NVIC_ISER1 |= (1<<(IRQNumber%32));
		}
		else if(IRQNumber >=64 && IRQNumber < 96){
			*NVIC_ISER2 |= (1<<(IRQNumber%32));
		}
	}
	else
	{
		if(IRQNumber <= 31){
			*NVIC_ICER0 |= (1<<IRQNumber);
		}
		else if(IRQNumber >=32 && IRQNumber < 64){
			*NVIC_ICER1 |= (1<<(IRQNumber%32));
		}
		else if(IRQNumber >=64 && IRQNumber < 96){
			*NVIC_ICER2 |= (1<<(IRQNumber%32));
		}
	}
}

void I2C_IRQ_priority_Config(uint8_t IRQNumber,uint8_t priority){
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	uint8_t shift_amount  = (8*iprx_section) + (8-NO_PR_BITS_IMPLEMENTED);

	*((volatile uint32_t*)(NVIC_PR_BASE_ADDR+(iprx*4))) |= (priority << shift_amount);

}

void I2C_IRQHandling(I2C_Handle_t *pI2CHandle)
{
    uint32_t temp1, temp2;
    uint8_t event = I2C_EVENT_ERROR; // Initialize with error

    // Handle TXIS (Transmit Interrupt Status)
    temp1 = (pI2CHandle->pI2Cx->CR1 & (1 << I2C_CR1_TXIE));
    temp2 = (pI2CHandle->pI2Cx->ISR & (1 << I2C_ISR_TXIS));
    if(temp1 && temp2)
    {
        // Check if there's more data to send
        if(pI2CHandle->TxLen > 0)
        {
            pI2CHandle->pI2Cx->TXDR = *(pI2CHandle->pTxBuffer++);
            pI2CHandle->TxLen--;

            // If this was the last byte, disable TXIE
            if(pI2CHandle->TxLen == 0)
            {
                pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_TXIE);
            }
        }
    }

    // Handle RXNE (Receive Data Register Not Empty)
    temp1 = (pI2CHandle->pI2Cx->CR1 & (1 << I2C_CR1_RXIE));
    temp2 = (pI2CHandle->pI2Cx->ISR & (1 << I2C_ISR_RXNE));
    if(temp1 && temp2)
    {
        // Check if we're expecting more data
        if(pI2CHandle->RxLen > 0)
        {
            *(pI2CHandle->pRxBuffer++) = pI2CHandle->pI2Cx->RXDR;
            pI2CHandle->RxLen--;

            // If this was the last byte, disable RXIE
            if(pI2CHandle->RxLen == 0)
            {
                pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_RXIE);
            }
        }
    }

    // Handle TC (Transfer Complete) - Important for the end of transmission
    temp1 = (pI2CHandle->pI2Cx->CR1 & (1 << I2C_CR1_TCIE));
    temp2 = (pI2CHandle->pI2Cx->ISR & (1 << I2C_ISR_TC));
    if(temp1 && temp2)
    {
        // Transfer complete, but we might not have STOP condition yet
        // You might want to handle this depending on your use case
    }

    // Handle STOPF (Stop Detection Flag)
    temp1 = (pI2CHandle->pI2Cx->CR1 & (1 << I2C_CR1_STOPIE));
    temp2 = (pI2CHandle->pI2Cx->ISR & (1 << I2C_ISR_STOPF));
    if(temp1 && temp2)
    {
        // Clear STOP flag
        pI2CHandle->pI2Cx->ICR |= (1 << I2C_ICR_STOPCF);

        // Determine which event completed
        if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
        {
            event = I2C_EV_TX_CMPLT;
        }
        else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
        {
            event = I2C_EV_RX_CMPLT;
        }

        // Reset state
        pI2CHandle->TxRxState = I2C_READY;

        // Notify application - do this BEFORE clearing buffers
        if(pI2CHandle->ApplicationCallback)
        {
            pI2CHandle->ApplicationCallback(pI2CHandle, event);
        }

        // Clear buffers after callback (in case callback needs them)
        pI2CHandle->pTxBuffer = NULL;
        pI2CHandle->pRxBuffer = NULL;
        pI2CHandle->TxLen = 0;
        pI2CHandle->RxLen = 0;
    }
    if ((pI2CHandle->pI2Cx->CR1 & (1 << I2C_CR1_TCIE)) &&
        (pI2CHandle->pI2Cx->ISR & (1 << I2C_ISR_TC))) {

        if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX && pI2CHandle->TxLen == 0) {
            pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_STOP);  // generate STOP
            // (STOPF interrupt you already handle will fire and call the app callback)
        }
    }
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

