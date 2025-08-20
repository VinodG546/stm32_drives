#include "stm32l4xx_hal.h"
#include "adc.h"

extern ADC_HandleTypeDef hadc1;

uint32_t adc_read(void) {
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        return HAL_ADC_GetValue(&hadc1);
    }
    return 0;
}

void adc_init_start(void) {
    adc_continuous_conv();
    HAL_ADC_Start(&hadc1);
}

void adc_continuous_conv(void) {
    // Enable GPIOA clock
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Enable ADC clock
    __HAL_RCC_ADC_CLK_ENABLE();

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

    HAL_ADC_Init(&hadc1);

    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_5;  // PA0 = ADC1_IN5
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}
void adc_single_conv(void){
	// Enable GPIOA clock
	    __HAL_RCC_GPIOA_CLK_ENABLE();

	    GPIO_InitTypeDef GPIO_InitStruct = {0};
	    GPIO_InitStruct.Pin = GPIO_PIN_0;
	    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	    // Enable ADC clock
	    __HAL_RCC_ADC_CLK_ENABLE();

	    hadc1.Instance = ADC1;
	    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	    hadc1.Init.ContinuousConvMode = DISABLE;
	    hadc1.Init.DiscontinuousConvMode = DISABLE;
	    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	    hadc1.Init.NbrOfConversion = 1;
	    hadc1.Init.DMAContinuousRequests = DISABLE;
	    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

	    HAL_ADC_Init(&hadc1);

	    ADC_ChannelConfTypeDef sConfig = {0};
	    sConfig.Channel = ADC_CHANNEL_5;  // PA0 = ADC1_IN5
	    sConfig.Rank = ADC_REGULAR_RANK_1;
	    sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;

	    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}
