
#include "spi.h"


/*
 * Peripheral Clock Setup
 */
void SPI_PclkControl(SPI_TypeDef *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pSPIx == SPI1)
			SPI1_PCLK_EN();
		else if(pSPIx == SPI2)
			SPI2_PCLK_EN();
		else if(pSPIx == SPI3)
			SPI3_PCLK_EN();
	}
	else
	{
		if(pSPIx == SPI1)
			SPI1_PCLK_DI();
		else if(pSPIx == SPI2)
			SPI2_PCLK_DI();
		else if(pSPIx == SPI3)
			SPI3_PCLK_DI();
	}
}

/*
 * SPI Init & Deinit
 */

void SPI_Init(SPI_Handle_t *pSPIHandle)
{
    // 1. Enable peripheral clock
    SPI_PclkControl(pSPIHandle->pSPIx, ENABLE);

    // 2. Disable SPI before configuration
    SPI_PeripheralControl(pSPIHandle->pSPIx, DISABLE);

    // 3. Configure CR2 (Data Size & FRXTH) before enabling SPI
    uint32_t tempCR2 = 0;
    tempCR2 |= (0x7 << SPI_CR2_DS_Pos);  // 8-bit data size
    tempCR2 |= SPI_CR2_FRXTH;            // RX FIFO threshold for 8-bit
    pSPIHandle->pSPIx->CR2 = tempCR2;

    // 4. Configure CR1
    uint32_t tempCR1 = 0;
    tempCR1 |= SPI_CR1_MSTR;   // Master mode
    tempCR1 |= (pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR_Pos);  // Clock speed
    tempCR1 |= (pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL_Pos);
    tempCR1 |= (pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA_Pos);
    tempCR1 |= (pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM_Pos);    // Software slave management SSM High
    tempCR1 |= SPI_CR1_SSI;    // NSS high

    // BIDI configuration
    if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
    {
        tempCR1 &= ~(SPI_CR1_BIDIMODE);
    }
    else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
    {
        tempCR1 |= SPI_CR1_BIDIMODE;
    }
    else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
    {
        tempCR1 &= ~(SPI_CR1_BIDIMODE);
        tempCR1 |= SPI_CR1_RXONLY;
    }

    pSPIHandle->pSPIx->CR1 = tempCR1;

    // 5. Enable SPI
    //pSPIHandle->pSPIx->CR1 |= SPI_CR1_SPE;
}

void SPI_Deinit(SPI_TypeDef *pSPIx)
{
	if(pSPIx == SPI1)
		SPI1_REG_RST();
	else if(pSPIx == SPI2)
		SPI2_REG_RST();
	else if(pSPIx == SPI3)
		SPI3_REG_RST();
}

uint8_t SPI_GetFlagStatus(SPI_TypeDef *pSPIx , uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*
 * Data Send and Receive
 */
void SPI_SendData(SPI_TypeDef *pSPIx, uint8_t *pTxBuffer, uint32_t len)
{
	while(len > 0)
	{
		//1. wait until TXE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG)  == FLAG_RESET );

        // Write 8-bit data
        //*((__IO uint8_t*)&pSPIx->DR) = *pTxBuffer;
		pSPIx->DR = *pTxBuffer;

        len--;
        pTxBuffer++;

	}
}
void SPI_ReceiveData(SPI_TypeDef *pSPIx, uint8_t *pRxBuffer, uint32_t len)
{
	while(len > 0)
		{
			//1. wait until RXNE is set
			while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG)  == (uint8_t)FLAG_RESET);
	        // Read 8-bit data
	        //*pRxBuffer = *((__IO uint8_t*)&pSPIx->DR);
			*pRxBuffer = pSPIx->DR;

	        len--;
	        pRxBuffer++;
		}
}

void SPI_TransmitReceive(SPI_TypeDef *pSPIx, uint8_t *pTxBuffer, uint8_t *pRxBuffer, uint32_t len)
{
    while(len > 0)
    {
        // Wait until TXE is set
        while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

        // Write to DR — 8-bit
        *((__IO uint8_t*)&pSPIx->DR) = *pTxBuffer;

        // Wait until RXNE is set
        while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

        // Read from DR — 8-bit
        *pRxBuffer = *((__IO uint8_t*)&pSPIx->DR);

        pTxBuffer++;
        pRxBuffer++;
        len--;
    }
}


void SPI_PeripheralControl(SPI_TypeDef *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |=  SPI_CR1_SPE;
	}else
	{
		pSPIx->CR1 &=  ~SPI_CR1_SPE;
	}
}


void  SPI_SSIConfig(SPI_TypeDef *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |=  (SPI_CR1_SSI);
	}else
	{
		pSPIx->CR1 &=  ~(SPI_CR1_SSI);
	}
}

void  SPI_SSOEConfig(SPI_TypeDef *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR2 |=  (SPI_CR2_SSOE);
	}else
	{
		pSPIx->CR2 &=  ~(SPI_CR2_SSOE);
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
		pSPIHandle->pSPIx->CR2 |= SPI_TXEIE;

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
		pSPIHandle->TxState = SPI_BUSY_IN_RX;

		// Enable RXNEIE interrupt
		pSPIHandle->pSPIx->CR2 |= SPI_RXNEIE;

		return 1;
	}
	return 0;
}


/*
 * IRQ Configuration and ISR handling
 */
//void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);

void SPI_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(IRQNumber < 32)
			NVIC->ISER[0] |= (1 << IRQNumber);
		else if(IRQNumber < 64)
			NVIC->ISER[1] |= (1 << (IRQNumber % 32));

		NVIC_SetPriority(IRQNumber, IRQPriority);
	}
	else
	{
		if(IRQNumber < 32)
			NVIC->ICER[0] |= (1 << IRQNumber);
		else if(IRQNumber < 64)
			NVIC->ICER[1] |= (1 << (IRQNumber % 32));
	}
}

void SPI_IRQHandling(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp1, temp2;

	// Check for TXE
	temp1 = pSPIHandle->pSPIx->SR & SPI_TXE_FLAG;
	temp2 = pSPIHandle->pSPIx->CR2 & SPI_TXEIE;

	if (temp1 && temp2)
	{
		// Handle TXE
		spi_txe_IT_handle(pSPIHandle);
	}

	// Check for RXNE
	temp1 = pSPIHandle->pSPIx->SR & SPI_RXNE_FLAG;
	temp2 = pSPIHandle->pSPIx->CR2 & SPI_RXNEIE;

	if (temp1 && temp2)
	{
		// Handle RXNE
		spi_rxe_IT_handle(pSPIHandle);
	}
}

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
		pSPIHandle->pSPIx->CR2 &= ~SPI_TXEIE;
		pSPIHandle->TxState = SPI_READY;
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
		pSPIHandle->pSPIx->CR2 &= ~SPI_RXNEIE;
		pSPIHandle->TxState = SPI_READY;
	}
}

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv)
{
    if(AppEv == SPI_EVENT_TX_CMPLT)
        spi_tx_done = 1;
    else if(AppEv == SPI_EVENT_RX_CMPLT)
        spi_rx_done = 1;
}





