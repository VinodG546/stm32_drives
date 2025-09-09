#include"stm32l4xx.h"

#define GPIOAEN   (1U << 0)
#define UASRT2EN  (1u << 17)

#define SYS_FREQ	16000000
#define APB1_CLK	SYS_FREQ

#define UART_BAUDRATE	115200

#define CR1_TE 	(1U<<3)
#define CR1_UE  (1U<<0)
#define ISR_TXE  (1U<<7)

static void uart_set_baudrate(USART_TypeDef *USARTX , uint32_t PeriphClk , uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk,uint32_t Baudrate);
void uart2_write(int ch);
void USART_tx_init();

int main()
{

	USART_tx_init();

	while(1)
	{
		uart2_write('S');
		for(int i=0;i<1000000;i++);
	}
}

void USART_tx_init()
{
	/***********configure uart2 gpio pins**********/
	// enable GPIOA clock
	RCC->AHB2ENR |= GPIOAEN;

	// set PA2 mode to alternate function mode
	GPIOA->MODER &= ~(1<<4);
	GPIOA->MODER |= (1<<5);
	//set PA2 alternate function type to UART_Tx (AF07)
	GPIOA->AFR[0] &= ~(0xF << 8);   // clear bits 11:8
	GPIOA->AFR[0] |=  (7U << 8);    // AF7 = 0111

	/*************configure uart2 module*************/
	//configure clock of usart2 peripheral
	RCC->APB1ENR1 |= UASRT2EN;

	//configure baudrate
	uart_set_baudrate(USART2 , APB1_CLK , UART_BAUDRATE);

	//transfer direction
	USART2->CR1 |= CR1_TE;

	//enable uart module
	USA   RT2->CR1 |= CR1_UE;


}
void uart2_write(int ch)
{
	while(!(USART2->ISR & ISR_TXE));

	USART2->TDR = (ch & 0xFF);
}
static void uart_set_baudrate(USART_TypeDef *USARTX , uint32_t PeriphClk , uint32_t BaudRate)
{
	USARTX->BRR = compute_uart_bd(PeriphClk , BaudRate);
}
static uint16_t compute_uart_bd(uint32_t PeriphClk,uint32_t Baudrate)
{
	return ((PeriphClk) + (Baudrate/2U))/Baudrate;
}
