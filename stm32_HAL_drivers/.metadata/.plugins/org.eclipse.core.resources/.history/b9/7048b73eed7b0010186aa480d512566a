#include "stm32l4xx_hal.h"
#include "adc.h"
#define LED_PIN		GPIO_PIN_5
#define LED_PORT	GPIOA


void led_init(void);
void Error_Handler(void);
void SystemClock_Config(void);

ADC_HandleTypeDef hadc1;

uint32_t adc_val[2];
int main()
{
	HAL_Init();
	SystemClock_Config();
	led_init();
	adc_CC_Multiple_DMA_init();
	//1.Start ADC_IT
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_val, 2);
	while(1)
	{

	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
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

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Supply configuration update enable */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);  // Required for high freq

    /** Initializes the CPU, AHB and APB busses clocks */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS; // MCO from ST-Link
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 20;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2; // 8/1*20/2 = 80 MHz
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7; // Not used
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2; // Optional (for USB, RNG etc.)

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB busses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // Use PLL as system clock
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }

    // Update SysTick to reflect new frequency
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}



void SysTick_Handler(void)
{
	HAL_IncTick();
}
void Error_Handler(void)
{

}
