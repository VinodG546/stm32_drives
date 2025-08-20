#include "stm32l4xx_hal.h"
#include <stdint.h>


#define  BUFFER_SIZE 32
uint32_t srcBuffer[BUFFER_SIZE] = {
    0x11111111,0x22222222,0x33333333,0x44444444,
    0x55555555,0x66666666,0x77777777,0x88888888,
    0x99999999,0xAAAAAAAA,0xBBBBBBBB,0xCCCCCCCC,
    0xDDDDDDDD,0xEEEEEEEE,0xFFFFFFFF,0x00000000,
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
};
uint32_t dstBuffer[BUFFER_SIZE];
DMA_HandleTypeDef hdma_memtomem;
static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* LD2 on NUCLEO-L476RG is PA5 */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void MX_DMA_Init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();

    hdma_memtomem.Instance                 = DMA1_Channel1;
    hdma_memtomem.Init.Request             = DMA_REQUEST_MEM2MEM;
    hdma_memtomem.Init.Direction           = DMA_MEMORY_TO_MEMORY;
    hdma_memtomem.Init.PeriphInc           = DMA_PINC_ENABLE;   // source increments
    hdma_memtomem.Init.MemInc              = DMA_MINC_ENABLE;   // destination increments
    hdma_memtomem.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;  // word (32-bit)
    hdma_memtomem.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    hdma_memtomem.Init.Mode                = DMA_NORMAL;
    hdma_memtomem.Init.Priority            = DMA_PRIORITY_HIGH;

    HAL_DMA_Init(&hdma_memtomem);

}
static void SystemClock_Config(void)
{
    /* Minimal HSI-based config @80MHz typical for L476.
       Replace with your CubeMX-generated function if you have one. */
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 10;   // 16MHz * 10 / 2 = 80MHz
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
}
int main()
{
	HAL_Init();
	SystemClock_Config();
	MX_DMA_Init();
	MX_GPIO_Init();
	HAL_DMA_Start(&hdma_memtomem,
	                      (uint32_t)srcBuffer,
	                      (uint32_t)dstBuffer,
	                      BUFFER_SIZE);
	HAL_DMA_PollForTransfer(&hdma_memtomem,
	                                HAL_DMA_FULL_TRANSFER,
	                                HAL_MAX_DELAY);
	for (uint32_t i = 0; i < BUFFER_SIZE; i++) {
	        if (dstBuffer[i] != srcBuffer[i]) {
	            /* Indicate error: fast blink */
	            while (1) {
	                HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	                HAL_Delay(100);
	            }
	        }
	    }

	    /* Success: slow blink LD2 (PA5) forever */
	    while (1) {
	        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	        HAL_Delay(500);
	    }
}


void SysTick_Handler(void)
{
	HAL_IncTick();
}

