/*
 * timer.c
 *
 *  Created on: Sep 9, 2025
 *      Author: VINOD
 */

#include"timer.h"
#define TIM2EN   (1U<<0)
#define CR1_CEN  (1U<<0)
void tim2_1hz_init(void)
{
	RCC->APB1ENR1 |= TIM2EN;

	TIM2->PSC = 1600-1;

	TIM2->ARR = 10000-1;

	TIM2->CNT = 0;
	TIM2->CR1  |= CR1_CEN;

}
