#include "stm32l4xx_hal.h"

void gpio_interrupt_init(void);
#define BTN_PIN		GPIO_PIN_13
#define BTN_PORT	GPIOC
#define LED_PIN		GPIO_PIN_5
#define LED_PORT	GPIOA

int b13=0;
int b0=0;
int main()
{
	HAL_Init();
	gpio_interrupt_init();
	while(1)
	{

	}
}

void gpio_interrupt_init(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
		//2. Configure PC13 as GPIO Input for Button
		//Enable CLock
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		GPIO_InitStruct.Pin = BTN_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; //external interrupt mode with rising edge trigger detection
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(BTN_PORT, &GPIO_InitStruct);

		//configure pa0
		GPIO_InitStruct.Pin = GPIO_PIN_0;;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; //external interrupt mode with rising edge trigger detection
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(BTN_PORT, &GPIO_InitStruct);


		GPIO_InitStruct.Pin = LED_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; //external interrupt mode with rising edge trigger detection
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);


		HAL_NVIC_SetPriority(EXTI15_10_IRQn,0,0);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

		HAL_NVIC_SetPriority(EXTI0_IRQn,3,0);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{	if(GPIO_Pin == GPIO_PIN_13)
	{
		HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
		b13++;
	}
	if(GPIO_Pin == GPIO_PIN_0){
		b0++;
	}
}
void EXTI0_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);

}
void EXTI15_10_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(BTN_PIN);

}
void SysTick_Handler(void)
{
	HAL_IncTick();
}

