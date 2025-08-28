/*
 * 05_gpio_interrupt.c
 *
 *  Created on: Aug 25, 2025
 *      Author: VINOD
 */
#include "stm32l4xx_gpio_driver.h"
#include<string.h>
#define HIGH 1
#define LOW  0
#define BTN_PRESSED LOW

void delay(void){
    for(uint32_t i=0; i<50000/2; i++);
}

int main(void)
{
    GPIO_Handle_t GpioLed, GpioButton;
    memset(&GpioLed,0,sizeof(GpioLed));
    memset(&GpioButton,0,sizeof( GpioButton));

    // LED: PA5
    GpioLed.pGPIOx = GPIOA;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_PeriClockControl(GPIOA, ENABLE);
    GPIO_Init(&GpioLed);

    // Button: PC13 (falling edge interrupt)
    GpioButton.pGPIOx = GPIOC;
    GpioButton.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GpioButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
    GpioButton.GPIO_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;
    GpioButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_PeriClockControl(GPIOC, ENABLE);
    SYSCFG_PCLK_EN();
    GPIO_Init(&GpioButton);

    // NVIC settings
    GPIO_IRQ_interrupt_Config(IRQ_NO_EXTI15_10, ENABLE);
    GPIO_IRQ_priority_Config(IRQ_NO_EXTI15_10, 2);

    while(1){
        // wait for interrupts
    }
}

void EXTI15_10_IRQHandler(void){
	delay();
    GPIO_IRQHandling(GPIO_PIN_NO_13);  // clear pending bit
    GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
}
