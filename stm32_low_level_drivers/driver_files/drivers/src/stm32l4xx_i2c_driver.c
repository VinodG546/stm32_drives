/*
 * stm32l4xx_i2c_driver.c
 *
 *  Created on: Aug 28, 2025
 *      Author: VINOD
 */
#include"stm32l4xx_i2c_driver.h"

void I2C_PclkControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(pI2Cx == I2C1)
        {
            I2C1_PCLK_EN();
        }
        else if(pI2Cx == I2C2)
        {
            I2C2_PCLK_EN();
        }
        else if(pI2Cx == I2C3)
        {
            I2C3_PCLK_EN();
        }
    }
    else
    {

    }
}
/*
 * I2C Init & Deinit
 */
void I2C_Init(I2C_Handle_t *pI2CHandle)
{


}
void I2C_Deinit(I2C_RegDef_t *pI2Cx)
{
	if(pI2Cx == I2C1)
	{
		I2C1_REG_RESET();
	}
	else if(pI2Cx == I2C2)
	{
		I2C2_REG_RESET();
	}
	else if(pI2Cx == I2C3)
	{
		I2C3_REG_RESET();
	}
}

/*
 * Data Send and Receive
 */
void I2C_MasterSendData(I2C_RegDef_t *pI2Cx, uint8_t *pTxBuffer, uint32_t len,uint8_t SlaveAddr)
{

}
void I2C_MasterReceiveData(I2C_RegDef_t *pI2Cx, uint8_t *pRxBuffer, uint32_t len,uint8_t SlaveAddr)
{

}

/*
 * Interrupt mode Transmission and Reception
 */

/*
 * IRQ Configuration and ISR handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{

}
uint8_t I2C_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{

}


void I2C_IRQHandling(I2C_RegDef_t *pI2CHandle)
{

}

/*
 * Other Peripheral Control API's
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi)
{

}

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName)
{

}

