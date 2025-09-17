/*
 * systick.c
 *
 *  Created on: Sep 9, 2025
 *      Author: VINOD
 */
#include"stm32l4xx.h"
#include"systick.h"

#define SYSTICK_LOAD_VAL    16000
#define CTRL_ENABLE			(1U << 0)
#define CTRL_CLKSRC			(1U<<2)
#define CTRL_COUNTFLAG		(1U << 16)

void systickDelayMs(int n)
{
	SysTick->LOAD  = SYSTICK_LOAD_VAL;

	SysTick -> VAL = 0 ;

	SysTick -> CTRL = CTRL_CLKSRC | CTRL_ENABLE ;

	for(int i=0;i<n;i++)
	{
		while((SysTick -> CTRL  & CTRL_COUNTFLAG)==0){}

	}
	SysTick -> CTRL = 0;

}

