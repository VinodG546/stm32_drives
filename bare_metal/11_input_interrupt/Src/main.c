#include "stm32l4xx.h"
#include <stdio.h>
#include"uart.h"
#include"systick.h"
#include"timer.h"
#include"exti.h"

static void exti_callback(void);

#define GPIOAEN   (1U << 0)
//#define GPIOCEN   (1U << 2)

#define PIN5      (1U<<5)
//#define PIN13	  (1U << 13)

#define LED_PIN   PIN5
//#define BTN_PIN	  PIN13

int main(void)
{
	USART2_tx_rx_init();
	pc13_exti_init();
	RCC->AHB2ENR |= GPIOAEN;
	//RCC->AHB2ENR |= GPIOCEN;


	GPIOA -> MODER |= (1U<<10);
	GPIOA -> MODER &= ~(1U << 11);

	//GPIOC->MODER &= ~((1U<<26) | (1U <<27));

    while(1)
    {

    }
}

static void exti_callback(void)
{
	printf("BTN pressed...\n\r");
}

void EXTI15_10_IRQHandler(void)
{
	if((EXTI->PR1 & LINE13)!=0)
	{
		EXTI -> PR1 |= LINE13;
		exti_callback();
	}
}
