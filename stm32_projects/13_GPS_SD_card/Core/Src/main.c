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
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"stdio.h"
#include"string.h"
#include"stdarg.h"
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
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */
void process_SD_card( void );
void read_SD_data(void);
void write_to_SD1(char *msg);
void un_mount(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

FATFS       FatFs;                //Fatfs handle
FIL         fil;                  //File handle
FRESULT     fres;                 //Result after operations
char        buf[100];

uint8_t rxBuffer[128] = {0};
uint8_t rxIndex = 0;
uint8_t rxData;
float nmeaLong;
float nmeaLat;
float utcTime;
char northsouth;
char eastwest;
char posStatus;
float decimalLong;
float decimalLat;

uint32_t lastSendTime = 0;
const uint32_t SEND_INTERVAL = 5000; // 5 seconds in milliseconds
uint8_t coordinateSent = 0;

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

void send_coordinates_periodically(void)
{
    static uint32_t lastTime = 0;
    uint32_t now = HAL_GetTick();

    if (now - lastTime >= 5000) {  // 5 seconds
        char logStr[128];
        sprintf(logStr, "%.6f,%c,%.6f,%c,%.2f\n",
                decimalLat, northsouth,
                decimalLong, eastwest,
                utcTime);

        write_to_SD1(logStr);
        debug_printf("Logged: %s", logStr);

        lastTime = now;
    }
}

float nmeaToDecimal(float coordinate) {
    int degree = (int)(coordinate/100);
    float minutes = coordinate - degree * 100;
    float decimalDegree = minutes / 60;
    float decimal = degree + decimalDegree;
    return decimal;
}

void gpsParse(char *strParse){
  if(!strncmp(strParse, "$GPGGA", 6)){
    sscanf(strParse, "$GPGGA,%f,%f,%c,%f,%c",
      &utcTime, &nmeaLat, &northsouth, &nmeaLong, &eastwest);
    decimalLat = nmeaToDecimal(nmeaLat);
    decimalLong = nmeaToDecimal(nmeaLong);
  }
  else if (!strncmp(strParse, "$GPGLL", 6)){
    sscanf(strParse, "$GPGLL,%f,%c,%f,%c,%f",
      &nmeaLat, &northsouth, &nmeaLong, &eastwest, &utcTime);
    decimalLat = nmeaToDecimal(nmeaLat);
    decimalLong = nmeaToDecimal(nmeaLong);
  }
  else if (!strncmp(strParse, "$GPRMC", 6)){
    sscanf(strParse, "$GPRMC,%f,%c,%f,%c,%f,%c",
      &utcTime, &posStatus, &nmeaLat, &northsouth, &nmeaLong, &eastwest);
    decimalLat = nmeaToDecimal(nmeaLat);
    decimalLong = nmeaToDecimal(nmeaLong);
  }
}

int gpsValidate(char *nmea){
    char check[3];
    char calculatedString[3];
    int index;
    int calculatedCheck;

    index=0;
    calculatedCheck=0;

    // Ensure that the string starts with a "$"
    if(nmea[index] == '$')
        index++;
    else
        return 0;

    //No NULL reached, 75 char largest possible NMEA message, no '*' reached
    while((nmea[index] != 0) && (nmea[index] != '*') && (index < 75)){
        calculatedCheck ^= nmea[index];// calculate the checksum
        index++;
    }

    if(index >= 75){
        return 0;// the string is too long so return an error
    }

    if (nmea[index] == '*'){
        check[0] = nmea[index+1];    //put hex chars in check string
        check[1] = nmea[index+2];
        check[2] = 0;
    }
    else
        return 0;// no checksum separator found therefore invalid data

    sprintf(calculatedString,"%02X",calculatedCheck);
    return((calculatedString[0] == check[0])
        && (calculatedString[1] == check[1])) ? 1 : 0 ;
}

void printLatLon(const char *nmeaSentence) {
    // Check if the sentence starts with $GNGGA
    if(strncmp(nmeaSentence, "$GNGGA", 6) == 0) {
        char line[200];
        strcpy(line, nmeaSentence);  // copy to mutable string

        char *token = strtok(line, ",");       // $GNGGA
        token = strtok(NULL, ",");       // UTC Time
        char *lat = strtok(NULL, ",");    // Latitude
        char *lat_dir = strtok(NULL, ","); // N/S
        char *lon = strtok(NULL, ",");    // Longitude
        char *lon_dir = strtok(NULL, ","); // E/W

        if(lat && lat_dir && lon && lon_dir) {
            debug_printf("Latitude: %s %s, Longitude: %s %s\n", lat, lat_dir, lon, lon_dir);
            //char str[100];
           // sprintf(str, "Coordinates: Lat: %.6f %c, Lon: %.6f %c, Time: %.2f\n",
             //                  decimalLat, northsouth, decimalLong, eastwest, utcTime);
            //write_to_SD1(str);
        }
    }
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART3)
  {
    if(rxData != '\n' && rxIndex < sizeof(rxBuffer) - 1)
    {
      rxBuffer[rxIndex++] = rxData;
    }
    else
    {
      rxBuffer[rxIndex] = '\0';

      if(gpsValidate((char*)rxBuffer))
      {
        gpsParse((char*)rxBuffer);

        // Print only after full sentence is parsed
        //debug_printf("Buffer : %s\n",rxBuffer);
        printLatLon((char*)rxBuffer);
        //debug_printf("Latitude : %.6f %c, Longitude : %.6f %c\r\n", decimalLat, northsouth, decimalLong, eastwest);
      }
      else
      {
        debug_printf("Invalid NMEA: %s\r\n", rxBuffer);
      }

      rxIndex = 0;
      memset(rxBuffer, 0, sizeof(rxBuffer));
    }

    // Re-enable interrupt
    HAL_UART_Receive_IT(&huart3, &rxData, 1);
  }
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
  MX_SPI2_Init();
  MX_FATFS_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  HAL_UART_Receive_IT(&huart3,&rxData,1);



  debug_printf("SD Card Initialization\n");
  process_SD_card();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  send_coordinates_periodically();

	  // Add a small delay to prevent excessive CPU usage
	  HAL_Delay(100);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void process_SD_card( void )
{

    //Mount the SD Card
    fres = f_mount(&FatFs, "", 1);    //1=mount now
    if (fres != FR_OK)
    {
      debug_printf("No SD Card found : (%i)\r\n", fres);
    }
    debug_printf("SD Card Mounted Successfully!!!\r\n");

    //Read the SD Card Total size and Free Size
    FATFS *pfs;
    DWORD fre_clust;
    uint32_t totalSpace, freeSpace;

    f_getfree("", &fre_clust, &pfs);
    totalSpace = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
    freeSpace = (uint32_t)(fre_clust * pfs->csize * 0.5);

    debug_printf("TotalSpace : %lu bytes, FreeSpace = %lu bytes\n", totalSpace, freeSpace);

    //Open the file
    fres = f_open(&fil, "Logs.txt", FA_WRITE | FA_READ | FA_CREATE_ALWAYS);
    if(fres != FR_OK)
    {
    	debug_printf("File creation/open Error : (%i)\r\n", fres);
    }

    debug_printf("Writing data!!!\r\n");

    f_puts("********GPS module Data**************\n", &fil);

    f_close(&fil);

    fres = f_open(&fil, "Logs.txt", FA_READ);
    if(fres != FR_OK)
    {
    	debug_printf("File opening Error : (%i)\r\n", fres);
    }

    f_gets(buf, sizeof(buf), &fil);
    //int len = strlen(buf);

    debug_printf("Read Data : %s\n", buf);

    f_close(&fil);
    debug_printf("Closing File!!!\r\n");

}
void read_SD_data(void)
{
      fres = f_open(&fil, "Logs.txt", FA_READ);
      //int len;

      while(f_gets(buf, sizeof(buf), &fil))
      {
    	  //f_lseek(&fil, strlen(buf)+1);
    	  debug_printf("Sensor_data : %s\n", buf);
    	  //memset(buf,'\0',sizeof(buf));
      }
      f_close(&fil);
}

void write_to_SD1(char *msg)
{
    fres = f_open(&fil, "Logs.txt", FA_OPEN_APPEND | FA_WRITE);
    if (fres == FR_OK) {
        debug_printf("writing log into SD card %s", msg);
        f_puts(msg, &fil);
        f_sync(&fil);
        f_close(&fil);
    } else {
        debug_printf("File open error: %d\n", fres);
    }
}

void un_mount(void)
{
	  f_mount(NULL, "", 0);
	  debug_printf("SD Card Unmounted Successfully!!!\r\n");
}

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

#ifdef  USE_FULL_ASSERT
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
