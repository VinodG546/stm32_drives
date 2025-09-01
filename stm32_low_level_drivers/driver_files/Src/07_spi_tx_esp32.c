/*
 * 07_tx_esp32.c
 *
 *  Created on: Aug 28, 2025
 *      Author: VINOD
 */



#include "stm32l4xx_spi_driver.h"
#include"stm32l4xx_gpio_driver.h"
#include <string.h>

/*
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB13 -> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT function mode : 5
 */

void delay(uint32_t n)
{
	uint32_t i,j;
	for(i=0; i<n;i++)
		for(j=0;j<1000;j++);
}

void SPI2_GPIOInits(void)
{

	GPIO_PeriClockControl(GPIOB, ENABLE);

	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//MISO
	//SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	//GPIO_Init(&SPIPins);


	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);


}

void SPI2_Inits(void)
{

	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	//SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV256;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;
	SPI2handle.SPIConfig.SPI_DataSize = SPI_DATASIZE_8BIT;
	SPI_Init(&SPI2handle);
}

void btn_init(void)
{
	GPIO_Handle_t pGPIOBtn;

	GPIO_PeriClockControl(GPIOC, ENABLE);


	pGPIOBtn.pGPIOx = GPIOC;
	pGPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	pGPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	pGPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;
	pGPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&pGPIOBtn);
}

//uint8_t tx_data = 0xAB;
//uint8_t rx_data = 0x00;
uint8_t tx_data[] = {10,20,30,40,50,60,70,80,90,100};
uint8_t rx_data[10];

char data[] = "Hello all";
int main(void)
{

    SPI2_GPIOInits();
    btn_init();
    SPI2_Inits();
    /*
     * Making SSOE 1 does NSS output enable
     * The NSS pin is automatically managed by the hardware
     * i.e When SPE = 1, NSS will be pulled to low
     * and if SPE = 0, the NSS will be pulled high automatically
     */
    SPI_SSOEConfig(SPI2, ENABLE);

    while(1)
    {
    	//Waits until button is pressed
    	while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == SET);

    	delay(500);
    	//Enable SPI2 Peripheral
		SPI_PeripheralControl(SPI2, ENABLE);

		//uint8_t data_len = strlen(data);
		//SPI_SendData(SPI2, &data_len, 1);	//Transmitting length first

		//delay(200);
		//Transmit data
		//SPI_SendData(SPI2, (uint8_t *)data, data_len);
		//SPI_SendData(SPI2, tx_data, 10);
		SPI_TransmitReceive(SPI2, tx_data, rx_data, 10);


		//Confirm SPI is not busy
		while( SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG) );

		SPI_PeripheralControl(SPI2, DISABLE);
    }

}




