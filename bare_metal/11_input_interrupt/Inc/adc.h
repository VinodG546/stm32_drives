/*
 * adc.h
 *
 *  Created on: Sep 9, 2025
 *      Author: VINOD
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32l4xx.h"

void pa1_adc1_init(void);
uint32_t adc_read(void);
void adc_start(void);

#endif /* ADC_H_ */
