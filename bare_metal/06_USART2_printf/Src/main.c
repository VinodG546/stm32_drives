#include "stm32l4xx.h"
#include <stdio.h>

#define GPIOAEN     (1U << 0)
#define USART2EN    (1U << 17)

#define SYS_FREQ    16000000
#define UART_BAUDRATE 115200

#define CR1_TE      (1U << 3)
#define CR1_UE      (1U << 0)
#define ISR_TXE     (1U << 7)

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
void uart2_write(int ch);
void USART2_tx_init(void);

int __io_putchar(int ch)
{
    uart2_write(ch);
    return ch;
}

int main(void)
{
    USART2_tx_init();

    while(1)
    {
        printf("HELLO FROM STM32...\r\n");
        for (volatile int i = 0; i < 1000000; i++); // crude delay
    }
}

void USART2_tx_init(void)
{
    /*********** Enable HSI16 ***********/
    RCC->CR |= RCC_CR_HSION;              // Enable HSI16
    while (!(RCC->CR & RCC_CR_HSIRDY));   // Wait until HSI16 ready

    /*********** Configure UART2 GPIO pins ***********/
    RCC->AHB2ENR |= GPIOAEN;              // Enable GPIOA clock

    // Set PA2 mode to AF7 (USART2_TX)
    GPIOA->MODER &= ~(3U << 4);   // clear MODER2[1:0]
    GPIOA->MODER |=  (2U << 4);   // set to AF
    GPIOA->AFR[0] &= ~(0xFU << 8);
    GPIOA->AFR[0] |=  (7U  << 8); // AF7 = USART2_TX

    /************* Configure USART2 *************/
    RCC->APB1ENR1 |= USART2EN;            // Enable USART2 clock

    // Select HSI16 (16 MHz) as USART2 clock source
    RCC->CCIPR &= ~(3U << 2);   // clear USART2SEL[1:0]
    RCC->CCIPR |=  (2U << 2);   // 10: HSI16 selected

    // Configure baudrate
    uart_set_baudrate(USART2, SYS_FREQ, UART_BAUDRATE);

    // Enable transmitter
    USART2->CR1 |= CR1_TE;

    // Enable USART2
    USART2->CR1 |= CR1_UE;
}

void uart2_write(int ch)
{
    while (!(USART2->ISR & ISR_TXE));  // Wait until TX buffer empty
    USART2->TDR = (ch & 0xFF);
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
    USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
    return ((PeriphClk) + (BaudRate / 2U)) / BaudRate;
}
