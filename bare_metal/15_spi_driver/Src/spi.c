#include "stm32l4xx.h"
#define GPIOAEN   (1U << 0)
#define SPI1EN    (1U << 12)
#define __IO volatile

void spi1_init(void)
{
    // Enable clocks
    RCC->AHB2ENR |= GPIOAEN;
    RCC->APB2ENR |= SPI1EN;

    // PA5 = SCK, PA6 = MISO, PA7 = MOSI → AF5
    GPIOA->MODER &= ~((3U << 10) | (3U << 12) | (3U << 14));
    GPIOA->MODER |=  ((2U << 10) | (2U << 12) | (2U << 14));

    GPIOA->AFR[0] &= ~((0xF << 20) | (0xF << 24) | (0xF << 28));
    GPIOA->AFR[0] |=  ((5U << 20) | (5U << 24) | (5U << 28));

    GPIOA->OSPEEDR |= (3U << 10) | (3U << 12) | (3U << 14);
    GPIOA->OTYPER  &= ~((1U << 5) | (1U << 6) | (1U << 7));
    GPIOA->PUPDR   &= ~((3U << 10) | (3U << 12) | (3U << 14));

    // ---- SPI1 config ----
    SPI1->CR1 = 0;
    SPI1->CR1 |= (1U << 2);      // Master mode
    SPI1->CR1 |= (0x4 << 3);     // Baud rate = fPCLK/32
    SPI1->CR1 &= ~(1U << 7);     // MSB first
    SPI1->CR1 &= ~(1U << 11);    // 8-bit data format
    SPI1->CR1 &= ~(1U << 1);     // CPOL = 0
    SPI1->CR1 &= ~(1U << 0);     // CPHA = 0
    SPI1->CR1 |= (1U << 8) | (1U << 9);   // SSM=1, SSI=1 (internal NSS high)

    SPI1->CR2 = 0;
    SPI1->CR2 |= (7U << 8);      // DS = 8-bit
    SPI1->CR2 |= (1U << 12);     // FRXTH=1 → RXNE when 8-bit

    SPI1->CR1 |= (1U << 6);      // Enable SPI
}

void spi1_transmit_receive(uint8_t *tx_buf, uint8_t *rx_buf, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++)
    {
        // Wait until TXE=1
        while (!(SPI1->SR & (1U << 1)));

        *((__IO uint8_t*)&SPI1->DR) = tx_buf[i];

        // Wait until RXNE=1
        while (!(SPI1->SR & (1U << 0)));

        rx_buf[i] = *((__IO uint8_t*)&SPI1->DR);
    }
}
