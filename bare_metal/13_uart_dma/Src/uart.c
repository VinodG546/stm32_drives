
#include"uart.h"
#include"stm32l4xx.h"


#define GPIOAEN     (1U << 0)
#define USART2EN    (1U << 17)

#define SYS_FREQ    16000000
#define UART_BAUDRATE 9600

#define CR1_TE      (1U << 3)
#define CR1_RE      (1U << 2)

#define CR1_UE      (1U << 0)

#define ISR_TXE     (1U << 7)

#define DMA_S_EN   (1U<<0)



void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
void uart2_write(int ch);
void USART2_rxtx_init(void);

int __io_putchar(int ch)
{
    uart2_write(ch);
    return ch;
}





void dma1_stream6_init(uint32_t  src  , uint32_t dst , int len  )
{
	//enable clock
	RCC->AHB1ENR |= 1<<0 ;

	//disable DMA1 channel 7
	DMA1_Channel7->CCR &= ~DMA_S_EN ;

	while(DMA1_Channel7->CCR & DMA_S_EN ){}
	//clear all interrupt flags of streams
	DMA1->IFCR &= ~(0xf << 24);
	//set the destination buffer
	DMA1_Channel7->CPAR = dst ;
	//set the src buffer
	DMA1_Channel7->CMAR = src ;
	//set length
	DMA1_Channel7->CNDTR = len ;
	//select channel
	DMA1_Channel7->CCR = 0;

	// 7. Select channel request in DMA1_CSELR
	DMA1_CSELR->CSELR &= ~(0xF << 24);   // clear bits for channel 7
	DMA1_CSELR->CSELR |=  (2 << 24);     // map channel 7 to request 2 (example)


	//Enable memory inc
	DMA1_Channel7->CCR |= (1 << 7);
	DMA1_Channel7->CCR |= (1<<4);

	//configure transfer complete interrupt
	DMA1_Channel7->CCR |=  (1<<1);


	//enable DMA
	DMA1_Channel7->CCR |= 1 ;

	//Enable UART2 transmitter
	USART2->CR3 |= (1<<7);


	NVIC_EnableIRQ(DMA1_Channel7_IRQn);


}

void USART2_rxtx_init(void)
{
    /*********** Enable HSI16 ***********/
    RCC->CR |= RCC_CR_HSION;              // Enable HSI16
    while (!(RCC->CR & RCC_CR_HSIRDY));   // Wait until HSI16 ready

    /*********** Configure UART2 GPIO pins ***********/
    RCC->AHB2ENR |= GPIOAEN;              // Enable GPIOA clock

    // Set PA2 mode to AF7 (USART2_TX)
    GPIOA->MODER &= ~(3U << 4);   // clear MODER2[1:0]
    GPIOA->MODER |=  (2U << 4);   // set to AF

    GPIOA->AFR[0] &= ~(0xFU << 8);
    GPIOA->AFR[0] |=  (7U  << 8); // AF7 = USART2_TX


    //Set PA3 mode to AF7
    GPIOA->MODER &= ~(1<<6);
    GPIOA->MODER |= (1<<7);

    GPIOA->AFR[0] &= ~(0xFU << 12);
    GPIOA->AFR[0] |=  (7U  << 12); // AF7 = USART2_Rx

    /************* Configure USART2 *************/
    RCC->APB1ENR1 |= USART2EN;            // Enable USART2 clock

    // Select HSI16 (16 MHz) as USART2 clock source
    RCC->CCIPR &= ~(3U << 2);   // clear USART2SEL[1:0]
    RCC->CCIPR |=  (2U << 2);   // 10: HSI16 selected

    // Configure baudrate
    uart_set_baudrate(USART2, SYS_FREQ, UART_BAUDRATE);

    // Enable transmitter
    USART2->CR1 |= (CR1_TE | CR1_RE) ;

    // Enable USART2
    USART2->CR1 |= CR1_UE;

    NVIC_EnableIRQ(USART2_IRQn);

}



void USART2_rx_interrupt_init(void)
{
    /*********** Enable HSI16 ***********/
    RCC->CR |= RCC_CR_HSION;              // Enable HSI16
    while (!(RCC->CR & RCC_CR_HSIRDY));   // Wait until HSI16 ready

    /*********** Configure UART2 GPIO pins ***********/
    RCC->AHB2ENR |= GPIOAEN;              // Enable GPIOA clock

    // Set PA2 mode to AF7 (USART2_TX)
    GPIOA->MODER &= ~(3U << 4);   // clear MODER2[1:0]
    GPIOA->MODER |=  (2U << 4);   // set to AF

    GPIOA->AFR[0] &= ~(0xFU << 8);
    GPIOA->AFR[0] |=  (7U  << 8); // AF7 = USART2_TX


    //Set PA3 mode to AF7
    GPIOA->MODER &= ~(1<<6);
    GPIOA->MODER |= (1<<7);

    GPIOA->AFR[0] &= ~(0xFU << 12);
    GPIOA->AFR[0] |=  (7U  << 12); // AF7 = USART2_Rx

    /************* Configure USART2 *************/
    RCC->APB1ENR1 |= USART2EN;            // Enable USART2 clock

    // Select HSI16 (16 MHz) as USART2 clock source
    RCC->CCIPR &= ~(3U << 2);   // clear USART2SEL[1:0]
    RCC->CCIPR |=  (2U << 2);   // 10: HSI16 selected

    // Configure baudrate
    uart_set_baudrate(USART2, SYS_FREQ, UART_BAUDRATE);

    // Enable transmitter
    USART2->CR1 |= (CR1_TE | CR1_RE) ;

    USART2->CR1 |= CR1_RXINE ;

    // Enable USART2
    USART2->CR1 |= CR1_UE;
}



char uart2_read(void){

	while(!(USART2->ISR & ISR_RXNE) ){}

	return USART2->RDR ;

}

void uart2_write(int ch)
{
    while (!(USART2->ISR & ISR_TXE));  // Wait until TX buffer empty
    USART2->TDR = (ch & 0xFF);
}

void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
    USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
    return ((PeriphClk) + (BaudRate / 2U)) / BaudRate;
}
