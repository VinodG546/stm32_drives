#include "stm32l4xx.h"
#include <stdio.h>
#include<stdint.h>
#include<string.h>
#include"uart.h"
#include"i2c.h"
uint8_t buffer[]={1,2,3,4,5,6};
uint8_t msg[30];
int main(void)
{
	USART2_tx_rx_init();
	i2c1_init();
	i2c1_master_send(0x3C, buffer, sizeof(buffer));
	i2c1_master_receive(0x3C,msg,sizeof(msg));
	printf("%s\r\n",msg);
    while(1)
    {


    }
}
