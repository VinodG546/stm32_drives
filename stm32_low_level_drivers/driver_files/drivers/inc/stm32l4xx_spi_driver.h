/*
 * stm32l4xx_spi_driver.h
 *
 *  Created on: Aug 26, 2025
 *      Author: VINOD
 */
/*
 * stm32l4xx_spi_driver.h
 */

#ifndef INC_STM32L4XX_SPI_DRIVER_H_
#define INC_STM32L4XX_SPI_DRIVER_H_

#include "stm32l4xx.h"

typedef struct {
    uint8_t SPI_DeviceMode;
    uint8_t SPI_BusConfig;
    uint8_t SPI_SclkSpeed;
    uint8_t SPI_DataSize;
    uint8_t SPI_CPOL;
    uint8_t SPI_CPHA;
    uint8_t SPI_SSM;
} SPI_Config_t;

typedef struct {
    SPI_RegDef_t *pSPIx;
    SPI_Config_t SPIConfig;
} SPI_Handle_t;

#define SPI_DEVICE_MODE_MASTER   1
#define SPI_DEVICE_MODE_SLAVE    0

#define SPI_BUS_CONFIG_FD        1
#define SPI_BUS_CONFIG_HD        2
#define SPI_BUS_CONFIG_SIMPLEX_TXONLY 3

#define SPI_SCLK_SPEED_DIV2      0
#define SPI_SCLK_SPEED_DIV4      1
#define SPI_SCLK_SPEED_DIV8      2
#define SPI_SCLK_SPEED_DIV16     3
#define SPI_SCLK_SPEED_DIV32     4
#define SPI_SCLK_SPEED_DIV64     5
#define SPI_SCLK_SPEED_DIV128    6
#define SPI_SCLK_SPEED_DIV256    7

// DS field values (CR2[11:8])
#define SPI_DATASIZE_8BIT        0x7
#define SPI_DATASIZE_16BIT       0xF

#define SPI_CPOL_HIGH            1
#define SPI_CPOL_LOW             0

#define SPI_CPHA_HIGH            1
#define SPI_CPHA_LOW             0

#define SPI_SSM_DI               0
#define SPI_SSM_EN               1

#define SPI_TXE_FLAG             (1 << SPI_SR_TXE)
#define SPI_RXNE_FLAG            (1 << SPI_SR_RXNE)
#define SPI_BUSY_FLAG            (1 << SPI_SR_BSY)

// ✅ Correct DS bit position in CR2
#define SPI_CR2_DS               8

// Peripheral clock setup
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

// Init and deinit SPI
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

// Data send and receive
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len);
void SPI_RecieveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len);

// APIs for IRQ configuration and handling
void SPI_IRQ_interrupt_Config(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQ_priority_Config(uint8_t IRQNumber, uint8_t priority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);
void  SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx,uint8_t EnorDi);
void SPI_TransmitReceive(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint8_t *pRxBuffer, uint32_t len);
#endif /* INC_STM32L4XX_SPI_DRIVER_H_ */
