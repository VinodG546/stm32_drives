/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include<stdio.h>
#include<string.h>
#include<stdarg.h>
#include<inttypes.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
#define FLASH_USER_START_ADDR   0x08080000
#define MAX_LOGS  10
#define LOG_SLOT_SIZE  256  // bytes

uint8_t log_index = 0;   // next slot index
uint8_t log_full = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void DumpAllFlash(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t IC_Val1 = 0;
uint32_t IC_Val2 = 0;
uint32_t Difference = 0;
uint8_t Is_First_Captured = 0;  // is the first value captured ?
uint16_t Distance  = 0 , object_present;
uint8_t str[]="hello world";
uint8_t first[]="object entered into range";
uint8_t second[]="object removed at ";
uint8_t cnt=0;
#define TRIG_PIN GPIO_PIN_15
#define TRIG_PORT GPIOB


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // if the interrupt source is channel1
	{
		if (Is_First_Captured==0) // if the first value is not captured
		{
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
			Is_First_Captured = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (Is_First_Captured==1)   // if the first is already captured
		{
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

			if (IC_Val2 > IC_Val1)
			{
				Difference = IC_Val2-IC_Val1;
			}

			else if (IC_Val1 > IC_Val2)
			{
				Difference = (0xffff - IC_Val1) + IC_Val2;
			}

			Distance = Difference * .034/2;
			Is_First_Captured = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);
		}
	}
}

uint16_t HCSR04_Read(void)
{
    uint32_t timeout = 0;

    // Send 10us pulse
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);

    // crude ~10us delay (assuming 80 MHz core, 100 iterations ≈ 1 us)
    for (volatile int i = 0; i < 800; i++);

    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);

    // enable capture interrupt
    __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);

    // wait for measurement to complete or timeout (~30 ms max)
    timeout = HAL_GetTick();
    while (Is_First_Captured || Distance == 0)
    {
        if ((HAL_GetTick() - timeout) > 30) // 30 ms timeout
        {
            Distance = 0; // no object detected
            break;
        }
    }

    return Distance;  // in cm
}
void debug_printf(const char *fmt, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	uint16_t i = 0;
	while(buffer[i] != '\0')
	{
		ITM_SendChar(buffer[i]);
		i++;
	}
}
void EraseFlash(void )
{
	FLASH_EraseInitTypeDef FlashEraseDefination;
	uint32_t FlashEraseFault = 0;
	HAL_FLASH_Unlock();

	FlashEraseDefination.TypeErase = FLASH_TYPEERASE_PAGES;
	FlashEraseDefination.Banks     = FLASH_BANK_2;
	FlashEraseDefination.Page      = 256;
	FlashEraseDefination.NbPages   = 3;

	HAL_FLASHEx_Erase(&FlashEraseDefination, &FlashEraseFault);

	HAL_FLASH_Lock();
}
uint32_t flash_write_addr = FLASH_USER_START_ADDR;  // start address

void Flash_WriteString(const char *str)
{
    HAL_FLASH_Unlock();

    uint32_t slot_addr = FLASH_USER_START_ADDR + (log_index * LOG_SLOT_SIZE);
    uint64_t data64;
    uint32_t i = 0;
    int b = 0;

    // erase the slot before writing
    FLASH_EraseInitTypeDef eraseInit;
    uint32_t pageError;
    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.Banks = FLASH_BANK_2;
    eraseInit.Page  = 256 + log_index;   // adjust page number
    eraseInit.NbPages = 1;
    HAL_FLASHEx_Erase(&eraseInit, &pageError);

    uint32_t write_addr = slot_addr;

    while (str[i] != '\0') {
        data64 = 0xFFFFFFFFFFFFFFFF;

        for (b = 0; b < 8 && str[i] != '\0'; b++, i++) {
            ((uint8_t*)&data64)[b] = str[i];
        }
        if(str[i] == '\0' && b <= 7) {
            ((uint8_t*)&data64)[b] = '\0';
        }

        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, write_addr, data64);
        write_addr += 8;
    }

    // store terminator if needed
    if (b > 8) {
        data64 = 0xFFFFFFFFFFFFFFFF;
        ((uint8_t*)&data64)[0] = '\0';
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, write_addr, data64);
    }

    HAL_FLASH_Lock();

    // move to next slot
    log_index = (log_index + 1) % MAX_LOGS;
    if (log_index == 0) log_full = 1;
}


void Flash_ReadString(uint32_t StartAddr, char *outBuf, uint32_t maxLen)
{
    uint32_t i = 0;
    while (i < maxLen - 1) {
        uint64_t data64 = *(uint64_t*)StartAddr;
        for (int b = 0; b < 8 && i < maxLen - 1; b++) {
            char c = ((uint8_t*)&data64)[b];
            if (c == '\0' || c == (char)0xFF) {
                outBuf[i] = '\0';
                return;
            }
            outBuf[i++] = c;
        }
        StartAddr += 8;
    }
    outBuf[i] = '\0';
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  //printf("ultasonic sensor sending signals:");
  uint8_t msg[]="initial data in flash:\r\n";
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen((char*)msg), 100);

  DumpAllFlash();

  //EraseFlash();

  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);

  //HAL_UART_Transmit(&huart2,str,strlen((char*)str), 100);
  int cnt=0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
      Distance = HCSR04_Read();
      uint16_t d;
      if (Distance > 0 && Distance < 150)  // object detected within 150 cm
      {
          if (object_present == 0)  // just entered range
          {
              //HAL_UART_Transmit(&huart2, first, strlen((char*)first), 100);

              char msg[50];
              sprintf(msg, "%s Distance=%u cm\r\n",(char*)first, Distance);
              HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
              //Flash_WriteString((char*)first);
              Flash_WriteString(msg);

              object_present = 1;  // mark as present
          }
          d=Distance;
          // else: do nothing, object still in range
      }
      else  // no object or out of range
      {
          if (object_present == 1)  // just removed
          {
        	  cnt++;
        	  char msg[50];
			   sprintf(msg, "%s Distance=%u cm\r\n",(char*)second, d);
			   HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
              Flash_WriteString(msg);
              object_present = 0;  // mark as removed
          }
          // else: do nothing, still no object
      }

      HAL_Delay(200); // small delay to reduce trigger spam
      if(cnt==MAX_LOGS)
      {
    	  DumpAllFlash();
    	  cnt=0;
      }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 80-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void DumpAllFlash(void)
{
    char buffer[64];
    uint32_t start_index;

    HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n--- FLASH LOG ---\r\n", 21, 100);

    // If wrapped, oldest = log_index, else oldest = 0
    if (log_full)
        start_index = log_index;
    else
        start_index = 0;

    uint32_t count = log_full ? MAX_LOGS : log_index;

    for (uint32_t i = 0; i < count; i++) {
        uint32_t slot = (start_index + i) % MAX_LOGS;
        uint32_t addr = FLASH_USER_START_ADDR + (slot * LOG_SLOT_SIZE);

        Flash_ReadString(addr, buffer, sizeof(buffer));

        if (buffer[0] != '\0') {
            HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
            HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);
        }
    }

    HAL_UART_Transmit(&huart2, (uint8_t*)"--- END LOG ---\r\n", 18, 100);
}


//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//    if (GPIO_Pin == B1_Pin)  // PC13 user button pressed
//    {
//
//    	DumpAllFlash();
//    }
//
//}
//void EXTI15_10_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
//}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
