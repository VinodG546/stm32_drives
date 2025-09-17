
#ifndef INC_GPIO_DRIVER_H_
#define INC_GPIO_DRIVER_H_

#define __vo		volatile

#include "stm32l476xx.h"
#include <stdint.h>
#include <stddef.h>


/*
 * ARM Cortex Mx Processor NVIC ISERx register Addresses
 */

#define NVIC_ISER0          ( (__vo uint32_t*)0xE000E100 )
#define NVIC_ISER1          ( (__vo uint32_t*)0xE000E104 )
#define NVIC_ISER2          ( (__vo uint32_t*)0xE000E108 )
#define NVIC_ISER3          ( (__vo uint32_t*)0xE000E10c )


/*
 * ARM Cortex Mx Processor NVIC ICERx register Addresses
 */
#define NVIC_ICER0 			((__vo uint32_t*)0XE000E180)
#define NVIC_ICER1			((__vo uint32_t*)0XE000E184)
#define NVIC_ICER2  		((__vo uint32_t*)0XE000E188)
#define NVIC_ICER3			((__vo uint32_t*)0XE000E18C)


/*
 * ARM Cortex Mx Processor Priority Register Address Calculation
 */
#define NVIC_PR_BASE_ADDR 	((__vo uint32_t*)0xE000E400)

/*
 * ARM Cortex Mx Processor number of priority bits implemented in Priority Register
 */
#define NO_PR_BITS_IMPLEMENTED  4

/*
 * Enable clock for All GPIO ports
 */
#define GPIOA_PCLK_EN() 	(RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN)
#define GPIOB_PCLK_EN() 	(RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN)
#define GPIOC_PCLK_EN() 	(RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN)
#define GPIOD_PCLK_EN() 	(RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN)
#define GPIOE_PCLK_EN() 	(RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN)
#define GPIOF_PCLK_EN() 	(RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN)
#define GPIOG_PCLK_EN() 	(RCC->AHB2ENR |= RCC_AHB2ENR_GPIOGEN)
#define GPIOH_PCLK_EN() 	(RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN)

/*
 * Disable Clock for All the GPIO Ports
 */
#define GPIOA_PCLK_DI() 	(RCC->AHB2ENR &= ~(RCC_AHB2ENR_GPIOAEN))
#define GPIOB_PCLK_DI() 	(RCC->AHB2ENR &= ~(RCC_AHB2ENR_GPIOBEN))
#define GPIOC_PCLK_DI() 	(RCC->AHB2ENR &= ~(RCC_AHB2ENR_GPIOCEN))
#define GPIOD_PCLK_DI() 	(RCC->AHB2ENR &= ~(RCC_AHB2ENR_GPIODEN))
#define GPIOE_PCLK_DI() 	(RCC->AHB2ENR &= ~(RCC_AHB2ENR_GPIOEEN))
#define GPIOF_PCLK_DI() 	(RCC->AHB2ENR &= ~(RCC_AHB2ENR_GPIOFEN))
#define GPIOG_PCLK_DI() 	(RCC->AHB2ENR &= ~(RCC_AHB2ENR_GPIOGEN))
#define GPIOH_PCLK_DI() 	(RCC->AHB2ENR &= ~(RCC_AHB2ENR_GPIOHEN))

/*
 * Reset RCC clock configuration for all GPIO ports
 */
#define GPIOA_REG_RST()		do{(RCC->AHB2RSTR |= RCC_AHB2RSTR_GPIOARST); (RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_GPIOARST));}while(0)
#define GPIOB_REG_RST()		do{(RCC->AHB2RSTR |= RCC_AHB2RSTR_GPIOBRST); (RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_GPIOBRST));}while(0)
#define GPIOC_REG_RST()		do{(RCC->AHB2RSTR |= RCC_AHB2RSTR_GPIOCRST); (RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_GPIOCRST));}while(0)
#define GPIOD_REG_RST()		do{(RCC->AHB2RSTR |= RCC_AHB2RSTR_GPIODRST); (RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_GPIODRST));}while(0)
#define GPIOE_REG_RST()		do{(RCC->AHB2RSTR |= RCC_AHB2RSTR_GPIOERST); (RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_GPIOERST));}while(0)
#define GPIOF_REG_RST()		do{(RCC->AHB2RSTR |= RCC_AHB2RSTR_GPIOFRST); (RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_GPIOFRST));}while(0)
#define GPIOG_REG_RST()		do{(RCC->AHB2RSTR |= RCC_AHB2RSTR_GPIOGRST); (RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_GPIOGRST));}while(0)
#define GPIOH_REG_RST()		do{(RCC->AHB2RSTR |= RCC_AHB2RSTR_GPIOHRST); (RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_GPIOHRST));}while(0)


#define SYSCFG_PCLK_EN() 	(RCC->APB2ENR  |= RCC_APB2ENR_SYSCFGEN)

#define GPIO_BASEADDR_TO_CODE(x)		((x==GPIOA) ? 0:\
										 (x==GPIOB) ? 1:\
										 (x==GPIOC) ? 2:\
										 (x==GPIOD) ? 3:\
										 (x==GPIOE) ? 4:\
										 (x==GPIOF) ? 5:\
										 (x==GPIOG) ? 6:\
										 (x==GPIOH) ? 7:0 )
/*
 * Generic Macros used in driver development
 */
#define ENABLE			1
#define DISABLE			0
#define SET				ENABLE
#define RESET			DISABLE
#define GPIO_PIN_SET	SET
#define GPIO_PIN_RESET	RESET



typedef struct
{
	uint8_t GPIO_PinNumber;			// @GPIO_Pin_Number
	uint8_t GPIO_PinMode;			// @GPIO_Pin_Modes
	uint8_t GPIO_PinSpeed;			// @GPIO_PIN_SPEED
	uint8_t GPIO_PinPuPdControl;	// @GPIO_PUPD
	uint8_t GPIO_PinOPType;			// @GPIO_Pin_Type
	uint8_t GPIO_PinAltFunMode;

}GPIO_PinConfig_t;

typedef struct {
	GPIO_TypeDef *pGPIOx;		/*This holds the base addres of GPIO port to which the actually belongs to*/
	GPIO_PinConfig_t GPIO_PinConfig;	/*This holds the GPIO pin configuration settings*/
}GPIO_Handle_t;

/*
 * @GPIO_Pin_Number
 * GPIO Pin Numbers
 */

#define GPIO_PIN_NO_0  			0
#define GPIO_PIN_NO_1  			1
#define GPIO_PIN_NO_2  			2
#define GPIO_PIN_NO_3  			3
#define GPIO_PIN_NO_4  			4
#define GPIO_PIN_NO_5  			5
#define GPIO_PIN_NO_6  			6
#define GPIO_PIN_NO_7  			7
#define GPIO_PIN_NO_8  			8
#define GPIO_PIN_NO_9  			9
#define GPIO_PIN_NO_10  		10
#define GPIO_PIN_NO_11 			11
#define GPIO_PIN_NO_12  		12
#define GPIO_PIN_NO_13 			13
#define GPIO_PIN_NO_14 			14
#define GPIO_PIN_NO_15 			15

/*
 * @GPIO_Pin_Modes
 * GPIO Pin possible Modes
 */
#define GPIO_MODE_IN		0
#define GPIO_MODE_OUT		1
#define GPIO_MODE_AFN		2
#define GPIO_MODE_ANALOG	3	//Analog mode
#define GPIO_MODE_IT_FE		4	//External interrupt Falling Edge
#define GPIO_MODE_IT_RE		5	//External interrupt Rising Edge
#define GPIO_MODE_IT_RFE	6	//Both Rising and falling edge

/*
 * @GPIO_Pin_Type
 * GPIO pin possible output types
 */
#define GPIO_OP_TYPE_PP   0
#define GPIO_OP_TYPE_OD   1

/*
 * @GPIO_PIN_SPEED
 * GPIO pin possible output speeds
 */
#define GPIO_SPEED_LOW			0
#define GPIO_SPEED_MEDIUM		1
#define GPIO_SPEED_FAST			2
#define GPOI_SPEED_HIGH			3

/*
 * @GPIO_PUPD
 * GPIO pin pull up AND pull down configuration macros
 */
#define GPIO_NO_PUPD   		0
#define GPIO_PIN_PU			1
#define GPIO_PIN_PD			2

/****************************************API's for GPIO drivers************************************************************/
/*
 * Peripheral Clock Setup
 */
void GPIO_PclkControl(GPIO_TypeDef *pGPIOx, uint8_t EnorDi);

/*
 * GPIO Init & Deinit
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_Deinit(GPIO_TypeDef *pGPIOx);

/*
 * Data Read and Write
 */
uint8_t GPIO_ReadInputPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadInputPort(GPIO_TypeDef *pGPIOx);
void GPIO_WritetoOutputPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber, uint8_t val);
void GPIO_WritetoOuputPort(GPIO_TypeDef *pGPIOx, uint16_t val);
void GPIO_TogglePin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber);

/*
 * IRQ Configuration and ISR Handling
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);
//void GPIO_IRQInterruptConfig(uint32_t IRQNumber, uint8_t EnorDi);
//void GPIO_IRQPriorityConfig(uint32_t IRQNumber, uint32_t IRQPriority);
void GPIO_IRQHandler(uint8_t PinNumber);



#endif /* INC_GPIO_DRIVER_H_ */
