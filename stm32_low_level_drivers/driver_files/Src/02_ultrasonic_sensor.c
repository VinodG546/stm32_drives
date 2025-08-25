/*
 * 02_ultrasonic_sensor.c
 *
 *  Created on: Aug 22, 2025
 *      Author: VINOD
 */
//#include "stm32l4xx.h"
#include "stm32l4xx_gpio_driver.h"

uint32_t distance;

// HC-SR04 pins
#define TRIG_PIN   9   // Example: PA9
#define ECHO_PIN   8   // Example: PA8

// Simple delay function
void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms * 4000; i++) {
        __asm("NOP");
    }
}

void delay_us(uint32_t us) {
    for(uint32_t i = 0; i < us * 4; i++) {
        __asm("NOP");
    }
}

void ultrasonic_init(void) {
    GPIO_Handle_t gpioTrig, gpioEcho;

    // Configure Trigger pin (output)
    gpioTrig.pGPIOx = GPIOA;
    gpioTrig.GPIO_PinConfig.GPIO_PinNumber = TRIG_PIN;
    gpioTrig.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    gpioTrig.GPIO_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;
    gpioTrig.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    gpioTrig.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GPIO_PeriClockControl(GPIOA, ENABLE);
    GPIO_Init(&gpioTrig);

    // Configure Echo pin (input)
    gpioEcho.pGPIOx = GPIOA;
    gpioEcho.GPIO_PinConfig.GPIO_PinNumber = ECHO_PIN;
    gpioEcho.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    gpioEcho.GPIO_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;
    gpioEcho.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GPIO_Init(&gpioEcho);
}

uint32_t ultrasonic_get_distance(void) {
    uint32_t timeCount = 0;

    // Send 10µs pulse on TRIG
    GPIO_WriteToOutputPin(GPIOA, TRIG_PIN, 0);
    delay_us(2);
    GPIO_WriteToOutputPin(GPIOA, TRIG_PIN, 1);
    delay_us(10);
    GPIO_WriteToOutputPin(GPIOA, TRIG_PIN, 0);

    // Wait for ECHO to go HIGH
    while(!GPIO_ReadFromInputPin(GPIOA, ECHO_PIN));

    // Measure HIGH time of ECHO
    while(GPIO_ReadFromInputPin(GPIOA, ECHO_PIN)) {
        timeCount++;
        delay_us(1);
    }

    // Distance calculation (time in µs / 58 = distance in cm)
    return (timeCount / 58);
}

int main(void) {
    ultrasonic_init();

    while(1) {
        distance = ultrasonic_get_distance();
        // here you can debug using UART/LED
        delay_ms(50);
    }
}
