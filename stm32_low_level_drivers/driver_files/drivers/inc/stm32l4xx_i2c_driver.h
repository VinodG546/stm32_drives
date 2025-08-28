/*
 * stm32l4xx_i2c_driver.h
 *
 *  Created on: Aug 28, 2025
 *      Author: VINOD
 */

#ifndef INC_STM32L4XX_I2C_DRIVER_H_
#define INC_STM32L4XX_I2C_DRIVER_H_

#include"stm32l4xx.h"

typedef struct
{
	uint32_t I2C_SCLSpeed;					// @I2C_SCLSpeed
	uint8_t I2C_DeviceAddress;
	uint8_t I2C_ACKControl;					// @I2C_AckControl
	uint16_t I2C_FMDutyCycle;				// @I2C_FMDutyCycle
}I2C_Config_t;

/*
 * Handle structure for I2Cx peripheral
 */
typedef struct
{
	I2C_RegDef_t		*pI2Cx;
	I2C_Config_t	I2C_config;
}I2C_Handle_t;


/*
 * I2C application states
 */
#define I2C_BUSY    1
#define I2C_READY 					0
#define I2C_BUSY_IN_RX 				1
#define I2C_BUSY_IN_TX 				2

/*
 * @I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_SM 	100000
#define I2C_SCL_SPEED_FM4K 	400000
#define I2C_SCL_SPEED_FM2K  200000


/*
 * @I2C_AckControl
 */
#define I2C_ACK_ENABLE        1
#define I2C_ACK_DISABLE       0


/*
 * @I2C_FMDutyCycle
 */
#define I2C_FM_DUTY_2        0
#define I2C_FM_DUTY_16_9     1

/*
 * I2C related status flags definitions
 */
#define I2C_FLAG_TXE   		I2C_ISR_TXE
#define I2C_FLAG_TXIS		I2C_ISR_TXIS
#define I2C_FLAG_TC			I2C_ISR_TC
#define I2C_FLAG_RXNE   	I2C_ISR_RXNE
//#define I2C_FLAG_SB			( 1 << I2C_ISR_SB)
#define I2C_FLAG_BUSY		I2C_ISR_BUSY
#define I2C_FLAG_OVR  		I2C_ISR_OVR
//#define I2C_FLAG_AF   		( 1 << I2C_ISR_AF)
#define I2C_FLAG_ARLO 		I2C_ISR_ARLO
#define I2C_FLAG_BERR 		I2C_ISR_BERR
#define I2C_FLAG_STOPF 		I2C_ISR_STOPF
//#define I2C_FLAG_ADD10 		( 1 << I2C_ISR_ADD10)
#define I2C_FLAG_ADD10		I2C_ISR_ADDCODE
//#define I2C_FLAG_BTF  		( 1 << I2C_ISR_BTF)
#define I2C_FLAG_ADDR 		I2C_ISR_ADDR
#define I2C_FLAG_TIMEOUT 	I2C_ISR_TIMEOUT

#define I2C_DISABLE_SR  	RESET
#define I2C_ENABLE_SR   	SET


/****************************************API's for I2C drivers************************************************************/
/*
 * Peripheral Clock Setup
 */
void I2C_PclkControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/*
 * I2C Init & Deinit
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_Deinit(I2C_RegDef_t *pI2Cx);

/*
 * Data Send and Receive
 */
void I2C_MasterSendData(I2C_RegDef_t *pI2Cx, uint8_t *pTxBuffer, uint32_t len,uint8_t SlaveAddr);
void I2C_MasterReceiveData(I2C_RegDef_t *pI2Cx, uint8_t *pRxBuffer, uint32_t len,uint8_t SlaveAddr);

/*
 * Interrupt mode Transmission and Reception
 */

/*
 * IRQ Configuration and ISR handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
uint8_t I2C_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);


void I2C_IRQHandling(I2C_RegDef_t *pI2CHandle);

/*
 * Other Peripheral Control API's
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName);
#endif /* INC_STM32L4XX_I2C_DRIVER_H_ */
