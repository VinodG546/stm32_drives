#include "stm32l4xx.h"
#include <stdio.h>
#include"uart.h"
#include"adc.h"

int val;
int main(void)
{
    USART2_tx_rx_init();
    pa1_adc1_init();
    while(1)
    {
        val=adc_read();
        printf("ADC value:%d\r\n",val);
        for(int i=0;i<100000;i++);
    }
}
