/*
 * usart_tx.c
 *
 *  Created on: Sep 3, 2025
 *      Author: VINOD
 */

#include "stm32l4xx_uart_driver.h"
#include "stm32l4xx_gpio_driver.h"
#include <string.h>

/*
 * PA2	 -->		UART_TX
 * PA3  -->			UART_RX
 * Baud Rate --> 9600
 */

USART_Handle_t uarthandle;
USART_Handle_t *pUARTHandle = &uarthandle;


void delay(uint32_t n)
{
	uint32_t i,j;
	for(i=0; i<n;i++)
		for(j=0;j<1000;j++);
}

void UART_GPIOInits(void)
{

	GPIO_PeriClockControl(GPIOA, ENABLE);

	GPIO_Handle_t UARTPins;

	UARTPins.pGPIOx = GPIOA;
	UARTPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	UARTPins.GPIO_PinConfig.GPIO_PinAltFunMode = 7;
	UARTPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	UARTPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	UARTPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;

	//UART_Tx
	UARTPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	GPIO_Init(&UARTPins);

	//UART_Rx
	UARTPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
	GPIO_Init(&UARTPins);
}

void uart_init(void)
{
	pUARTHandle->pUSARTx = USART1;
	pUARTHandle->USART_Config.USART_Mode = USART_MODE_TXRX;
	pUARTHandle->USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
	pUARTHandle->USART_Config.USART_Baud = USART_STD_BAUD_9600;
	pUARTHandle->USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	pUARTHandle->USART_Config.USART_NoOfStopBits =  USART_STOPBITS_1;
	pUARTHandle->USART_Config.USART_ParityControl = USART_PARITY_DISABLE;

	USART_Init(pUARTHandle);

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

uint8_t tx_buffer[] = "STM32 Request\r\n";
uint8_t rx_buffer[32];
uint8_t len;

int main(void)
{
	UART_GPIOInits();
	uart_init();
	//btn_init();

	while (1)
	{
		//while (GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == SET);
		delay(100);

		// Send to Arduino
		len = strlen((char*)tx_buffer);
		//USART_SendData(pUARTHandle, tx_buffer, len);
		USART_SendReceive(pUARTHandle,tx_buffer,  rx_buffer, len);

		// Receive from Arduino (expecting fixed response length)
		//USART_ReceiveData(pUARTHandle, rx_buffer, 32);

		delay(1000); // Optional for debug visibility
	}
}



