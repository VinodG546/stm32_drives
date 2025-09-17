/*
 * adc.c
 *
 *  Created on: Sep 9, 2025
 *      Author: VINOD
 */
#include "adc.h"

#define ADC1EN  (1U << 13)
#define GPIOAEN (1U << 0)

void pa1_adc1_init(void)
{
    /************** Configure PA1 as analog ****************/
    RCC->AHB2ENR |= GPIOAEN;            // Enable GPIOA clock
    GPIOA->MODER |= (3U << 2);          // Analog mode
    GPIOA->ASCR  |= (1U << 1);          // Connect analog switch

    /************** Enable ADC1 clock ****************/
    RCC->AHB2ENR |= ADC1EN;

    /************** Exit deep power-down ****************/
    ADC1->CR &= ~ADC_CR_DEEPPWD;        // Must disable before ADVREGEN

    /************** Enable ADC voltage regulator ****************/
    ADC1->CR &= ~ADC_CR_ADVREGEN;       // Clear voltage regulator bits
    ADC1->CR |=  (1U << 28);            // Enable ADC regulator
    for (volatile uint32_t i = 0; i < 1000; i++); // wait ~20us

    /************** Calibrate ADC ****************/
    if (ADC1->CR & ADC_CR_ADEN) {
        ADC1->CR |= ADC_CR_ADDIS;       // Request ADC disable
    }
    while (ADC1->CR & ADC_CR_ADEN);     // Wait until fully disabled

    ADC1->CR &= ~ADC_CR_ADCALDIF;       // Single-ended calibration
    ADC1->CR |= ADC_CR_ADCAL;           // Start calibration
    while (ADC1->CR & ADC_CR_ADCAL);    // ✅ Should now complete

    /************** Enable ADC ****************/
    ADC1->CR |= ADC_CR_ADEN;            // Enable ADC
    while (!(ADC1->ISR & ADC_ISR_ADRDY));

    /************** Configure ADC parameters ****************/
    ADC1->CFGR &= ~ADC_CFGR_CONT;       // Single conversion
    ADC1->CFGR &= ~ADC_CFGR_RES;        // 12-bit
    ADC1->CFGR &= ~ADC_CFGR_ALIGN;      // Right alignment

    /************** Configure channel 6 (PA1) ****************/
    ADC1->SQR1 &= ~ADC_SQR1_L;          // 1 conversion
    ADC1->SQR1 &= ~ADC_SQR1_SQ1;
    ADC1->SQR1 |= (6U << 0);            // SQ1 = channel 6

    ADC1->SMPR1 &= ~(7U << 18);
    ADC1->SMPR1 |=  (2U << 18);         // Sampling time
}

void adc_start(void)
{
    // Clear end of conversion flag
    ADC1->ISR &= ~ADC_ISR_EOC;

    // Start conversion
    ADC1->CR |= ADC_CR_ADSTART;
}

uint32_t adc_read(void)
{
    // Wait until conversion complete
    while (!(ADC1->ISR & ADC_ISR_EOC));

    // Clear end of conversion flag
    ADC1->ISR &= ~ADC_ISR_EOC;

    // Read data register
    return ADC1->DR;
}

// Optional: Function to check if ADC is ready
uint8_t adc_is_ready(void)
{
    return (ADC1->ISR & ADC_ISR_ADRDY);
}

// Optional: Function to disable ADC
void adc_disable(void)
{
    ADC1->CR |= ADC_CR_ADDIS;
    while (ADC1->CR & ADC_CR_ADEN);
}
