/*
 * 08_spi_master_stm.c
 *
 *  Created on: Sep 2, 2025
 *      Author: VINOD
 */

#include"stm32l4xx.h"
#include"stm32l4xx_gpio_driver.h"
#include"stm32l4xx_spi_driver.h"

/*
* PA6 --> SPI1_MISO
* PA7 --> SPI1_MOSI
* PA5 -> SPI1_SCLK
* PA4 --> SPI1_NSS
* ALT function mode : 5
*/

void SPI2_GPIOInits(void)
{

	GPIO_PeriClockControl(GPIOA, ENABLE);

	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOA;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&SPIPins);

	//MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&SPIPins);


	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);


}

void SPI2_Inits(void)
{

	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI1;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;//generates sclk of 8MHz
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;

	SPI_Init(&SPI2handle);
}
//uint8_t tx_data = 0xAB;
//uint8_t rx_data = 0x00;
uint8_t tx_data[10] = "hello";
uint8_t rx_data[11]={0};
int main(void)
{

    SPI2_GPIOInits();
    SPI2_Inits();


    /*
     * In SSM mode i.e Slave select pin is controller by software at that SSI must set
     * other wise Master mode will be affected and changes to Slave mode by changing MSTR bit to 0
     */

    //SPI_PeripheralControl(SPI1, ENABLE);

    //SPI_SendData(SPI1, (uint8_t *)&tx_data, 1);
    //SPI_TransmitReceive(SPI1, tx_data, rx_data, 10);
    //while( SPI_GetFlagStatus(SPI1, SPI_BUSY_FLAG) );
    SPI_SendData(SPI1, &tx_data, 5);

    SPI_PeripheralControl(SPI1, DISABLE);

    while(1);
}
