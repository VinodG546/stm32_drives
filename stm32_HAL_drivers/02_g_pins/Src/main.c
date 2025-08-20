#include "stm32l4xx_hal.h"
#include <stdio.h>

#define BTN_PIN		GPIO_PIN_13
#define BTN_PORT	GPIOC
#define LED_PIN		GPIO_PIN_5
#define LED_PORT	GPIOA
/*
 * User Button = PC13, 	Bus = AHB2EN bit0
 * User LED = PA5,		Bus = AHB2EN bit2
 * */

void btn_init(void);
void led_init(void);


uint8_t button_status;

int main()
{
	HAL_Init();
	btn_init();
	led_init();
	while(1)
	{
		button_status = HAL_GPIO_ReadPin(BTN_PORT, BTN_PIN);
		HAL_GPIO_WritePin(LED_PORT, LED_PIN, button_status);

	}
}

void btn_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	//2. Configure PC13 as GPIO Input for Button
	//Enable CLock
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitStruct.Pin = BTN_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BTN_PORT, &GPIO_InitStruct);
}
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	//1. Configure PA5 as GPIO output for LED
	//Enable Clock
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin = LED_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}

void SysTick_Handler(void)
{
	HAL_IncTick();
}
