
#ifndef ADC_H_
#define ADC_H_
#include "stm32l4xx_hal.h"

void adc_start(void);
uint32_t adc_read(void);
void adc_CC_init(void);
void adc_SC_init(void);
void adc_CC_Interrupt_init(void);


#endif /* ADC_H_ */
