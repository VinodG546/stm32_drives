#include "stm32l4xx.h"
#include <stdio.h>
#include"uart.h"
char ch;
int main(void)
{
    USART2_tx_rx_init();

    while(1)
    {
        ch=uart2_read();

    }
}
