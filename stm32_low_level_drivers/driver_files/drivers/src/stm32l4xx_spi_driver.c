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
	if (pSPIx->CR1 & (1 << SPI_CR1_SSM)) {
	        pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	    }
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
		pSPIx->CR2 |=  (SPI_CR2_SSOE);
	}else
	{
		pSPIx->CR2 &=  ~(SPI_CR2_SSOE);
	}
}
// IRQ config and handling (skeletons)
void SPI_IRQ_interrupt_Config(uint8_t IRQNumber, uint8_t EnorDi)
{
    // TODO: configure NVIC
}

void SPI_IRQ_priority_Config(uint8_t IRQNumber, uint8_t priority)
{
    // TODO: set NVIC priority
}

void SPI_IRQHandling(SPI_Handle_t *pHandle)
{
    // TODO: handle SPI interrupts
}
