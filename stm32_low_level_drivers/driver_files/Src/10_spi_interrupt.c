/*
 * 10_spi_interrupt.c
 *
 *  Created on: Sep 2, 2025
 *      Author: VINOD
 */
#include "stm32l4xx.h"
#include "stm32l4xx_gpio_driver.h"
#include "stm32l4xx_spi_driver.h"

#include <string.h>

SPI_Handle_t SPI2handle;

uint8_t tx_data[] ="BS" ;
uint8_t rx_data[10];

volatile uint8_t spi_tx_done = 0;
volatile uint8_t spi_rx_done = 0;

void SPI2_GPIOInits(void)
{
    GPIO_PeriClockControl(GPIOB, ENABLE);

    GPIO_Handle_t SPIPins;
    SPIPins.pGPIOx = GPIOB;
    SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;  // AF5 for SPI2
    SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;

    // SCLK -> PB13
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIO_Init(&SPIPins);

    // MOSI -> PB15
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
    GPIO_Init(&SPIPins);

    // MISO -> PB14
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
    GPIO_Init(&SPIPins);

    // NSS -> PB12 (optional, not used in loopback)
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GPIO_Init(&SPIPins);
}

void SPI2_Inits(void)
{
    SPI2handle.pSPIx = SPI2;   //  use SPI2
    SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
    SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;   // Software slave select
    SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;

    SPI_Init(&SPI2handle);
}

int main(void)
{
    // Init SPI pins and peripheral
    SPI2_GPIOInits();
    SPI2_Inits();

    // Enable SPI interrupt in NVIC
    SPI_IRQ_interrupt_Config(IRQ_NO_SPI2, ENABLE);
    SPI_IRQ_priority_Config(IRQ_NO_SPI2, 1);

    // Enable SPI
    SPI_PeripheralControl(SPI2, ENABLE);

    // Start loopback test
    SPI_TransmitReceive_IT(&SPI2handle, tx_data, rx_data, sizeof(rx_data));

    while (!(spi_tx_done || spi_rx_done));

    // Now rx_data[] should contain same as tx_data[]
    while(1);
}

/* Application Callback */
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv)
{
    if(AppEv == SPI_EVENT_TX_CMPLT)
    {
        spi_tx_done = 1;
    }
    else if(AppEv == SPI_EVENT_RX_CMPLT)
    {
        spi_rx_done = 1;
    }
    else if (AppEv == SPI_EVENT_TXRX_CMPLT)
   {
	   spi_tx_done = 1;
	   spi_rx_done = 1;
   }
}

/* IRQ Handler */
void SPI2_IRQHandler(void)
{
    SPI_IRQHandling(&SPI2handle);
}
