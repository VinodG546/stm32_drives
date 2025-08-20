
#include "stm32l4xx_hal.h"
#include "tim.h"

//configure timer to timeout every second
//meaning 1hz

//system default freq = 16 000 000
//default pclk1 = 16 000 000
//default pclk2 = 16 000 000
//timer prescaler = 1600 => 16 000 000/ 1600 => 10 000(10KH)
//timer period (Auto relaod Value) = 10 000

 TIM_HandleTypeDef TimHandle;


void tim_timebase_init(void){

	_TIMx_CLK_ENABLE;

	TimHandle.Instance= TIMx;
	TimHandle.Init.Prescaler = 1600-1; // 16 000 000 / 1600 = 10 000
	TimHandle.Init.Period = 10000-1; // 10 000 / 10 000 = 1hz
	TimHandle.Init.ClockDivision = 0;
	TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;


	HAL_TIM_Base_Init(&TimHandle);

	HAL_NVIC_SetPriority(TIMx_IRQn, 0 , 0);
	HAL_NVIC_EnableIRQ(TIMx_IRQn);

	HAL_TIM_Base_Start_IT(&TimHandle);
}

void TIMx_IRQHandler(void){
	HAL_TIM_IRQHandler(&TimHandle);
}
