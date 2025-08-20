
#ifndef ADC_H_
#define ADC_H_

void adc_init_start(void);
void adc_continuous_conv(void);
uint32_t adc_read(void);
void adc_single_conv(void);

#endif /* ADC_H_ */
