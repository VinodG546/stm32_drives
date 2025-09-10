#include "stm32l4xx.h"
#include <stdio.h>
#include"uart.h"
#include"systick.h"


int val;
int main(void)
{
    USART2_tx_rx_init();

    while(1)
    {
        printf("vinod\r\n");
        systickDelayMs(1000);
    }
}
