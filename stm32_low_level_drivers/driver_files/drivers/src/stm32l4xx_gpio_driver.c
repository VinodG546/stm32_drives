#include "stm32l4xx_gpio_driver.h"

// Peripheral clock setup
/**
  * @fn        - GPIO_PeriClockControl
  * @brief     - This function enables or disables the peripheral clock for the given GPIO port.
  * @param[in] pGPIOx  - Base address of GPIO peripheral (e.g., GPIOA, GPIOB, etc.)
  * @param[in] EnorDi  - Enable or disable macros: ENABLE = 1, DISABLE = 0
  * @return    - None
  * @note      - Must be called before accessing GPIO registers.
  */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pGPIOx==GPIOA){
			GPIOA_PCLK_EN();
		}
		else if(pGPIOx==GPIOB){
			GPIOB_PCLK_EN();
		}
		else if(pGPIOx==GPIOC){
			GPIOC_PCLK_EN();
		}
		else if(pGPIOx==GPIOD){
			GPIOD_PCLK_EN();
		}
		else if(pGPIOx==GPIOE){
			GPIOE_PCLK_EN();
		}
		else if(pGPIOx==GPIOF){
			GPIOF_PCLK_EN();
		}
		else if(pGPIOx==GPIOG){
			GPIOG_PCLK_EN();
		}
		else if(pGPIOx==GPIOH){
			GPIOH_PCLK_EN();
		}
	}
	else{
		if(pGPIOx==GPIOA){
			GPIOA_PCLK_DI();
		}
		else if(pGPIOx==GPIOB){
			GPIOB_PCLK_DI();
		}
		else if(pGPIOx==GPIOC){
			GPIOC_PCLK_DI();
		}
		else if(pGPIOx==GPIOD){
			GPIOD_PCLK_DI();
		}
		else if(pGPIOx==GPIOE){
			GPIOE_PCLK_DI();
		}
		else if(pGPIOx==GPIOF){
			GPIOF_PCLK_DI();
		}
		else if(pGPIOx==GPIOG){
			GPIOG_PCLK_DI();
		}
		else if(pGPIOx==GPIOH){
			GPIOH_PCLK_DI();
		}
	}
}

// Init and deinit GPIO
/**
  * @fn        - GPIO_Init
  * @brief     - Initializes the GPIO pin according to the configuration in GPIO handle.
  * @param[in] pGPIOHandle - Handle structure that contains GPIO configuration information.
  * @return    - None
  * @note      - Make sure peripheral clock for GPIO is enabled before calling this function.
  */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
    uint32_t temp = 0;

    // 1. Configure the mode of GPIO pin
    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
    {
        // non-interrupt mode
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
        pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
        pGPIOHandle->pGPIOx->MODER |= temp;
        temp = 0;
    }
    else
    {
        // interrupt mode
        if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
        {
        	EXTI->FTSR1 |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        	EXTI->RTSR1 &= ~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

        }
        else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
        {
        	EXTI->RTSR1 |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        	EXTI->FTSR1 &= ~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }
        else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
        {
        	EXTI->FTSR1 |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        	EXTI->RTSR1 |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }
        uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
        uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
        uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
        SYSCFG->EXTICR[temp1] &= ~(0xF << (temp2 * 4));
        SYSCFG -> EXTICR[temp1] = portcode << (temp2 * 4);

        EXTI -> IMR1 |= 1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
    }

    // 2. Configure speed
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->OSPEEDR |= temp;
    temp = 0;

    // 3. Configure pull-up/pull-down
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->PUPDR |= temp;
    temp = 0;

    // 4. Configure output type
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    pGPIOHandle->pGPIOx->OTYPER |= temp;
    temp = 0;

    // 5. Configure alternate function if needed
    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
    {
        uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;  // AFR[0] or AFR[1]
        uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;  // Position within register

        pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2));
        pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2));
    }
}


/**
  * @fn        - GPIO_DeInit
  * @brief     - Resets all registers of the given GPIO port.
  * @param[in] pGPIOx - Base address of GPIO peripheral (e.g., GPIOA, GPIOB, etc.)
  * @return    - None
  * @note      - Internally uses RCC reset register macros.
  */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){
			if(pGPIOx==GPIOA){
				GPIOA_REG_RESET();
			}
			else if(pGPIOx==GPIOB){
				GPIOB_REG_RESET();
			}
			else if(pGPIOx==GPIOC){
				GPIOC_REG_RESET();
			}
			else if(pGPIOx==GPIOD){
				GPIOD_REG_RESET();
			}
			else if(pGPIOx==GPIOE){
				GPIOE_REG_RESET();
			}
			else if(pGPIOx==GPIOF){
				GPIOF_REG_RESET();
			}
			else if(pGPIOx==GPIOG){
				GPIOG_REG_RESET();
			}
			else if(pGPIOx==GPIOH){
				GPIOH_REG_RESET();
			}
}


// APIs for reading and writing into/from GPIOs
/**
  * @fn        - GPIO_ReadFromInputPin
  * @brief     - Reads the logic level (0 or 1) from a specific GPIO pin.
  * @param[in] pGPIOx       - Base address of GPIO port.
  * @param[in] GPIO_PinNumber - Pin number (0-15).
  * @return    - uint8_t: 0 (LOW) or 1 (HIGH).
  * @note      - None
  */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t GPIO_PinNumber){
	uint8_t value;
	value = (uint8_t)((pGPIOx->IDR >> GPIO_PinNumber) & 1);
	return value;
}

/**
  * @fn        - GPIO_ReadFromInputPort
  * @brief     - Reads the entire 16-bit value from the GPIO input data register.
  * @param[in] pGPIOx - Base address of GPIO port.
  * @return    - uint16_t: 16-bit port input value.
  * @note      - None
  */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx){
	uint16_t value;
	value = (uint16_t)(pGPIOx->IDR);
	return value;
}

/**
  * @fn        - GPIO_WriteToOutputPin
  * @brief     - Writes a logic value (0 or 1) to a specific GPIO output pin.
  * @param[in] pGPIOx       - Base address of GPIO port.
  * @param[in] GPIO_PinNumber - Pin number (0-15).
  * @param[in] value        - 0 = RESET (LOW), 1 = SET (HIGH).
  * @return    - None
  * @note      - None
  */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t GPIO_PinNumber, uint32_t value){
	if( value == GPIO_PIN_SET){
		pGPIOx->ODR |= (1<<GPIO_PinNumber);
	}
	else{
		pGPIOx->ODR &= ~(1<<GPIO_PinNumber);
	}
}

/**
  * @fn        - GPIO_WriteToOutputPort
  * @brief     - Writes a 16-bit value to the entire GPIO output data register.
  * @param[in] pGPIOx - Base address of GPIO port.
  * @param[in] value  - 16-bit value to be written on port pins.
  * @return    - None
  * @note      - Overwrites all pin states of the port.
  */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint32_t value){
	pGPIOx->ODR = value;
}

/**
  * @fn        - GPIO_ToggleOutputPin
  * @brief     - Toggles the output state of the given GPIO pin.
  * @param[in] pGPIOx       - Base address of GPIO port.
  * @param[in] GPIO_PinNumber - Pin number (0-15).
  * @return    - None
  * @note      - Useful for blinking LEDs or toggling signals.
  */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t GPIO_PinNumber){
	pGPIOx->ODR ^= (1<<GPIO_PinNumber);

}


// APIs for IRQ configuration and handling
/**
  * @fn        - GPIO_IRQConfig
  * @brief     - Configures the IRQ number and enables/disables it in NVIC.
  * @param[in] IRQNumber   - IRQ number to configure (from MCU vector table).
  * @param[in] IRQPriority - Priority of the IRQ (lower number = higher priority).
  * @param[in] EnorDi      - ENABLE = 1, DISABLE = 0.
  * @return    - None
  * @note      - Must also configure EXTI line for GPIO before enabling IRQ.
  */
void GPIO_IRQ_interrupt_Config(uint8_t IRQNumber, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(IRQNumber <= 31){
			*NVIC_ISER0 |= (1<<IRQNumber);
		}
		else if(IRQNumber >=32 && IRQNumber < 64){
			*NVIC_ISER1 |= (1<<(IRQNumber%32));
		}
		else if(IRQNumber >=64 && IRQNumber < 96){
			*NVIC_ISER0 |= (1<<(IRQNumber%64));
		}
	}
	else{
		if(IRQNumber <= 31){
			*NVIC_ICER0 |= (1<<IRQNumber);
		}
		else if(IRQNumber >=32 && IRQNumber < 64){
			*NVIC_ICER1 |= (1<<(IRQNumber%32));
		}
		else if(IRQNumber >=64 && IRQNumber < 96){
			*NVIC_ICER2 |= (1<<(IRQNumber%64));
		}
	}
}
void GPIO_IRQ_priority_Config(uint8_t IRQNumber,uint8_t priority){
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	uint8_t shift_amount  = (8*iprx_section) + (8-NO_PR_BITS_IMPLEMENTED);

	*((NVIC_PR_BASE_ADDR+(iprx*4))) |= (priority << shift_amount);

}


/**
  * @fn        - GPIO_IRQHandling
  * @brief     - Handles the interrupt generated on the given pin number.
  * @param[in] GPIO_PinNumber - Pin number (0-15) which triggered the interrupt.
  * @return    - None
  * @note      - Clears the EXTI pending register bit corresponding to the pin.
  */
void GPIO_IRQHandling(uint8_t GPIO_PinNumber){
	if(EXTI->PR1 & (1<<GPIO_PinNumber))
	{
		EXTI->PR1  |=  (1<<GPIO_PinNumber);
	}
}
