/*
 * uart.c
 *
 *  Created on: Sep 9, 2025
 *      Author: VINOD
 */
#include "uart.h"

#define GPIOAEN     (1U << 0)
#define USART2EN    (1U << 17)

#define SYS_FREQ        16000000
#define UART_BAUDRATE   115200

#define CR1_TE      (1U << 3)
#define CR1_RE      (1U << 2)
#define CR1_UE      (1U << 0)

#define ISR_TXE     (1U << 7)
#define ISR_RXNE    (1U << 5)

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);


int __io_putchar(int ch)
{
    uart2_write(ch);
    return ch;
}

int __io_getchar(void)
{
    return uart2_read();
}

void USART2_tx_rx_init(void)
{
    /*********** Enable HSI16 ***********/
    RCC->CR |= RCC_CR_HSION;              // Enable HSI16
    while (!(RCC->CR & RCC_CR_HSIRDY));   // Wait until HSI16 ready

    /*********** Configure UART2 GPIO pins ***********/
    RCC->AHB2ENR |= GPIOAEN;              // Enable GPIOA clock

    // PA2 = USART2_TX (AF7)
    GPIOA->MODER &= ~(3U << 4);   // clear MODER2[1:0]
    GPIOA->MODER |=  (2U << 4);   // set to AF
    GPIOA->AFR[0] &= ~(0xFU << 8);
    GPIOA->AFR[0] |=  (7U  << 8);

    // PA3 = USART2_RX (AF7)
    GPIOA->MODER &= ~(3U << 6);   // clear MODER3[1:0]
    GPIOA->MODER |=  (2U << 6);   // set to AF
    GPIOA->AFR[0] &= ~(0xFU << 12);
    GPIOA->AFR[0] |=  (7U  << 12);

    /************* Configure USART2 *************/
    RCC->APB1ENR1 |= USART2EN;            // Enable USART2 clock

    // Select HSI16 (16 MHz) as USART2 clock source
    RCC->CCIPR &= ~(3U << 2);   // clear USART2SEL[1:0]
    RCC->CCIPR |=  (2U << 2);   // 10: HSI16 selected

    // Configure baudrate
    uart_set_baudrate(USART2, SYS_FREQ, UART_BAUDRATE);

    // Enable transmitter and receiver
    USART2->CR1 |= (CR1_TE | CR1_RE);

    // Enable USART2
    USART2->CR1 |= CR1_UE;
}

void uart2_write(int ch)
{
    while (!(USART2->ISR & ISR_TXE));  // Wait until TX buffer empty
    USART2->TDR = (ch & 0xFF);
}

char uart2_read(void)
{
    while (!(USART2->ISR & ISR_RXNE));  // Wait until RXNE (data ready)
    return (char)(USART2->RDR & 0xFF);
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
    USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
    return ((PeriphClk) + (BaudRate / 2U)) / BaudRate;
}
