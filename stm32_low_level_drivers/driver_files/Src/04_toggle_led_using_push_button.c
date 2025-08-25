/*
 * 04_toggle_led_using_push_button.c
 *
 *  Created on: Aug 25, 2025
 *      Author: VINOD
 */


#include"stm32l4xx_gpio_driver.h"

void delay(void){
	for(uint32_t i=0;i<50000/2;i++);
}
int main(){
	GPIO_Handle_t GpioLed,GpioButton;

	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType =GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_PIN_PU;

	GPIO_PeriClockControl(GPIOA, ENABLE);

	GPIO_Init(&GpioLed);

	GpioButton.pGPIOx = GPIOC;
	GpioButton.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GpioButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioButton.GPIO_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;
	//GpioButton.GPIO_PinConfig.GPIO_PinOPType =GPIO_OP_TYPE_OD;
	GpioButton.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOC, ENABLE);
	GPIO_Init(&GpioButton);

	while(1){
		if(GPIO_ReadFromInputPin(GPIOC,GPIO_PIN_NO_13)==0)
		{
			GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
			delay();
		}
	}

}


