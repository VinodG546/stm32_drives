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
#include<stdarg.h>
#include<stdio.h>
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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
#define FLASH_USER_START_ADDR   0x08080000
uint32_t count;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void DumpAllFlash(void)
{
    uint32_t addr = FLASH_USER_START_ADDR;
    char buffer[64];

    HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n--- FLASH LOG ---\r\n", 21, 100);

    while (1)
    {
        Flash_ReadString(addr, buffer, sizeof(buffer));

        if (buffer[0] == '\0') break; // no more strings

        HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
        HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);

        addr += 256; // move to next 8-byte aligned slot
    }

    HAL_UART_Transmit(&huart2, (uint8_t*)"--- END LOG ---\r\n", 18, 100);
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
	FlashEraseDefination.Page      = 0;
	FlashEraseDefination.NbPages   = 1;

	HAL_FLASHEx_Erase(&FlashEraseDefination, &FlashEraseFault);

	HAL_FLASH_Lock();
}

void WriteData(uint64_t Data)
{
    HAL_StatusTypeDef status;
    HAL_FLASH_Unlock();
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,FLASH_USER_START_ADDR,Data);
    if(status != HAL_OK)
    {

    }
    HAL_FLASH_Lock();
}

uint64_t  RetriveData(uint32_t address)
{
	return *(uint64_t*)address;
}
void Flash_WriteString(uint32_t StartAddr, const char *str)
{
    HAL_FLASH_Unlock();

    uint64_t data64;
    uint32_t i = 0;
    while (str[i] != '\0') {
        // Pack 8 characters into one 64-bit word
        data64 = 0xFFFFFFFFFFFFFFFF; // fill with 0xFF (erased default)
        for (int b = 0; b < 8 && str[i] != '\0'; b++, i++) {
            ((uint8_t*)&data64)[b] = str[i];
        }

        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, StartAddr, data64);
        StartAddr += 8;  // move to next 64-bit slot
    }

    HAL_FLASH_Lock();
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
	uint64_t write_val = 0x1122334455667788ULL;  // test value
	uint64_t read_val  = 0;
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
  /* USER CODE BEGIN 2 */

//  EraseFlash();
//
//  WriteData(write_val);
//
//  read_val = RetriveData(FLASH_USER_START_ADDR);
//
//  uint32_t low  = (uint32_t)(read_val & 0xFFFFFFFF);
//  uint32_t high = (uint32_t)(read_val >> 32);
//  count+=8;
//
//  debug_printf("data read from flash 0x%08lx%08lx\r\n", high, low);
//
//  char buffer[50];
//
//  //EraseFlash();
//
//  Flash_WriteString(FLASH_USER_START_ADDR+count, "Hello STM32!");
//
//  Flash_ReadString(FLASH_USER_START_ADDR+count, buffer, sizeof(buffer));
//
//  debug_printf("String read from flash: %s\r\n", buffer);
//
  DumpAllFlash();
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
