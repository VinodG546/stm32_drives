#include "stm32l4xx_hal.h"
#include "adc.h"
#define LED_PIN		GPIO_PIN_5
#define LED_PORT	GPIOA

void led_init(void);

ADC_HandleTypeDef hadc1;

uint32_t adc_val;
int main()
{
	HAL_Init();
	led_init();
	adc_CC_Interrupt_init();
	//1.Start ADC_IT
	HAL_ADC_Start_IT(&hadc1);
	while(1)
	{

	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	adc_val = adc_read();
	HAL_GPIO_TogglePin(LED_PORT, LED_PIN);

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
