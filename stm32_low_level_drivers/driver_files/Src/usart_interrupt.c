/*
 * usart_interrupt.c
 *
 *  Created on: Sep 3, 2025
 *      Author: VINOD
 */
#include "stm32l4xx_uart_driver.h"
#include "stm32l4xx_gpio_driver.h"
#include <string.h>

/*
 * PA9  --> USART1_TX
 * PA10 --> USART1_RX
 * Baud Rate --> 9600
 */

USART_Handle_t uarthandle;
USART_Handle_t *pUARTHandle = &uarthandle;

uint8_t tx_buffer[] = "STM32 Interrupt Loopback Test\r\n";
uint8_t rx_buffer[32];
uint8_t txDone = 0;
uint8_t rxDone = 0;

void delay(uint32_t n)
{
    uint32_t i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 1000; j++);
}

void UART_GPIOInits(void)
{
    GPIO_PeriClockControl(GPIOA, ENABLE);

    GPIO_Handle_t UARTPins;
    UARTPins.pGPIOx = GPIOA;
    UARTPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    UARTPins.GPIO_PinConfig.GPIO_PinAltFunMode = 7; // AF7 = USART1
    UARTPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    UARTPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    UARTPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;

    // TX (PA9)
    UARTPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
    GPIO_Init(&UARTPins);

    // RX (PA10)
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
    pUARTHandle->USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
    pUARTHandle->USART_Config.USART_ParityControl = USART_PARITY_DISABLE;

    USART_Init(pUARTHandle);
}

int main(void)
{
    UART_GPIOInits();
    uart_init();

    // Enable USART1 interrupt in NVIC
    USART_IRQConfig(IRQ_NO_USART1, 1, ENABLE);

    while (1)
    {
        txDone = 0;
        rxDone = 0;

        // Start interrupt-driven TX and RX
        USART_SendDataIT(pUARTHandle, tx_buffer, strlen((char *)tx_buffer));
        USART_ReceiveDataIT(pUARTHandle, rx_buffer, sizeof(rx_buffer));

        // Wait until both complete
        while (!(txDone && rxDone));

        delay(1000); // Slow down loop for readability
    }
}

/* IRQ Handler for USART1 */
void USART1_IRQHandler(void)
{
    USART_IRQHandling(pUARTHandle);
}

/* Application callback */
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t AppEv)
{
    if (AppEv == USART_EVENT_TX_CMPLT)
    {
        txDone = 1; // Transmission done
    }
    else if (AppEv == USART_EVENT_RX_CMPLT)
    {
        rxDone = 1; // Reception done
        // Optionally, send back what was received
        USART_SendDataIT(pUSARTHandle, rx_buffer, strlen((char *)rx_buffer));
    }
}
