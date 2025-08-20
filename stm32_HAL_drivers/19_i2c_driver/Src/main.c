#include "stm32l4xx_hal.h"


I2C_HandleTypeDef hi2c1,hi2c2;

uint8_t tx_buffer[10] = {1,2,3,4,5,6,7,8,9,10};


void i2c_master_init();

/*void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C2)
    {
        // Data received into rx_buffer
        HAL_I2C_Slave_Receive_IT(&hi2c2, &rx_buffer, sizeof(rx_buffer));
    }
}*/

int main()
{
	HAL_Init();
	i2c_master_init();

	HAL_I2C_Master_Transmit(&hi2c1, (0x55 << 1), tx_buffer, sizeof(tx_buffer), HAL_MAX_DELAY);

	while(1)
	{

	}
}

void i2c_master_init(){
	/*
	 * PB6	-->	I2C_SCL
	 * PB7	--> I2C_SDA
	 */

	//Enable clock for GPIOB and I2C
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_Initstruct = {0};
	GPIO_Initstruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_Initstruct.Mode = GPIO_MODE_AF_OD;
	GPIO_Initstruct.Alternate = GPIO_AF4_I2C1;
	GPIO_Initstruct.Pull = GPIO_PULLUP;
	GPIO_Initstruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;


	HAL_GPIO_Init(GPIOB,&GPIO_Initstruct);

	hi2c1.Instance = I2C1;
	//hi2c1.Init.ClockSpeed  = 400000;
	hi2c1.Init.Timing = 0x00b1112e;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	HAL_I2C_Init(&hi2c1);

}
/*void i2c_slave_init(){

//	 * PB10	-->	I2C_SCL
//	 * PB11 --> I2C_SDA


	//Enable clock for GPIOB and I2C
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_I2C2_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_Initstruct = {0};
	GPIO_Initstruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_Initstruct.Mode = GPIO_MODE_AF_OD;
	GPIO_Initstruct.Alternate = GPIO_AF4_I2C2;
	GPIO_Initstruct.Pull = GPIO_NOPULL;
	GPIO_Initstruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;


	HAL_GPIO_Init(GPIOB,&GPIO_Initstruct);

	hi2c2.Instance = I2C2;
	//hi2c1.Init.ClockSpeed  = 400000;
	hi2c2.Init.Timing = 0x00b1112e;
	hi2c2.Init.OwnAddress1 = 0x52;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	HAL_I2C_Init(&hi2c2);
	HAL_NVIC_SetPriority(I2C2_EV_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
	HAL_NVIC_SetPriority(I2C2_ER_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);

}

void I2C2_EV_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(&hi2c2);
}

void I2C2_ER_IRQHandler(void)
{
    HAL_I2C_ER_IRQHandler(&hi2c2);
}*/

void SysTick_Handler(void)
{
	HAL_IncTick();
}

