#include "stm32l4xx.h"
#include <stdio.h>
#include<stdint.h>
#include<string.h>
#include"uart.h"
#include"i2c.h"
uint8_t buffer[]={1,2,3,4,5,6};
int main(void)
{
	i2c1_init();
	i2c1_master_send(0x3C, buffer, sizeof(buffer));
    while(1)
    {


    }
}
