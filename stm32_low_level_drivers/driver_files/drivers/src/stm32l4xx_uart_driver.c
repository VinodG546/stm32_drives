/*
 * stm32l4xx_uart_driver.c
 *
 *  Created on: Sep 3, 2025
 *      Author: VINOD
 */

#include"stm32l4xx_uart_driver.h"

void USART_PeriClockControl(USART_TypeDef *pUSARTx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
		{
			if(pUSARTx == USART1)
				USART1_PCLK_EN();
			else if(pUSARTx == USART2)
				USART2_PCLK_EN();
			else if(pUSARTx == USART3)
				USART3_PCLK_EN();
			else if(pUSARTx == UART4)
				UART4_PCLK_EN();
			else if(pUSARTx == UART5)
				UART5_PCLK_EN();
		}
		else
		{
			if(pUSARTx == USART1)
				USART1_PCLK_DI();
			else if(pUSARTx == USART2)
				USART2_PCLK_DI();
			else if(pUSARTx == USART3)
				USART3_PCLK_DI();
			else if(pUSARTx == UART4)
				UART4_PCLK_DI();
			else if(pUSARTx == UART5)
				UART5_PCLK_DI();
		}
}

/*
 * Init and De-init
 */
void USART_Init(USART_Handle_t *pUSARTHandle)
{
	uint32_t tempCR =0;
	USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

	//Disable USART
	USART_PeripheralControl(pUSARTHandle->pUSARTx, DISABLE);
	//Configure CR1

	//Tx and Rx Configuration
	if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)
	{
		tempCR |= USART_CR1_RE;
	}
	else if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)
	{
		tempCR |= USART_CR1_TE;
	}
	else if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)
	{
		tempCR |= (USART_CR1_TE | USART_CR1_RE);
	}

	//Configure Wordlenth
	if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_8BITS)
	{
		tempCR &= ~(USART_CR1_M0 | USART_CR1_M1);
	}
	else
	{
		tempCR |= USART_CR1_M0;
		tempCR &= ~(USART_CR1_M1);
	}
	//Configure Parity
	if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
	{
		tempCR &= ~(USART_CR1_PCE);
	}
	else if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN)
	{
		tempCR |= USART_CR1_PCE;
		tempCR &= ~USART_CR1_PS;
	}
	else
	{
		tempCR |= USART_CR1_PCE;
		tempCR |= USART_CR1_PS;
	}

	pUSARTHandle->pUSARTx->CR1 = tempCR;

	//Configure CR2
	tempCR &= ~USART_CR2_STOP;

	if(pUSARTHandle->USART_Config.USART_NoOfStopBits == USART_STOPBITS_1)
	{
	    tempCR |= (USART_STOPBITS_1 << USART_CR2_STOP_Pos);
	}
	else if(pUSARTHandle->USART_Config.USART_NoOfStopBits == USART_STOPBITS_2)
	{
	    tempCR |= (USART_STOPBITS_2 << USART_CR2_STOP_Pos);
	}
	else if(pUSARTHandle->USART_Config.USART_NoOfStopBits == USART_STOPBITS_0_5)
	{
	    tempCR |= (USART_STOPBITS_0_5 << USART_CR2_STOP_Pos);
	}
	else // 1.5 stop bits
	{
	    tempCR |= (USART_STOPBITS_1_5 << USART_CR2_STOP_Pos);
	}
	pUSARTHandle->pUSARTx->CR2 = tempCR;

	//Configure CR3
	tempCR = 0;
	//Hardware flow control configuration
	if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_NONE)
		tempCR &= ~(USART_CR3_CTSE | USART_CR3_RTSE);
	else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
		tempCR |= USART_CR3_CTSE;
	else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
		tempCR |= USART_CR3_CTSE;
	else
		tempCR |= (USART_CR3_CTSE | USART_CR3_RTSE);

	pUSARTHandle->pUSARTx->CR3 = tempCR;

	//Configure BRR for baudrate
	/*
	 * USARTDIV = 16,000,000 / (16 * 9600) = 104.1667
	 * Mantissa = 104 (integer part of USARTDIV) --> 0x68 Hex
	 * Fraction = (0.1667 * 16) = 2.667, rounded to 0x3
	 */
	USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_Config.USART_Baud);
	//pUSARTHandle->pUSARTx->BRR = 0x3 << USART_BRR_DIV_FRACTION_Pos;
	//pUSARTHandle->pUSARTx->BRR = 0x68 << USART_BRR_DIV_MANTISSA_Pos;

	//Enable USART
	USART_PeripheralControl(pUSARTHandle->pUSARTx, ENABLE);

}
void USART_DeInit(USART_TypeDef *pUSARTx)
{
	if(pUSARTx == USART1)
		USART1_REG_RESET();
	else if(pUSARTx == USART2)
		USART2_REG_RESET();
	else if(pUSARTx == USART3)
		USART3_REG_RESET();
	else if(pUSARTx == UART4)
		UART4_REG_RESET();
	else if(pUSARTx == UART5)
		UART5_REG_RESET();
}

void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t len)
{
	while(len > 0)
	{
		//check TXE
		while(!(USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_TXE)));

		pUSARTHandle->pUSARTx->TDR = *pTxBuffer;
		pTxBuffer++;
		len--;
	}
	//Check for TC flag to complete the transmission
	while(!(USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_TC)));

}

void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len)
{
	while(len > 0)
	{
		// Wait until RXNE  is set
		while(!(USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_RXNE)));


		// Read 1 byte from RDR
		*pRxBuffer = pUSARTHandle->pUSARTx->RDR;

		pRxBuffer++;
		len--;
	}
}

void USART_SendReceive(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint8_t *pRxBuffer, uint32_t len)
{
	while (len > 0)
	{
		// Wait until TXE is set
		while (!(USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TXE)));

		// Load data into TDR
		pUSARTHandle->pUSARTx->TDR = *pTxBuffer;

		// Wait until RXNE is set
		while (!(USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_RXNE)));

		// Read received data from RDR
		*pRxBuffer = pUSARTHandle->pUSARTx->RDR & 0xFF;

		pTxBuffer++;
		pRxBuffer++;
		len--;
	}

	while (!(USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TC)));
}

uint8_t USART_GetFlagStatus(USART_TypeDef *pUSARTx, uint8_t StatusFlagName)
{
	if(pUSARTx->ISR & StatusFlagName)
			return SET;
		return RESET;
}
void USART_ClearFlag(USART_TypeDef *pUSARTx, uint16_t StatusFlagName)
{
	pUSARTx->ISR &= ~(1<<StatusFlagName);
}
void USART_PeripheralControl(USART_TypeDef *pUSARTx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
		pUSARTx->CR1 |= USART_CR1_UE;
	else
		pUSARTx->CR1 &= ~1<<USART_CR1_UE;
}
#include "stm32l476xx.h"
#include <stdint.h>

// Helper function to extract PCLK from RCC
static uint32_t RCC_GetPCLKFreq(USART_TypeDef *pUSARTx)
{
    uint32_t sysclk, hclk, pclk;
    uint32_t tmp, presc;

    // 1. Get SYSCLK source
    tmp = (RCC->CFGR >> RCC_CFGR_SWS_Pos) & 0x3;
    switch(tmp)
    {
        case 0: sysclk = 4000000U; break;  // MSI (default 4 MHz)
        case 1: sysclk = 16000000U; break; // HSI16
        case 2: sysclk = 8000000U; break;  // HSE (assuming 8 MHz crystal)
        case 3: sysclk = (RCC->PLLCFGR & RCC_PLLCFGR_PLLREN) ? 80000000U : 0; // example PLL (80 MHz if enabled)
                 break;
        default: sysclk = 4000000U; break;
    }

    // 2. Compute HCLK (AHB prescaler)
    presc = (RCC->CFGR >> RCC_CFGR_HPRE_Pos) & 0xF;
    if (presc < 8) hclk = sysclk;
    else {
        static const uint16_t ahb_presc_table[8] = {2,4,8,16,64,128,256,512};
        hclk = sysclk / ahb_presc_table[presc-8];
    }

    // 3. Choose APB1 or APB2
    if (pUSARTx == USART1)
    {
        // APB2
        presc = (RCC->CFGR >> RCC_CFGR_PPRE2_Pos) & 0x7;
        if (presc < 4) pclk = hclk;
        else {
            static const uint8_t apb_presc_table[4] = {2,4,8,16};
            pclk = hclk / apb_presc_table[presc-4];
        }
    }
    else
    {
        // APB1
        presc = (RCC->CFGR >> RCC_CFGR_PPRE1_Pos) & 0x7;
        if (presc < 4) pclk = hclk;
        else {
            static const uint8_t apb_presc_table[4] = {2,4,8,16};
            pclk = hclk / apb_presc_table[presc-4];
        }
    }

    return pclk;
}

void USART_SetBaudRate(USART_TypeDef *pUSARTx, uint32_t BaudRate)
{
	uint32_t pclk = 16000000; // Force HSI16 for debug
	    uint32_t usartdiv;

	    if ((pUSARTx->CR1 & USART_CR1_OVER8) == 0)
	        usartdiv = (pclk + (BaudRate/2U)) / BaudRate;  // Oversampling 16
	    else
	        usartdiv = ((2U * pclk) + (BaudRate/2U)) / BaudRate; // Oversampling 8

	    pUSARTx->BRR = usartdiv;
}

