#include "stm32l4xx.h"
#include <stdio.h>
#include"uart.h"
#include"systick.h"
#include"timer.h"


int val;
int main(void)
{
    USART2_tx_rx_init();
    tim2_1hz_init();


    while(1)
    {
    	while(!(TIM2->SR & SR_UIF));
    	TIM2->SR &= ~(SR_UIF);
        printf("vinod\r\n");
        systickDelayMs(1000);
    }
}
