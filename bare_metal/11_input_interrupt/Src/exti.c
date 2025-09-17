/*
 * exti.c
 *
 *  Created on: Sep 9, 2025
 *      Author: VINOD
 */

#include"exti.h"

#define GPIOCEN (1U<<2)

void pc13_exti_init(void)
{
	//disable global interrupt
	__disable_irq();
	//enable clock access for gpioc
	RCC->AHB2ENR |= GPIOCEN;
	//enable clock access to syscfg
	RCC->APB2ENR |= (1U<<0);

	GPIOC->MODER &= ~(1U<<26);
	GPIOC->MODER &= ~(1U<<27);

	SYSCFG -> EXTICR[3] |= (1U<<5);

	EXTI -> IMR1 |= (1U<<13);

	EXTI -> FTSR1 |= (1U<<13);
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	__enable_irq();
}
