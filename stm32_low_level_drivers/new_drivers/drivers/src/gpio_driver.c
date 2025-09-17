
#include "gpio_driver.h"


/*
 * Peripheral Clock Setup
 */
void GPIO_PclkControl(GPIO_TypeDef *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
			GPIOA_PCLK_EN();
		else if(pGPIOx == GPIOB)
			GPIOB_PCLK_EN();
		else if(pGPIOx == GPIOC)
			GPIOC_PCLK_EN();
		else if(pGPIOx == GPIOD)
			GPIOD_PCLK_EN();
		else if(pGPIOx == GPIOE)
			GPIOE_PCLK_EN();
		else if(pGPIOx == GPIOF)
			GPIOF_PCLK_EN();
		else if(pGPIOx == GPIOG)
			GPIOG_PCLK_EN();
		else if(pGPIOx == GPIOH)
			GPIOH_PCLK_EN();
	}
	else
	{
		if(pGPIOx == GPIOA)
			GPIOA_PCLK_DI();
		else if(pGPIOx == GPIOB)
			GPIOB_PCLK_DI();
		else if(pGPIOx == GPIOC)
			GPIOC_PCLK_DI();
		else if(pGPIOx == GPIOD)
			GPIOD_PCLK_DI();
		else if(pGPIOx == GPIOE)
			GPIOE_PCLK_DI();
		else if(pGPIOx == GPIOF)
			GPIOF_PCLK_DI();
		else if(pGPIOx == GPIOG)
			GPIOG_PCLK_DI();
		else if(pGPIOx == GPIOH)
			GPIOG_PCLK_DI();
	}
}

/*
 * GPIO Init & Deinit
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp=0;
	// 1.Configure Mode for GPIO Pin
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		//Non Interrupt mode
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(3 << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER |= temp;
	}
	else
	{
		//Interrupt Mode
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FE)
		{
			//1.Configure FTSR
			EXTI->FTSR1 |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear RTSR to make sure that same bit in RTSR is disabled
			EXTI->RTSR1 &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RE)
		{
			//1.Configure RTSR
			EXTI->RTSR1 |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear FTSR to make sure that same bit in RTSR is disabled
			EXTI->FTSR1 &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFE)
		{
			//1.Configure both FTSR and RTSR
			EXTI->FTSR1 |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR1 |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		//2.Configure GPIO port selection in SYSCFG_EXTICR
		uint8_t t1, t2;
		t1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber/4;
		t2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber%4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);

		SYSCFG_PCLK_EN();
		//SYSCFG->EXTICR[t1] = portcode << (4*t2);
		SYSCFG->EXTICR[t1] &= ~(0xF << (4*t2));              // Clear 4 bits
		SYSCFG->EXTICR[t1] |= (portcode << (4*t2));          // Set desired port


		//3.Enable EXTI interrupt delivery using IMR
		EXTI->IMR1 |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}
	temp =0;
	// 2.Configure the Speed
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(3 << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;

	temp = 0;
	// 3.Configure PUPD
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(3 << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR |= temp;

	temp = 0;
	// 4.Configure Output type
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER |= temp;
	// 5.Configure Alternate functionality
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_AFN)
	{
		uint8_t temp1, temp2;

		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber/8; // This give the either 0 or 1 to know AF[0] or AF[1]
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber%8; // This give Alternate functionality number of GPIO pin
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4*temp2));
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4*temp2));
	}
}
void GPIO_Deinit(GPIO_TypeDef *pGPIOx)
{
	if(pGPIOx == GPIOA)
		GPIOA_REG_RST();
	else if(pGPIOx == GPIOB)
		GPIOB_REG_RST();
	else if(pGPIOx == GPIOC)
		GPIOC_REG_RST();
	else if(pGPIOx == GPIOD)
		GPIOD_REG_RST();
	else if(pGPIOx == GPIOE)
		GPIOE_REG_RST();
	else if(pGPIOx == GPIOF)
		GPIOF_REG_RST();
	else if(pGPIOx == GPIOG)
		GPIOG_REG_RST();
	else if(pGPIOx == GPIOH)
		GPIOH_REG_RST();

}

/*
 * Data Read and Write
 */
uint8_t GPIO_ReadInputPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber)
{
	uint8_t val;
	val = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);
	return val;
}

uint16_t GPIO_ReadInputPort(GPIO_TypeDef *pGPIOx)
{
	uint16_t val;
	val = (uint16_t)pGPIOx->IDR;
	return val;
}

void GPIO_WritetoOutputPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber, uint8_t val)
{
	if(val == GPIO_PIN_SET)
	{
		pGPIOx->ODR |= (1<<PinNumber);
	}
	else
	{
		pGPIOx->ODR &= ~(1<<PinNumber);
	}
}

void GPIO_WritetoOuputPort(GPIO_TypeDef *pGPIOx, uint16_t val)
{
	pGPIOx->ODR = val;
}

void GPIO_TogglePin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1<<PinNumber);
}


/*
 * IRQ Configuration and ISR Handling
 */
//void GPIO_IRQInterruptConfig(uint32_t IRQNumber, uint8_t EnorDi)
//{
//	if(EnorDi == ENABLE)
//	{
//		if(IRQNumber <= 31)
//		{
//			//program ISER0 register
//			//*NVIC_ISER0 |= ( 1 << IRQNumber );
//			NVIC->ISER[0] |= ( 1 << IRQNumber );
//
//		}else if(IRQNumber > 31 && IRQNumber < 64 ) //32 to 63
//		{
//			//program ISER1 register
//			//*NVIC_ISER1 |= ( 1 << (IRQNumber % 32) );
//			NVIC->ISER[1] |= ( 1 << (IRQNumber % 32) );
//		}
//		else if(IRQNumber >= 64 && IRQNumber < 96 )
//		{
//			//program ISER2 register //64 to 95
//			//*NVIC_ISER2 |= ( 1 << (IRQNumber % 64) );
//			NVIC->ISER[2] |= ( 1 << (IRQNumber % 64) );
//		}
//	}else
//	{
//		if(IRQNumber <= 31)
//		{
//			//program ICER0 register
//			*NVIC_ICER0 |= ( 1 << IRQNumber );
//		}else if(IRQNumber > 31 && IRQNumber < 64 )
//		{
//			//program ICER1 register
//			*NVIC_ICER1 |= ( 1 << (IRQNumber % 32) );
//		}
//		else if(IRQNumber >= 64 && IRQNumber < 96 )
//		{
//			//program ICER2 register
//			*NVIC_ICER2 |= ( 1 << (IRQNumber % 64) );
//		}
//	}
//
//}


void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(IRQNumber < 32)
			NVIC->ISER[0] |= (1 << IRQNumber);
		else if(IRQNumber < 64)
			NVIC->ISER[1] |= (1 << (IRQNumber % 32));

		NVIC_SetPriority(IRQNumber, IRQPriority);
	}
	else
	{
		if(IRQNumber < 32)
			NVIC->ICER[0] |= (1 << IRQNumber);
		else if(IRQNumber < 64)
			NVIC->ICER[1] |= (1 << (IRQNumber % 32));
	}
}



//void GPIO_IRQPriorityConfig(uint32_t IRQNumber, uint32_t IRQPriority)
//{
//	//1. find out the ipr register
//	uint8_t iprx = IRQNumber / 4;
//	uint8_t iprx_section  = IRQNumber %4 ;
//
//	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;
//
//	*(  NVIC_PR_BASE_ADDR + iprx ) |=  ( IRQPriority << shift_amount );
//}

void GPIO_IRQHandler(uint8_t PinNumber)
{
	//clear the EXTI PR register corresponding to the pin number
	if(EXTI->PR1 & ( 1 << PinNumber))
	{
		//clear
		EXTI->PR1 |= ( 1 << PinNumber);
	}
}



