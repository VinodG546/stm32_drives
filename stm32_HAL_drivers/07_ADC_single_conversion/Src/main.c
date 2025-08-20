#include "stm32l4xx_hal.h"
#include"adc.h"


ADC_HandleTypeDef hadc1;

uint32_t sensor_value;


int main()
{
	HAL_Init();
	adc_single_conv();
	while(1)
	{
		HAL_ADC_Start(&hadc1);

		HAL_ADC_PollForConversion(&hadc1,1);

		sensor_value = adc_read();
		//HAL_Delay(10);
	}
}


void SysTick_Handler(void)
{
	HAL_IncTick();
}

