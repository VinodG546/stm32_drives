/*
 * 06_spi_tx_file.c
 *
 *  Created on: Aug 26, 2025
 */
#include "stm32l4xx.h"
#include "stm32l4xx_gpio_driver.h"
#include "stm32l4xx_spi_driver.h"
#include <string.h>

/*
 * SPI2 Pins:
 *  PB13 -> SPI2_SCK
 *  PB14 -> SPI2_MISO
 *  PB15 -> SPI2_MOSI
 *  PB12 -> SPI2_NSS
 */

void SPI2_GPIOInits(void)
{

    GPIO_Handle_t SPIPins;

    GPIO_PeriClockControl(GPIOB, ENABLE);

    SPIPins.pGPIOx = GPIOB;
    SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;  // AF5 = SPI2
    SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;

    // SCK
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIO_Init(&SPIPins);

    // MOSI
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
    GPIO_Init(&SPIPins);

    // MISO (optional, not used in TX-only)
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
    GPIO_Init(&SPIPins);

    // NSS (optional, if using hardware slave select)
    //SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    //GPIO_Init(&SPIPins);

}

void SPI2_Inits(void)
{
    SPI_Handle_t SPI2Handle;
    SPI_PeriClockControl(SPI2, ENABLE);
    SPI2Handle.pSPIx = SPI2;
    SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;  // fPCLK/8
    SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_EN;   // Software slave management
    SPI2Handle.SPIConfig.SPI_DataSize = SPI_DATASIZE_8BIT; //use DS field

    SPI_Init(&SPI2Handle);
}

uint8_t tx_data[] = "Hello STM32L476 SPI";
uint8_t rx_data[20];
int main(void)
{


    // 1. Init GPIO pins for SPI2
    SPI2_GPIOInits();

    // 2. Init SPI2 peripheral
    SPI2_Inits();

    // 3. Enable SPI2 peripheral
    SPI_PeripheralControl(SPI2,ENABLE);

    // 4. Send data
    SPI_TransmitReceive(SPI2,tx_data,rx_data,19);

    // 5. Wait until SPI is not busy
    while (SPI_GetFlagStatus(SPI2, SPI_SR_BSY));

    // 6. Disable SPI
    SPI_PeripheralControl(SPI2,DISABLE);

    while (1);
}
