/*
 * stm32l4xx_uart_driver.h
 *
 *  Created on: Sep 3, 2025
 *      Author: VINOD
 */

#ifndef INC_STM32L4XX_UART_DRIVER_H_
#define INC_STM32L4XX_UART_DRIVER_H_

#include<stdint.h>
#include"stm32l4xx.h"
/*
 * Configuration structure for I2Cx peripheral
 */
typedef struct
{
	uint8_t USART_Mode;
	uint32_t USART_Baud;
	uint8_t USART_NoOfStopBits;
	uint8_t USART_WordLength;
	uint8_t USART_ParityControl;
	uint8_t USART_HWFlowControl;
}USART_Config_t;


/*
 * Handle structure for USARTx peripheral
 */
typedef struct
{
	USART_TypeDef *pUSARTx;
	USART_Config_t   USART_Config;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxBusyState;
	uint8_t RxBusyState;
}USART_Handle_t;

/*
 *@USART_Mode
 *Possible options for USART_Mode
 */
#define USART_MODE_ONLY_TX 	0
#define USART_MODE_ONLY_RX 	1
#define USART_MODE_TXRX  	2

/*
 *@USART_Baud
 *Possible options for USART_Baud
 */
#define USART_STD_BAUD_1200					1200
#define USART_STD_BAUD_2400					400
#define USART_STD_BAUD_9600					9600
#define USART_STD_BAUD_19200 				19200
#define USART_STD_BAUD_38400 				38400
#define USART_STD_BAUD_57600 				57600
#define USART_STD_BAUD_115200 				115200
#define USART_STD_BAUD_230400 				230400
#define USART_STD_BAUD_460800 				460800
#define USART_STD_BAUD_921600 				921600
#define USART_STD_BAUD_2M 					2000000
#define SUART_STD_BAUD_3M 					3000000


/*
 *@USART_ParityControl
 *Possible options for USART_ParityControl
 */
#define USART_PARITY_EN_ODD   2
#define USART_PARITY_EN_EVEN  1
#define USART_PARITY_DISABLE   0

/*
 *@USART_WordLength
 *Possible options for USART_WordLength
 */
#define USART_WORDLEN_8BITS  0
#define USART_WORDLEN_9BITS  1

/*
 *@USART_NoOfStopBits
 *Possible options for USART_NoOfStopBits
 */
#define USART_STOPBITS_1     0
#define USART_STOPBITS_0_5   1
#define USART_STOPBITS_2     2
#define USART_STOPBITS_1_5   3


/*
 *@USART_HWFlowControl
 *Possible options for USART_HWFlowControl
 */
#define USART_HW_FLOW_CTRL_NONE    	0
#define USART_HW_FLOW_CTRL_CTS    	1
#define USART_HW_FLOW_CTRL_RTS    	2
#define USART_HW_FLOW_CTRL_CTS_RTS	3


/*
 * USART flags
 */

#define USART_FLAG_TXE 			USART_ISR_TXE
#define USART_FLAG_RXNE 		USART_ISR_RXNE
#define USART_FLAG_TC 			USART_ISR_TC

/*
 * Application states
 */
#define USART_BUSY_IN_RX 		1
#define USART_BUSY_IN_TX 		2
#define USART_READY 			0


#define 	USART_EVENT_TX_CMPLT  		 0
#define		USART_EVENT_RX_CMPLT  		 1
#define		USART_EVENT_IDLE    		 2
#define		USART_EVENT_CTS      		 3
#define		USART_EVENT_PE       		 4
#define		USART_ERR_FE     			 5
#define		USART_ERR_NE    			 6
#define		USART_ERR_ORE    			 7

/****************************************API's for USART drivers************************************************************/

/*
 * Peripheral Clock setup
 */
void USART_PeriClockControl(USART_TypeDef *pUSARTx, uint8_t EnOrDi);

/*
 * Init and De-init
 */
void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_TypeDef *pUSARTx);

/*
 * Data Send and Receive
 */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t len);
void  USART_ReceiveData(USART_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t len);
void USART_SendReceive(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint8_t *pRxBuffer, uint32_t len);
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t len);

/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);
void USART_IRQHandling(USART_Handle_t *pUSARTHandle);

/*
 * Other Peripheral Control APIs
 */

uint8_t USART_GetFlagStatus(USART_TypeDef *pUSARTx, uint8_t StatusFlagName);
void USART_ClearFlag(USART_TypeDef *pUSARTx, uint16_t StatusFlagName);
void USART_PeripheralControl(USART_TypeDef *pUSARTx, uint8_t EnOrDi);
void USART_SetBaudRate(USART_TypeDef *pUSARTx, uint32_t BaudRate);


/*
 * Application Callbacks
 */
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t ApEv);

#endif /* INC_STM32L4XX_UART_DRIVER_H_ */
