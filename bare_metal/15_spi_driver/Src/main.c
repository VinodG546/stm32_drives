#include "stm32l4xx.h"
#include <stdio.h>
#include<stdint.h>
#include<string.h>
#include"uart.h"
#include"i2c.h"
uint8_t tx_buf[] = "HELLO SPI";
uint8_t rx_buf[sizeof(tx_buf)];
int main(void)
{
	spi1_init();

    while(1)
    {
    	 spi1_transmit_receive(tx_buf, rx_buf, sizeof(tx_buf));

		// Now rx_buf should equal tx_buf
		// You can check with debugger or print via UART

		for (volatile int i = 0; i < 100000; i++); // Delay

    }
}
