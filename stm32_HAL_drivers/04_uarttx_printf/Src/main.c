#include "stm32l4xx_hal.h"
#include<stdio.h>
#include<stdarg.h>

UART_HandleTypeDef huart2;

void uart_init(void);

char message[20] = "HELLO FROM STM32\r\n";

int __io_Putchar(int ch){
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 10);
	return ch;
}
int main(void)
{
    HAL_Init();
    uart_init();

    while (1)
    {
    	printf("printf is being used \n\r");
        HAL_Delay(200);
    }
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void uart_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIOA clock (for PA2, PA3)
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Enable USART2 clock
    __HAL_RCC_USART2_CLK_ENABLE();

    // Configure PA2 (TX) and PA3 (RX) for USART2
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure UART module
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX; // TX only
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&huart2);
}

