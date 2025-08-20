#include "adc.h"
//PA0 can be used as ADC12_IN5

extern ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc;

uint32_t adc_read(void)
{
	return HAL_ADC_GetValue(&hadc1);
}

void adc_start(void)
{
	adc_CC_init();
	HAL_ADC_Start(&hadc1);
}

void adc_SC_init()
{
	//Enable clock for GPIOA port
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//Configure PA0 as analog input pin
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	ADC_ChannelConfTypeDef sConfig = {0};

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	//Enable clock for ADC1
	__HAL_RCC_ADC_CLK_ENABLE();

	//Configure ADC module for Continuous Conversion mode
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode  = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

	HAL_ADC_Init(&hadc1);

	//ADC channel congiguration
	sConfig.Channel = ADC_CHANNEL_5;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;

	HAL_ADC_ConfigChannel(&hadc1, &sConfig);


}

void adc_CC_init()
{
	//Enable clock for GPIOA port
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//Configure PA0 as analog input pin
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	ADC_ChannelConfTypeDef sConfig = {0};

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	//Enable clock for ADC1
	__HAL_RCC_ADC_CLK_ENABLE();

	//Configure ADC module for Continuous Conversion mode
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode  = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

	HAL_ADC_Init(&hadc1);

	//ADC channel congiguration
	sConfig.Channel = ADC_CHANNEL_5;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;

	HAL_ADC_ConfigChannel(&hadc1, &sConfig);


}

void adc_CC_Interrupt_init()
{
	//Enable clock for GPIOA port
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//Configure PA0 as analog input pin
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	ADC_ChannelConfTypeDef sConfig = {0};

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	//Enable clock for ADC1
	__HAL_RCC_ADC_CLK_ENABLE();

	//Configure ADC module for Continuous Conversion mode
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode  = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

	HAL_ADC_Init(&hadc1);

	//ADC channel congiguration
	sConfig.Channel = ADC_CHANNEL_5;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;

	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	HAL_NVIC_SetPriority(ADC1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC1_IRQn);

}

void adc_CC_DMA_init(void)
{
	//Enable clock for GPIOA port
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//Configure PA0 as analog input pin
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	ADC_ChannelConfTypeDef sConfig = {0};

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	//Enable clock for ADC1
	__HAL_RCC_ADC_CLK_ENABLE();

	//Configure ADC module for Continuous Conversion mode
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode  = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = ENABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

	HAL_ADC_Init(&hadc1);

	//ADC channel congiguration
	sConfig.Channel = ADC_CHANNEL_5;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;

	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	//Enable clock for DMA2
	__HAL_RCC_DMA1_CLK_ENABLE();

	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);


	hdma_adc.Instance = DMA1_Channel1;
	hdma_adc.Init.Request = DMA_REQUEST_0;
	hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_adc.Init.MemInc = DMA_MINC_DISABLE;
	hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_adc.Init.Mode = DMA_CIRCULAR;
	hdma_adc.Init.Priority = DMA_PRIORITY_LOW;

	HAL_DMA_Init(&hdma_adc);

	//Link adc to DMA
	//__HAL_LINKDMA(hadc1,DMA_Handle,&hdma_adc);
	hadc1.DMA_Handle = &hdma_adc;

}

void adc_CC_Multiple_DMA_init(void)
{
	//Enable clock for GPIOA port
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//Configure PA0 as analog input pin
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	ADC_ChannelConfTypeDef sConfig = {0};

	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	//Enable clock for ADC1
	__HAL_RCC_ADC_CLK_ENABLE();

	//Configure ADC module for Continuous Conversion mode
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.ScanConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode  = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 2;
	hadc1.Init.DMAContinuousRequests = ENABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

	HAL_ADC_Init(&hadc1);

	//ADC channel congiguration
	sConfig.Channel = ADC_CHANNEL_5;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;

	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	//ADC channel congiguration
	sConfig.Channel = ADC_CHANNEL_9;
	sConfig.Rank = 2;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	//Enable clock for DMA2
	__HAL_RCC_DMA1_CLK_ENABLE();

	hdma_adc.Instance = DMA1_Channel1;
	hdma_adc.Init.Request = DMA_REQUEST_0;
	hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_adc.Init.MemInc = DMA_MINC_DISABLE;
	hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_adc.Init.Mode = DMA_CIRCULAR;
	hdma_adc.Init.Priority = DMA_PRIORITY_LOW;

	HAL_DMA_Init(&hdma_adc);

	//Link adc to DMA
	//__HAL_LINKDMA(hadc1,DMA_Handle,&hdma_adc);
	hadc1.DMA_Handle = &hdma_adc;

	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}


void DMA1_CH1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_adc);
}

void ADC_IRQHandler(void)
{
	HAL_ADC_IRQHandler(&hadc1);
}
