#include "stm32l4xx.h"
#include <stdio.h>
#include"uart.h"
#include"systick.h"
#include"timer.h"
#include"exti.h"

//static void uart_callback(void);

char  data[]="vinod";
char key;
volatile uint8_t tx_index=0;

int main(void)
{
	USART2_tx_rx_interrupt_init();


    while(1)
    {

    }
}

/*static void uart_callback(void)
{
	key = USART2->RDR;
}*/

void USART2_IRQHandler(void)
{
	if(USART2->ISR & ISR_RXNE)
	{
		key = USART2->RDR;
	}
	if(USART2->ISR & ISR_TXE)
	{
		if (data[tx_index] != '\0')
		{
			USART2->TDR = data[tx_index++];
		}
		else
		{
			// Transmission finished → disable TXE interrupt
			USART2->CR1 &= ~CR1_TXEIE;
			tx_index = 0; // reset if you want to send again later
		}
	}
}
