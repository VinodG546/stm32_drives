#include<stdint.h>
#include <stdio.h>
#include"uart.h"
#include"stm32l4xx.h"


void Clock_Config(void)
{
    /******** Enable HSI16 ********/
    RCC->CR |= RCC_CR_HSION;                // Turn on HSI16
    while (!(RCC->CR & RCC_CR_HSIRDY));     // Wait until ready

    /******** Set HSI16 as system clock ********/
    RCC->CFGR &= ~(RCC_CFGR_SW);            // Clear SW bits
    RCC->CFGR |=  (RCC_CFGR_SW_HSI);        // Select HSI16 as SYSCLK
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);

    /******** AHB Prescaler = 1 ********/
    RCC->CFGR &= ~(RCC_CFGR_HPRE);

    /******** APB1 Prescaler = 1 ********/
    RCC->CFGR &= ~(RCC_CFGR_PPRE1);

    /******** APB2 Prescaler = 1 ********/
    RCC->CFGR &= ~(RCC_CFGR_PPRE2);
}

#define GPIOAEN  (1<<0)
#define GPIOA_5  (1U<<5)

#define LED_PIN GPIOA_5

uint8_t key ;


int main(void)
{

	char msg[]="hello from stm32 dma\n\r";

	Clock_Config();

    RCC->AHB2ENR |= GPIOAEN ;

    GPIOA->MODER |= (1<<10 );
    GPIOA->MODER &= ~(1<<11);

    USART2_rxtx_init();
    dma1_stream6_init((uint32_t )msg, (uint32_t )&USART2->TDR, sizeof(msg));
    //systick_1hz_interrupt();

    while(1){
    }
}

void dma_callback(void ){
	GPIOA->ODR |=LED_PIN ;
}

void DMA1_CH7_IRQHandler(void ){


	if( DMA1->ISR & ISR_TCIF7){
		DMA1->IFCR &= ~IFCR_CTCIF;
		dma_callback();

	}
}





