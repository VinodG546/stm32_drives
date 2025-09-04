/*
 * 11_i2c_int.c
 *
 *  Created on: Sep 2, 2025
 *      Author: VINOD
 */


#include "stm32l4xx.h"
#include "stm32l4xx_gpio_driver.h"
#include "stm32l4xx_i2c_driver.h"
#include <string.h>
#include <stdio.h>

I2C_Handle_t I2C1Handle;
#define SLAVE_ADDR	0x55
uint8_t txBuffer[] = "Hello from STM32";
uint8_t rxBuffer[32];

// Flags for completion (set in IRQ handler)
volatile uint8_t txDone = 0;
volatile uint8_t rxDone = 0;

void delay(void) {
    for (uint32_t i = 0; i < 500000; i++);
}

// Configure GPIO pins for I2C1 (PB6=SCL, PB7=SDA)
void I2C1_GPIOInits(void) {
    GPIO_Handle_t I2CPins;

    I2CPins.pGPIOx = GPIOB;
    I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4; // I2C1 AF4
    I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
    I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;

    // SCL
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
    GPIO_Init(&I2CPins);

    // SDA
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
    GPIO_Init(&I2CPins);
}

void I2C1_Inits(void) {
    I2C1Handle.pI2Cx = I2C1;
    I2C1Handle.I2C_Config.I2C_DeviceAddress = 0x61;   // STM32 own address (not used in master mode)
    I2C1Handle.I2C_Config.I2C_ACKControl   = I2C_ACK_ENABLE;
    I2C1Handle.I2C_Config.I2C_SCLSpeed     = I2C_SCL_SPEED_SM;

    I2C_Init(&I2C1Handle);
}
void I2C_ApplicationCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv)
{
    switch(AppEv)
    {
        case I2C_EV_TX_CMPLT:
            txDone = 1;
            break;
        case I2C_EV_RX_CMPLT:
            rxDone = 1;
            break;
        case I2C_EVENT_ERROR:
            // Handle error
            txDone = 1;
            rxDone = 1;
            break;
    }
}
int main(void) {
    // Init GPIO + I2C
    I2C1_GPIOInits();
    I2C1_Inits();

    I2C1Handle.ApplicationCallback = I2C_ApplicationCallback;

    // Enable I2C1 interrupts in NVIC
    I2C_IRQConfig(IRQ_NO_I2C1_EV, ENABLE);
    I2C_IRQConfig(IRQ_NO_I2C1_ER, ENABLE);

    // Send data to Arduino slave (address 0x08)
    I2C_MasterSendData_IT(&I2C1Handle, txBuffer, strlen((char*)txBuffer), SLAVE_ADDR);

    while (!txDone);   // wait until TX complete

    delay(); // small delay

    // Receive data from Arduino slave (address 0x08)
    I2C_MasterReceiveData_IT(&I2C1Handle, rxBuffer, sizeof(rxBuffer), SLAVE_ADDR);

    while (!rxDone);   // wait until RX complete

    // Here rxBuffer holds the received string
    while (1);
}

/*
 * IRQ Handlers
 */
void I2C1_EV_IRQHandler(void) {
    I2C_IRQHandling(&I2C1Handle);

}

void I2C1_ER_IRQHandler(void)
{
    uint32_t isr = I2C1->ISR;
    uint8_t error_event = I2C_EVENT_ERROR;

    if(isr & (1 << I2C_ISR_BERR)) {
        I2C1->ICR |= (1 << I2C_ICR_BERRCF);
        error_event = I2C_ERROR_BERR;
    }
    if(isr & (1 << I2C_ISR_ARLO)) {
        I2C1->ICR |= (1 << I2C_ICR_ARLOCF);
        error_event = I2C_ERROR_ARLO;
    }
    if(isr & (1 << I2C_ISR_NACKF)) {
        I2C1->ICR |= (1 << I2C_ICR_NACKCF);
        error_event = I2C_ERROR_AF;
    }
    if(isr & (1 << I2C_ISR_OVR)) {
        I2C1->ICR |= (1 << I2C_ICR_OVRCF);
        error_event = I2C_ERROR_OVR;
    }
    if(isr & (1 << I2C_ISR_TIMEOUT)) {
        I2C1->ICR |= (1 << I2C_ICR_TIMOUTCF);
        error_event = I2C_ERROR_TIMEOUT;
    }

    // Reset state and notify application
    I2C1Handle.TxRxState = I2C_READY;
    if(I2C1Handle.ApplicationCallback)
    {
        I2C1Handle.ApplicationCallback(&I2C1Handle, error_event);
    }
}
