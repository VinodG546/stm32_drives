/*
 * 07_spi_tx.c
 *
 *  Created on: Aug 26, 2025
 *      Author: VINOD
 */

#include "spi.h"

/*
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB13 -> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT function mode : 5
 */

void SPI2_GPIOInits(void)
{

	GPIO_PclkControl(GPIOB, ENABLE);

	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_AFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);


	//NSS
	//SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	//GPIO_Init(&SPIPins);


}

void SPI2_Inits(void)
{

	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	//SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;//generates sclk of 8MHz
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;

	SPI_Init(&SPI2handle);
}
//uint8_t tx_data = 0xAB;
//uint8_t rx_data = 0x00;
uint8_t tx_data[] = {10,20,30,40,50,60,70,80,90,100};
uint8_t rx_data[10];
int main(void)
{

    SPI2_GPIOInits();
    SPI2_Inits();
    /*
     * In SSM mode i.e Slave select pin is controller by software at that SSI must set
     * other wise Master mode will be affected and changes to Slave mode by changing MSTR bit to 0
     */
    SPI_SSIConfig(SPI2, ENABLE);	//Enables SSI so that NSS will be high and avoid MODF error
    SPI_PeripheralControl(SPI2, ENABLE);

    //SPI_SendData(SPI2, tx_data, 10);
    SPI_TransmitReceive(SPI2, tx_data, rx_data, 10);
    while( SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG) );
    //SPI_ReceiveData(SPI2, rx_data, 10);

    SPI_PeripheralControl(SPI2, DISABLE);

    while(1);
}






