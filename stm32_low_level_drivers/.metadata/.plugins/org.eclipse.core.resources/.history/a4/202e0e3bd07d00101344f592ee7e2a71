

#include <stdint.h>

#define ADC_BASE_ADDR          0x50040000UL
#define ADC_CR_REG_OFFSET      0x08
#define ADC_CR_REG_ADDR        (ADC_BASE_ADDR+ADC_CR_REG_OFFSET)

#define RCC_BASE_ADDR          0x40021000UL
#define RCC_AHB2_ENR_OFFSET    0x4C
#define RCC_AHB2_ENR__ADDR     (RCC_BASE_ADDR+RCC_AHB2_ENR_OFFSET)



int main(void)
{
    uint32_t *pAdcCrReg = (uint32_t *)ADC_CR_REG_ADDR;

    uint32_t  *pRCCAhb2enr  = (uint32_t*)RCC_AHB2_ENR__ADDR;

    *pRCCAhb2enr |= 1<<13;

    *pAdcCrReg |= (1<<31);


}
