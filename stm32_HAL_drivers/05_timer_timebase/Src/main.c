#include "stm32l4xx_hal.h"

#include "tim.h"

#define LED_PIN		GPIO_PIN_5
#define LED_PORT	GPIOA

int k=0;
void led_init(void);

int main()
{
	HAL_Init();
	led_init();
	tim_timebase_init();

	while(1)
	{

	}
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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
	k++;

}
void SysTick_Handler(void)
{
	HAL_IncTick();
}

