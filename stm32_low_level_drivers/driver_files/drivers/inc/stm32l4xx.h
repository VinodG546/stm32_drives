#ifndef STM32L4XX_H
#define STM32L4XX_H
#include <stdint.h>

#define     FLASH_BASEADDR                0x08000000U   //flash memory starting address
#define     SRAM1_BASEADDR                0x20000000U   //sram1 starting address  96kb
#define     SRAM2_BASEADDR				  0x10000000U   //sram2 starting address 32kb
#define     ROM_BASEADDR                  0x1FFF0000U 	//system memory or ROM starting address

#define     PERIPH_BASEADDR               0x40000000U
#define     APB1_BASEADDR				  0x40000000U
#define     APB2_BASEADDR				  0x40010000U
#define     AHB1_BASEADDR                 0x40020000U
#define     AHB2_BASEADDR                 0x48000000U



//*******************************************************
//NVIC ISERx register addrs and ICERx register addrs
#define NVIC_ISER0  ((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1  ((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2  ((volatile uint32_t*)0xE000E108)
#define NVIC_ISER3  ((volatile uint32_t*)0xE000E10c)

#define NVIC_ICER0  ((volatile uint32_t*)0xE000E180)
#define NVIC_ICER1  ((volatile uint32_t*)0xE000E184)
#define NVIC_ICER2  ((volatile uint32_t*)0xE000E188)
#define NVIC_ICER3  ((volatile uint32_t*)0xE000E18c)

//prp registers
#define NVIC_PR_BASE_ADDR  ((volatile uint32_t*)0xE000E400)

#define NO_PR_BITS_IMPLEMENTED      4

//base addresses of peripheral of AHB2  bus
#define     GPIOA_BASEADDR				  0x48000000U
#define     GPIOB_BASEADDR				  0x48000400U
#define     GPIOC_BASEADDR                0x48000800U
#define     GPIOD_BASEADDR				  0x48000c00U
#define     GPIOE_BASEADDR				  0x48001000U
#define     GPIOF_BASEADDR				  0x48001400U
#define     GPIOG_BASEADDR				  0x48001800U
#define     GPIOH_BASEADDR				  0x48001C00U

#define     ADC_BASEADDR                  0x50040000U



//base addresses of AHB1 bus peripherals

#define    DMA1_BASEADDR				  0x40020000U
#define    DMA2_BASEADDR                  0x40020400U

#define    RCC_BASEADDR                   0x40021000U


//base addresses of APB2 peripherals

#define    USART1_BASEADDR                0x40013800U

#define    SPI1_BASEADDR                  0x40013000U

#define    EXTI_BASEADDR                  0x40010400U

#define    SYSCFG_BASEADDR                0x40010000U


//base addresses of peripherals of APB1 bus

#define    RTC_BASEADDR                    0x40002800U

#define    SPI2_BASEADDR                   0x40003800U
#define    SPI3_BASEADDR                   0x40003C00U

#define    USART2_BASEADDR                 0x40004400U
#define    USART3_BASEADDR                 0x40004800U

#define    UART4_BASEADDR                  0x40004C00U
#define    UART5_BASEADDR				   0x40005000U

#define    I2C1_BASEADDR				   0x40005400U
#define    I2C2_BASEADDR				   0x40005800U
#define    I2C3_BASEADDR				   0x40005C00U

#define    DAC1_BASEADDR				   0x40007400U


//********************************************************************************************

//registers for different peripherals  for stm32l4xx  only


//GPIO registers structure

typedef struct
{
	volatile uint32_t  MODER;    //GPIO port mode register (GPIOx_MODER) offset 0x0
	volatile uint32_t  OTYPER;   //GPIO port output type register (GPIOx_OTYPER)  offset 0x04
	volatile uint32_t  OSPEEDR;  //GPIO port output speed register (GPIOx_OSPEEDR) offset 0x08
	volatile uint32_t  PUPDR;    //GPIO port pull-up/pull-down register (GPIOx_PUPDR)
	volatile uint32_t  IDR;     // GPIO port input data register (GPIOx_IDR)
	volatile uint32_t  ODR;      //GPIO port output data register (GPIOx_ODR)
	volatile uint32_t  BSSR;    //GPIO port bit set/reset register (GPIOx_BSRR)
	volatile uint32_t  LCKR;      //GPIO port configuration lock register (GPIOx_LCKR)
	volatile uint32_t  AFR[2];   //GPIO alternate function low  and high register (GPIOx_AFRL  and GPIO_ARRH)
	volatile uint32_t  BRR;      // GPIO port bit reset register (GPIOx_BRR)
	volatile uint32_t  ASCR;     // GPIO port analog switch control register (GPIOx_ASCR)
}GPIO_RegDef_t;      //GPIO registers structure


#define   GPIOA       ((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define   GPIOB       ((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define   GPIOC       ((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define   GPIOD       ((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define   GPIOE       ((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define   GPIOF       ((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define   GPIOG       ((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define   GPIOH       ((GPIO_RegDef_t*)GPIOH_BASEADDR)


//struct for RCC clock
typedef struct
{
    volatile uint32_t CR;          // 0x00: Clock control register
    volatile uint32_t ICSCR;       // 0x04: Internal clock sources calibration register
    volatile uint32_t CFGR;        // 0x08: Clock configuration register
    volatile uint32_t PLLCFGR;     // 0x0C: PLL configuration register
    volatile uint32_t PLLSAI1CFGR; // 0x10: PLLSAI1 configuration register
    volatile uint32_t PLLSAI2CFGR; // 0x14: PLLSAI2 configuration register
    volatile uint32_t CIER;        // 0x18: Clock interrupt enable register
    volatile uint32_t CIFR;        // 0x1C: Clock interrupt flag register
    volatile uint32_t CICR;        // 0x20: Clock interrupt clear register
    uint32_t RESERVED0;            // 0x24: Reserved
    volatile uint32_t AHB1RSTR;    // 0x28: AHB1 peripheral reset register
    volatile uint32_t AHB2RSTR;    // 0x2C: AHB2 peripheral reset register
    volatile uint32_t AHB3RSTR;    // 0x30: AHB3 peripheral reset register
    uint32_t RESERVED1;            // 0x34: Reserved
    volatile uint32_t APB1RSTR1;   // 0x38: APB1 peripheral reset register 1
    volatile uint32_t APB1RSTR2;   // 0x3C: APB1 peripheral reset register 2
    volatile uint32_t APB2RSTR;    // 0x40: APB2 peripheral reset register
    uint32_t RESERVED2;            // 0x44: Reserved
    volatile uint32_t AHB1ENR;     // 0x48: AHB1 peripheral clock enable register
    volatile uint32_t AHB2ENR;     // 0x4C: AHB2 peripheral clock enable register
    volatile uint32_t AHB3ENR;     // 0x50: AHB3 peripheral clock enable register
    uint32_t RESERVED3;            // 0x54: Reserved
    volatile uint32_t APB1ENR1;    // 0x58: APB1 peripheral clock enable register 1
    volatile uint32_t APB1ENR2;    // 0x5C: APB1 peripheral clock enable register 2
    volatile uint32_t APB2ENR;     // 0x60: APB2 peripheral clock enable register
    uint32_t RESERVED4;            // 0x64: Reserved
    volatile uint32_t AHB1SMENR;   // 0x68: AHB1 peripheral clocks enable in sleep/stop
    volatile uint32_t AHB2SMENR;   // 0x6C: AHB2 peripheral clocks enable in sleep/stop
    volatile uint32_t AHB3SMENR;   // 0x70: AHB3 peripheral clocks enable in sleep/stop
    uint32_t RESERVED5;            // 0x74: Reserved
    volatile uint32_t APB1SMENR1;  // 0x78: APB1 peripheral clocks enable in sleep/stop 1
    volatile uint32_t APB1SMENR2;  // 0x7C: APB1 peripheral clocks enable in sleep/stop 2
    volatile uint32_t APB2SMENR;   // 0x80: APB2 peripheral clocks enable in sleep/stop
    uint32_t RESERVED6;            // 0x84: Reserved
    volatile uint32_t CCIPR;       // 0x88: Peripherals independent clock config register
    uint32_t RESERVED7;            // 0x8C: Reserved
    volatile uint32_t BDCR;        // 0x90: Backup domain control register
    volatile uint32_t CSR;         // 0x94: Control/status register
    volatile uint32_t CRRCR;       // 0x98: Clock recovery RC register
    volatile uint32_t CCIPR2;      // 0x9C: Peripherals independent clock config register 2
} RCC_RegDef_t;

#define    RCC   ((RCC_RegDef_t*)RCC_BASEADDR)

//EXTI registers structure
typedef struct
{
    volatile uint32_t IMR1;    /*!< Interrupt mask register 1,      Address offset: 0x00 */
    volatile uint32_t EMR1;    /*!< Event mask register 1,          Address offset: 0x04 */
    volatile uint32_t RTSR1;   /*!< Rising trigger selection reg 1, Address offset: 0x08 */
    volatile uint32_t FTSR1;   /*!< Falling trigger selection reg 1,Address offset: 0x0C */
    volatile uint32_t SWIER1;  /*!< Software interrupt event reg 1, Address offset: 0x10 */
    volatile uint32_t PR1;     /*!< Pending register 1,             Address offset: 0x14 */
    uint32_t RESERVED1[2];     /*!< Reserved                        Address offset: 0x18–0x1C */
    volatile uint32_t IMR2;    /*!< Interrupt mask register 2,      Address offset: 0x20 */
    volatile uint32_t EMR2;    /*!< Event mask register 2,          Address offset: 0x24 */
    volatile uint32_t RTSR2;   /*!< Rising trigger selection reg 2, Address offset: 0x28 */
    volatile uint32_t FTSR2;   /*!< Falling trigger selection reg 2,Address offset: 0x2C */
    volatile uint32_t SWIER2;  /*!< Software interrupt event reg 2, Address offset: 0x30 */
    volatile uint32_t PR2;     /*!< Pending register 2,             Address offset: 0x34 */
} EXTI_RegDef_t;

#define    EXTI  ((EXTI_RegDef_t*)EXTI_BASEADDR)

//EXTI lines IRQ numbers
#define IRQ_NO_EXTI0       6
#define IRQ_NO_EXTI1       7
#define IRQ_NO_EXTI2       8
#define IRQ_NO_EXTI3       9
#define IRQ_NO_EXTI4       10
#define IRQ_NO_EXTI9_5     23
#define IRQ_NO_EXTI15_10   40



typedef struct
{
    volatile uint32_t MEMRMP;       /*!< SYSCFG memory remap register,              Address offset: 0x00 */
    volatile uint32_t CFGR1;        /*!< SYSCFG configuration register 1,           Address offset: 0x04 */
    volatile uint32_t EXTICR[4];    /*!< SYSCFG external interrupt config registers,0x08–0x14 */
    volatile uint32_t SCSR;         /*!< SYSCFG SRAM2 control and status register,  Address offset: 0x18 */
    volatile uint32_t CFGR2;        /*!< SYSCFG configuration register 2,           Address offset: 0x1C */
    volatile uint32_t SWPR;         /*!< SYSCFG SRAM2 write protection register,    Address offset: 0x20 */
    volatile uint32_t SKR;          /*!< SYSCFG SRAM2 key register,                 Address offset: 0x24 */
} SYSCFG_RegDef_t;

#define   SYSCFG    ((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

typedef struct
{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t CRCPR;
	volatile uint32_t RXCRCPR;
	volatile uint32_t RXCRCR;
	volatile uint32_t TXCRCR;
	volatile uint32_t I2SCFGR;
	volatile uint32_t I2SPR;
}SPI_RegDef_t;


#define SPI1  ((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2  ((SPI_RegDef_t*)SPI2_BASEADDR)
#define SPI3  ((SPI_RegDef_t*)SPI3_BASEADDR)

typedef struct
{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t OAR1;
	volatile uint32_t OAR2;
	volatile uint32_t TIMINGR;
	volatile uint32_t TIMEOUTR;
	volatile uint32_t ISR;
	volatile uint32_t ICR;
	volatile uint32_t PECR;
	volatile uint32_t RXDR;
	volatile uint32_t TXDR;
}I2C_RegDef_t;

#define I2C1    ((I2C_RegDef_t*)I2C1_BASEADDR)
#define I2C2	((I2C_RegDef_t*)I2C2_BASEADDR)
#define I2C3	((I2C_RegDef_t*)I2C3_BASEADDR)

//macros for enabling GPIOx RCC clock

#define GPIOA_PCLK_EN()    (RCC->AHB2ENR |= (1 << 0))
#define GPIOB_PCLK_EN()    (RCC->AHB2ENR |= (1 << 1))
#define GPIOC_PCLK_EN()    (RCC->AHB2ENR |= (1 << 2))
#define GPIOD_PCLK_EN()    (RCC->AHB2ENR |= (1 << 3))
#define GPIOE_PCLK_EN()    (RCC->AHB2ENR |= (1 << 4))
#define GPIOF_PCLK_EN()    (RCC->AHB2ENR |= (1 << 5))
#define GPIOG_PCLK_EN()    (RCC->AHB2ENR |= (1 << 6))
#define GPIOH_PCLK_EN()    (RCC->AHB2ENR |= (1 << 7))

//macros for enabling SPIx RCC clock

#define SPI1_PCLK_EN()     (RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()     (RCC->APB1ENR1 |= (1 << 14))
#define SPI3_PCLK_EN()     (RCC->APB1ENR1 |= (1 << 15))

//macros for enabling I2cx RCC clock

#define I2C1_PCLK_EN()     (RCC->APB1ENR1 |= (1 << 21))
#define I2C2_PCLK_EN()     (RCC->APB1ENR1 |= (1 << 22))
#define I2C3_PCLK_EN()     (RCC->APB1ENR1 |= (1 << 23))

//macros for enabling USARTx RCC clock

#define USART1_PCLK_EN()   (RCC->APB2ENR |= (1 << 14))
#define USART2_PCLK_EN()   (RCC->APB1ENR1 |= (1 << 17))
#define USART3_PCLK_EN()   (RCC->APB1ENR1 |= (1 << 18))
#define UART4_PCLK_EN()    (RCC->APB1ENR1 |= (1 << 19))
#define UART5_PCLK_EN()    (RCC->APB1ENR1 |= (1 << 20))

//macros for enabling DMAx RCC clock

#define DMA1_PCLK_EN()     (RCC->AHB1ENR |= (1 << 0))
#define DMA2_PCLK_EN()     (RCC->AHB1ENR |= (1 << 1))

//ADC clock

#define ADC_PCLK_EN()      (RCC->AHB2ENR |= (1 << 13))

//DAC clock

#define DAC1_PCLK_EN()     (RCC->APB1ENR1 |= (1 << 29))

//system configuration clock

#define SYSCFG_PCLK_EN()   (RCC->APB2ENR |= (1 << 0))



//macros for disabling clock

//macros for disabling GPIOx RCC clock
#define GPIOA_PCLK_DI()    (RCC->AHB2ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()    (RCC->AHB2ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()    (RCC->AHB2ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()    (RCC->AHB2ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()    (RCC->AHB2ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI()    (RCC->AHB2ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI()    (RCC->AHB2ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI()    (RCC->AHB2ENR &= ~(1 << 7))

//macros for disabling SPIx RCC clock
#define SPI1_PCLK_DI()     (RCC->APB2ENR  &= ~(1 << 12))
#define SPI2_PCLK_DI()     (RCC->APB1ENR1 &= ~(1 << 14))
#define SPI3_PCLK_DI()     (RCC->APB1ENR1 &= ~(1 << 15))

//macros for disabling I2Cx RCC clock
#define I2C1_PCLK_DI()     (RCC->APB1ENR1 &= ~(1 << 21))
#define I2C2_PCLK_DI()     (RCC->APB1ENR1 &= ~(1 << 22))
#define I2C3_PCLK_DI()     (RCC->APB1ENR1 &= ~(1 << 23))

//macros for disabling USARTx RCC clock
#define USART1_PCLK_DI()   (RCC->APB2ENR  &= ~(1 << 14))
#define USART2_PCLK_DI()   (RCC->APB1ENR1 &= ~(1 << 17))
#define USART3_PCLK_DI()   (RCC->APB1ENR1 &= ~(1 << 18))
#define UART4_PCLK_DI()    (RCC->APB1ENR1 &= ~(1 << 19))
#define UART5_PCLK_DI()    (RCC->APB1ENR1 &= ~(1 << 20))

//macros for disabling DMAx RCC clock
#define DMA1_PCLK_DI()     (RCC->AHB1ENR  &= ~(1 << 0))
#define DMA2_PCLK_DI()     (RCC->AHB1ENR  &= ~(1 << 1))

//ADC clock disable
#define ADC_PCLK_DI()      (RCC->AHB2ENR  &= ~(1 << 13))

//DAC clock disable
#define DAC1_PCLK_DI()     (RCC->APB1ENR1 &= ~(1 << 29))

//system configuration clock disable
#define SYSCFG_PCLK_DI()   (RCC->APB2ENR  &= ~(1 << 0))

//macros to reset GPIOx peripheral
#define GPIOA_REG_RESET()  do{(RCC->AHB2RSTR |= (1<<0)); (RCC->AHB2RSTR &= ~(1<<0)); } while(0)
#define GPIOB_REG_RESET()  do{(RCC->AHB2RSTR |= (1<<1)); (RCC->AHB2RSTR &= ~(1<<1)); } while(0)
#define GPIOC_REG_RESET()  do{(RCC->AHB2RSTR |= (1<<2)); (RCC->AHB2RSTR &= ~(1<<2)); } while(0)
#define GPIOD_REG_RESET()  do{(RCC->AHB2RSTR |= (1<<3)); (RCC->AHB2RSTR &= ~(1<<3)); } while(0)
#define GPIOE_REG_RESET()  do{(RCC->AHB2RSTR |= (1<<4)); (RCC->AHB2RSTR &= ~(1<<4)); } while(0)
#define GPIOF_REG_RESET()  do{(RCC->AHB2RSTR |= (1<<5)); (RCC->AHB2RSTR &= ~(1<<5)); } while(0)
#define GPIOG_REG_RESET()  do{(RCC->AHB2RSTR |= (1<<6)); (RCC->AHB2RSTR &= ~(1<<6)); } while(0)
#define GPIOH_REG_RESET()  do{(RCC->AHB2RSTR |= (1<<7)); (RCC->AHB2RSTR &= ~(1<<7)); } while(0)

/********************** SPI Peripheral Reset Macros **********************/

//macros to reset spix peripherals
// Reset SPI1
#define SPI1_REG_RESET()   do{ (RCC->APB2RSTR |= (1 << 12)); (RCC->APB2RSTR &= ~(1 << 12)); } while(0)

// Reset SPI2
#define SPI2_REG_RESET()   do{ (RCC->APB1RSTR1 |= (1 << 14)); (RCC->APB1RSTR1 &= ~(1 << 14)); } while(0)

// Reset SPI3
#define SPI3_REG_RESET()   do{ (RCC->APB1RSTR1 |= (1 << 15)); (RCC->APB1RSTR1 &= ~(1 << 15)); } while(0)

/********************** I2C Peripheral Reset Macros **********************/

#define I2C1_REG_RESET()     do{ (RCC->APB1RSTR1 |=  (1U << 21)); (RCC->APB1RSTR1 &= ~(1U << 21)); }while(0)
#define I2C2_REG_RESET()     do{ (RCC->APB1RSTR1 |=  (1U << 22)); (RCC->APB1RSTR1 &= ~(1U << 22)); }while(0)
#define I2C3_REG_RESET()     do{ (RCC->APB1RSTR1 |=  (1U << 23)); (RCC->APB1RSTR1 &= ~(1U << 23)); }while(0)


#define GPIO_BASEADDR_TO_CODE(x)   ((x==GPIOA) ? 0:\
								   (x==GPIOB) ? 1:\
								   (x==GPIOC) ? 2:\
								   (x==GPIOD) ? 3:\
								   (x==GPIOE) ? 4:\
								   (x==GPIOF) ? 5:\
								   (x==GPIOG) ? 6:\
								   (x==GPIOH) ? 7: 0)

/********************** SPI Register Flag Positions **********************/

//macros for SPI CR1 register bits
#define SPI_CR1_CPHA    0
#define SPI_CR1_CPOL    1
#define SPI_CR1_MSTR    2
#define SPI_CR1_BR      3
#define SPI_CR1_SPE     6
#define SPI_CR1_LSBFIRST 7
#define SPI_CR1_SSI     8
#define SPI_CR1_SSM     9
#define SPI_CR1_RXONLY  10
#define SPI_CR1_CRCL    11
#define SPI_CR1_CRCNEXT  12
#define SPI_CR1_CRCEN    13
#define SPI_CR1_BIBIOE   14
#define SPI_CR1_BIDIMODE  15

//macros for SPI CR2 register
#define SPI_CR2_RXDMAEN   0
#define SPI_CR2_TXDMAEN   1
#define SPI_CR2_SSOE      2
#define SPI_CR2_NSSP      3
#define SPI_CR2_FRF       4
#define SPI_CR2_ERRIE     5
#define SPI_CR2_RXNEIE    6
#define SPI_CR2_TXEIE     7


//macros for SPI SR register
#define SPI_SR_RXNE       0
#define SPI_SR_TXE        1
#define SPI_SR_BSY        7
//***************************generic macros********************************
//generic macros
#define ENABLE          1
#define DISABLE         0
#define SET				ENABLE
#define RESET           DISABLE
#define GPIO_PIN_SET    SET
#define GPIO_PIN_RESET  RESET
#define FLAG_RESET      RESET
#define FLAG_SET        SET


/********************** I2C Register Flag Positions **********************/

//I2C peripheral macros for bits
//CR1 register
#define I2C_CR1_PE             0
#define I2C_CR1_TXIE           1
#define I2C_CR1_RXIE           2
#define I2C_CR1_ADDRIE         3
#define I2C_CR1_NACKIE  		4
#define I2C_CR1_SOPIE   		5
#define I2C_CR1_TCIE    		6
#define I2C_CR1_ERRIE   		7
#define I2C_CR1_DNF     		8
#define I2C_CR1_ANFOFF  		12
#define I2C_CR1_TXDMAEN 		14
#define I2C_CR1_RXDMAEN 		15
#define I2C_CR1_SBC     		16
#define I2C_CR1_NOSTRETCH		17
#define I2C_CR1_WUPEN			18
#define I2C_CR1_GCEN			19
#define I2C_CR1_PECEN			23

//CR2 flags
#define I2C_CR2_SADD       		0
#define I2C_CR2_RD_WRN 			10
#define I2C_CR2_ADD10			12
#define I2C_CR2_HEAD10R			12
#define I2C_CR2_START			13
#define I2C_CR2_STOP			14
#define I2C_CR2_NACK			15
#define I2C_CR2_NBYTES			16
#define I2C_CR2_RELOAD			24
#define I2C_CR2_AUTOEND         25
#define I2C_CR2_PECBYTE			26


//ISR flags

#define I2C_ISR_TXE        0   // Transmit data register empty
#define I2C_ISR_TXIS       1   // Transmit interrupt status
#define I2C_ISR_RXNE       2   // Receive data register not empty
#define I2C_ISR_ADDR       3   // Address matched (slave mode)
#define I2C_ISR_NACKF      4   // NACK received
#define I2C_ISR_STOPF      5   // STOP detection flag
#define I2C_ISR_TC         6   // Transfer complete
#define I2C_ISR_TCR        7   // Transfer complete reload

#define I2C_ISR_BERR       8   // Bus error
#define I2C_ISR_ARLO       9   // Arbitration lost
#define I2C_ISR_OVR        10  // Overrun/Underrun
#define I2C_ISR_PECERR     11  // PEC error
#define I2C_ISR_TIMEOUT    12  // Timeout error
#define I2C_ISR_ALERT      13  // SMBus alert

#define I2C_ISR_BUSY       15  // Bus busy
#define I2C_ISR_DIR        16  // Transfer direction
#define I2C_ISR_ADDCODE    17  // Address match code (bits 23:17)

//ICR flags

#define I2C_ICR_ADDCF      3   // Clear ADDR flag
#define I2C_ICR_NACKCF     4   // Clear NACK flag
#define I2C_ICR_STOPCF     5   // Clear STOP flag
#define I2C_ICR_BERRCF     8   // Clear Bus error flag
#define I2C_ICR_ARLOCF     9   // Clear Arbitration lost flag
#define I2C_ICR_OVRCF      10  // Clear Overrun/Underrun flag
#define I2C_ICR_PECCF      11  // Clear PEC error flag
#define I2C_ICR_TIMOUTCF   12  // Clear Timeout flag
#define I2C_ICR_ALERTCF    13  // Clear Alert flag

//OAR1 flags
#define I2c_OAR1_OA1EN		15
#define I2c_OAR1_OA1MODE	10
#define I2c_OAR1_OA1		0

//OAR2 flgas
#define I2c_OAR2_OA2		1
#define I2c_OAR2_OA2MSK		8
#define I2c_OAR2_OA2EN		15
//RXDR register
#define I2C_RXDR_RXDATA     0

//TXDR register
#define I2C_TXDR_TXDATA     0
#endif


