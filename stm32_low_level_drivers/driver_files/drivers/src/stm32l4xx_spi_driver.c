/*
 * stm32l4xx_spi_driver.c
 *
 *  Created on: Aug 26, 2025
 *      Author: VINOD
 */
/*
 * stm32l4xx_spi_driver.c (fixed)
 */
#include "stm32l4xx_spi_driver.h"

// Peripheral clock setup
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
    if (EnorDi == ENABLE) {
        if (pSPIx == SPI1) {
            SPI1_PCLK_EN();
        } else if (pSPIx == SPI2) {
            SPI2_PCLK_EN();
        } else if (pSPIx == SPI3) {
            SPI3_PCLK_EN();
        }
    } else {

    }
}

// Init and deinit SPI
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	SPI_PeriClockControl(pSPIHandle->pSPIx,ENABLE);
    uint32_t tempreg = 0;
    SPI_PeripheralControl(pSPIHandle->pSPIx, DISABLE);

    // Configure device mode
    tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

    // Configure bus config
    if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD) {
        tempreg &= ~(1 << SPI_CR1_BIDIMODE);
    } else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD) {
        tempreg |= (1 << SPI_CR1_BIDIMODE);
    } else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_TXONLY) {
        tempreg &= ~(1 << SPI_CR1_BIDIMODE);
        tempreg |= (1 << SPI_CR1_RXONLY);
    }

    // Configure clock speed
    tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

    // Configure CPOL & CPHA
    tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;
    tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

    tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;
    // Write to CR1
    pSPIHandle->pSPIx->CR1 = tempreg;

    // Configure data size in CR2
    pSPIHandle->pSPIx->CR2 &= ~(0xF << SPI_CR2_DS);
    pSPIHandle->pSPIx->CR2 |= (pSPIHandle->SPIConfig.SPI_DataSize << SPI_CR2_DS);

    if(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_SSM)) {
    	        pSPIHandle->pSPIx->CR1 |= (1 << SPI_CR1_SSI);
    }

    //SPI_PeripheralControl(pSPIHandle->pSPIx , ENABLE);
}

void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
    if (pSPIx == SPI1) {
        SPI1_REG_RESET();
    } else if (pSPIx == SPI2) {
        SPI2_REG_RESET();
    } else if (pSPIx == SPI3) {
        SPI3_REG_RESET();
    }
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
    if (pSPIx->SR & FlagName) {
        return FLAG_SET;
    }
    return FLAG_RESET;
}

// Data send
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len)
{
	if (pSPIx->CR1 & (1 << SPI_CR1_SSM)) {
	        pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	    }
    while (len > 0) {
        // Wait until TXE is set
        while (SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == RESET);

        uint8_t dsize = ((pSPIx->CR2 >> SPI_CR2_DS) & 0xF);

        if (dsize == SPI_DATASIZE_16BIT) {
            // 16-bit transfer
            *((volatile uint16_t *)&pSPIx->DR) = *((uint16_t *)pTxBuffer);
            pTxBuffer += 2;
            len -= 2;
        } else {
            // 8-bit transfer
            *((volatile uint8_t *)&pSPIx->DR) = *pTxBuffer;
            pTxBuffer++;
            len--;
        }
    }
}

void SPI_RecieveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len)
{
    while (len > 0) {
        // Wait until RXNE is set
        while (SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == RESET);

        uint8_t dsize = ((pSPIx->CR2 >> SPI_CR2_DS) & 0xF);

        if (dsize == SPI_DATASIZE_16BIT) {
            *((uint16_t *)pRxBuffer) = *((volatile uint16_t *)&pSPIx->DR);
            pRxBuffer += 2;
            len -= 2;
        } else {
            *pRxBuffer = *((volatile uint8_t *)&pSPIx->DR);
            pRxBuffer++;
            len--;
        }
    }
}
void SPI_TransmitReceive(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint8_t *pRxBuffer, uint32_t len)
{

    while(len > 0)
    {
        // Wait until TXE is set
        while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

        // Write to DR — 8-bit
        *((volatile uint8_t*)&pSPIx->DR) = *pTxBuffer;

        // Wait until RXNE is set
        while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

        // Read from DR — 8-bit
        *pRxBuffer = *((volatile uint8_t*)&pSPIx->DR);

        pTxBuffer++;
        pRxBuffer++;
        len--;
    }
}

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx,uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR1  |= (1 << SPI_CR1_SPE);
	}
	else
	{
		pSPIx->CR1  &= ~(1 << SPI_CR1_SPE);
	}
}

void  SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR2 |=  (1<<SPI_CR2_SSOE);
	}else
	{
		pSPIx->CR2 &=  ~(1<<SPI_CR2_SSOE);
	}
}
/*
 * Interrupt mode Transmission and Reception
 */
uint8_t SPI_SendData_IT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len)
{
	if (pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		//1. Save the TX Buffer address and Len information in some global variable
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = len;
		//2. Mark SPI state as busy in Transmission so that no other code can take over same SPI Peripheral until transmission i sover
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		//3. Enable TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1<<SPI_CR2_TXEIE);

		return 1;
	}
	return 0;
}

uint8_t SPI_ReceiveData_IT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len)
{
	if (pSPIHandle->TxState != SPI_BUSY_IN_RX)
	{
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = len;
		pSPIHandle->RxState = SPI_BUSY_IN_RX;

		// Enable RXNEIE interrupt
		pSPIHandle->pSPIx->CR2 |= (1<<SPI_CR2_RXNEIE);

		return 1;
	}
	return 0;
}

uint8_t SPI_TransmitReceive_IT(SPI_Handle_t *pSPIHandle,
                               uint8_t *pTxBuffer,
                               uint8_t *pRxBuffer,
                               uint32_t len)
{
    if (pSPIHandle->TxRxState != SPI_BUSY_IN_TXRX)
    {
        // Save buffers
        pSPIHandle->pTxBuffer = pTxBuffer;
        pSPIHandle->pRxBuffer = pRxBuffer;
        pSPIHandle->TxLen = len;
        pSPIHandle->RxLen = len;

        // Mark busy
        pSPIHandle->TxRxState = SPI_BUSY_IN_TXRX;

        // Enable TXEIE and RXNEIE
        pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);
        pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);

        return 1;
    }
    return 0;
}

#if 0
uint8_t SPI_TransmitReceive_IT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint8_t *pRxBuffer, uint32_t len)
{
    if (pSPIHandle->TxState != SPI_BUSY_IN_TX && pSPIHandle->RxState != SPI_BUSY_IN_RX)
    {
        // Save buffers
        pSPIHandle->pTxBuffer = pTxBuffer;
        pSPIHandle->pRxBuffer = pRxBuffer;
        pSPIHandle->TxLen = len;
        pSPIHandle->RxLen = len;

        // Mark busy

        pSPIHandle->TxRxState = SPI_BUSY_IN_TXRX;

        // Enable TXEIE and RXNEIE
        pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);
        pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);

        return 1;
    }
    return 0;
}
#endif
//it send and receive
void spi_txe_IT_handle(SPI_Handle_t *pSPIHandle)
{
	if (pSPIHandle->TxLen > 0)
	{
		pSPIHandle->pSPIx->DR = *(pSPIHandle->pTxBuffer);
		pSPIHandle->pTxBuffer++;
		pSPIHandle->TxLen--;
	}
	if (pSPIHandle->TxLen == 0)
	{
		// Tx done
		pSPIHandle->pSPIx->CR2 &= ~(1<<SPI_CR2_TXEIE);
		pSPIHandle->TxState = SPI_READY;
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
	}
	if ((pSPIHandle->TxLen == 0) && (pSPIHandle->RxLen == 0))
	{
		pSPIHandle->TxState = SPI_READY;
		pSPIHandle->RxState = SPI_READY;
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TXRX_CMPLT);
	}
}

void spi_rxe_IT_handle(SPI_Handle_t *pSPIHandle)
{
	if (pSPIHandle->RxLen > 0)
	{
		*(pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR;
		pSPIHandle->pRxBuffer++;
		pSPIHandle->RxLen--;
	}
	if (pSPIHandle->RxLen == 0)
	{
		// Rx done
		pSPIHandle->pSPIx->CR2 &= ~SPI_CR2_RXNEIE;
		pSPIHandle->TxState = SPI_READY;
	    SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
	}
}
// IRQ config and handling (skeletons)
void SPI_IRQ_interrupt_Config(uint8_t IRQNumber, uint8_t EnorDi)
{
    // TODO: configure NVIC
	if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31){
			*NVIC_ISER0 |= (1<<IRQNumber);
		}
		else if(IRQNumber >=32 && IRQNumber < 64){
			*NVIC_ISER1 |= (1<<(IRQNumber%32));
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
	}
}

void SPI_IRQ_priority_Config(uint8_t IRQNumber, uint8_t priority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	uint8_t shift_amount  = (8*iprx_section) + (8-NO_PR_BITS_IMPLEMENTED);

	*((NVIC_PR_BASE_ADDR+(iprx*4))) |= (priority << shift_amount);

}
#if 0
uint8_t ch;
void SPI_IRQHandling(SPI_Handle_t *pHandle)
{
    uint8_t temp1, temp2;

    // -------- Full-Duplex TX/RX --------
    // TXE flag set?
    temp1 = (pHandle->pSPIx->SR & SPI_TXE_FLAG);
    temp2 = (pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE));

    if (temp1 && temp2 && pHandle->TxRxState == SPI_BUSY_IN_TXRX)
    {
        // Write next byte to DR
        pHandle->pSPIx->DR = *(pHandle->pTxBuffer);
        pHandle->pTxBuffer++;
        pHandle->TxLen--;

        if (pHandle->TxLen == 0)
        {
            // All bytes written → disable TXE interrupt
            pHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
        }
    }

    // RXNE flag set?
    temp1 = (pHandle->pSPIx->SR & SPI_RXNE_FLAG);
    temp2 = (pHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE));

    if (temp1 && temp2 && pHandle->TxRxState == SPI_BUSY_IN_TXRX)
    {
        // Read next byte from DR
        *(pHandle->pRxBuffer) = pHandle->pSPIx->DR;
        pHandle->pRxBuffer++;
        pHandle->RxLen--;

        if (pHandle->RxLen == 0)
        {
            // All bytes received → disable RXNE interrupt
            pHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
            pHandle->TxRxState = SPI_READY;

            // Application callback
            SPI_ApplicationEventCallback(pHandle, SPI_EVENT_TXRX_CMPLT);
        }
    }
}
#endif
void SPI_IRQHandling(SPI_Handle_t *pHandle)
{
    uint32_t temp1, temp2;

    // Handle TXE interrupt
    temp1 = pHandle->pSPIx->SR & SPI_TXE_FLAG;
    temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

    if (temp1 && temp2)
    {
        spi_txe_IT_handle(pHandle);
    }

    // Handle RXNE interrupt
    temp1 = pHandle->pSPIx->SR & SPI_RXNE_FLAG;
    temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

    if (temp1 && temp2)
    {
        spi_rxe_IT_handle(pHandle);
    }


}
