
#ifndef INC_SPI_DRIVER_H_
#define INC_SPI_DRIVER_H_

#include "stm32l4xx_gpio_driver.h"

extern uint8_t spi_tx_done;
extern uint8_t spi_rx_done;

//RCC clock enable for SPIx
#define SPI1_PCLK_EN()		(RCC->APB2ENR |= RCC_APB2ENR_SPI1EN)
#define SPI2_PCLK_EN()		(RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN)
#define SPI3_PCLK_EN()		(RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN)

//RCC clock Disable for SPIx
#define SPI1_PCLK_DI()		(RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN)
#define SPI2_PCLK_DI()		(RCC->APB1ENR1 &= ~RCC_APB1ENR1_SPI2EN)
#define SPI3_PCLK_DI()		(RCC->APB1ENR1 &= ~RCC_APB1ENR1_SPI3EN)

/*
 * Reset RCC clock configuration for all SPIx ports
 */
#define SPI1_REG_RST()		do{(RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST); (RCC->APB2RSTR &= ~(RCC_APB2RSTR_SPI1RST));}while(0)
#define SPI2_REG_RST()		do{(RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI2RST); (RCC->APB1RSTR1 &= ~(RCC_APB1RSTR1_SPI2RST));}while(0)
#define SPI3_REG_RST()		do{(RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI3RST); (RCC->APB1RSTR1 &= ~(RCC_APB1RSTR1_SPI3RST));}while(0)

typedef struct
{
	uint8_t SPI_DeviceMode;				//@SPI_DeviceMode
	uint8_t SPI_BusConfig;				//@SPI_BusConfig
	uint8_t SPI_SclkSpeed;				//@SPI_SclkSpeed
	uint8_t SPI_CPOL;					// @CPOL
	uint8_t SPI_CPHA;					//@CPHA
	uint8_t SPI_SSM;					//@SPI_SSM
}SPI_Config_t;


typedef struct
{
	SPI_TypeDef 	*pSPIx; //Holds base address of the SPIx Peripherals
	SPI_Config_t 	SPIConfig;
	uint8_t 		*pTxBuffer;
	uint8_t 		*pRxBuffer;
	uint32_t 		TxLen;
	uint32_t 		RxLen;
	uint8_t 		TxState;

}SPI_Handle_t;

/*
 * SPI application states
 */
#define SPI_READY 					0
#define SPI_BUSY_IN_RX 				1
#define SPI_BUSY_IN_TX 				2

/*
 * Possible SPI Application events
 */
#define SPI_EVENT_TX_CMPLT   1
#define SPI_EVENT_RX_CMPLT   2
#define SPI_EVENT_OVR_ERR    3
#define SPI_EVENT_CRC_ERR    4



/*
 * @SPI_DeviceMode
 */
#define SPI_DEVICE_MODE_MASTER    1
#define SPI_DEVICE_MODE_SLAVE     0


/*
 * @SPI_BusConfig
 */
#define SPI_BUS_CONFIG_FD                1
#define SPI_BUS_CONFIG_HD                2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY    3

/*
 * @SPI_SclkSpeed
 */
#define SPI_SCLK_SPEED_DIV2             	0
#define SPI_SCLK_SPEED_DIV4             	1
#define SPI_SCLK_SPEED_DIV8             	2
#define SPI_SCLK_SPEED_DIV16             	3
#define SPI_SCLK_SPEED_DIV32             	4
#define SPI_SCLK_SPEED_DIV64             	5
#define SPI_SCLK_SPEED_DIV128             	6
#define SPI_SCLK_SPEED_DIV256             	7

/*
 * @CPOL
 */
#define SPI_CPOL_HIGH 1
#define SPI_CPOL_LOW 0

/*
 * @CPHA
 */
#define SPI_CPHA_HIGH 1
#define SPI_CPHA_LOW 0

/*
 * @SPI_SSM
 */
#define SPI_SSM_EN     1
#define SPI_SSM_DI     0


/*
 * SPI related status flags definitions
 */
#define SPI_TXE_FLAG    	SPI_SR_TXE
#define SPI_RXNE_FLAG   	SPI_SR_RXNE
#define SPI_BUSY_FLAG   	SPI_SR_BSY

#define SPI_TXEIE          SPI_CR2_TXEIE		//(1 << 7)
#define SPI_RXNEIE         SPI_CR2_RXNEIE		//(1 << 6)
#define SPI_ERRIE          SPI_CR2_ERRIE		//(1 << 5)



#define FLAG_RESET		RESET
#define FLAG_SET		SET

/****************************************API's for SPI drivers************************************************************/
/*
 * Peripheral Clock Setup
 */
void SPI_PclkControl(SPI_TypeDef *pSPIx, uint8_t EnorDi);

/*
 * I2C Init & Deinit
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_Deinit(SPI_TypeDef *pSPIx);

/*
 * Data Send and Receive
 */
void SPI_SendData(SPI_TypeDef *pSPIx, uint8_t *pTxBuffer, uint32_t len);
void SPI_ReceiveData(SPI_TypeDef *pSPIx, uint8_t *pRxBuffer, uint32_t len);
void SPI_TransmitReceive(SPI_TypeDef *pSPIx, uint8_t *pTxBuffer, uint8_t *pRxBuffer, uint32_t len);

/*
 * Interrupt mode Transmission and Reception
 */
uint8_t SPI_SendData_IT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len);
uint8_t SPI_ReceiveData_IT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len);

void spi_rxe_IT_handle(SPI_Handle_t *pSPIHandle);
void spi_txe_IT_handle(SPI_Handle_t *pSPIHandle);

/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);
//void SPI_IRQInterruptConfig(uint32_t IRQNumber, uint8_t EnorDi);
//void SPI_IRQPriorityConfig(uint32_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);

//void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv);
/*
 * Other Peripheral Control API's
 */
void SPI_PeripheralControl(SPI_TypeDef *pSPIx, uint8_t EnOrDi);
void SPI_SSIConfig(SPI_TypeDef *pSPIx, uint8_t EnOrDi);
void SPI_SSOEConfig(SPI_TypeDef *pSPIx, uint8_t EnOrDi);
uint8_t SPI_GetFlagStatus(SPI_TypeDef *pSPIx , uint32_t FlagName);

#endif /* INC_SPI_DRIVER_H_ */
