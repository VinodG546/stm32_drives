#ifndef __STM32L476xx_H
#define __STM32L476xx_H

#include <stdint.h>

/* ============= Base Addresses ============= */
#define PERIPH_BASE           0x40000000UL
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x08000000UL)
#define APB1PERIPH_BASE       PERIPH_BASE

#define RCC_BASE              (AHB2PERIPH_BASE + 0x1000UL)
#define GPIOA_BASE            (AHB2PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE            (AHB2PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE            (AHB2PERIPH_BASE + 0x0800UL)

#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400UL)

/* ============= Peripheral Structures ============= */
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
    volatile uint32_t BRR;
} GPIO_TypeDef;

typedef struct {
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
} I2C_TypeDef;

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t ICSCR;
    volatile uint32_t CFGR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t PLLSAI1CFGR;
    volatile uint32_t PLLSAI2CFGR;
    volatile uint32_t CIER;
    volatile uint32_t CIFR;
    volatile uint32_t CICR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t RESERVED0;
    volatile uint32_t APB1RSTR1;
    volatile uint32_t APB1RSTR2;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1;
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    uint32_t RESERVED2;
    volatile uint32_t APB1ENR1;
    volatile uint32_t APB1ENR2;
    volatile uint32_t APB2ENR;
} RCC_TypeDef;

/* ============= Peripheral Instances ============= */
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define RCC                 ((RCC_TypeDef  *) RCC_BASE)
#define I2C1                ((I2C_TypeDef  *) I2C1_BASE)

/* ============= Bit Definitions ============= */
/* RCC */
#define RCC_AHB2ENR_GPIOAEN   (1U << 0)
#define RCC_AHB2ENR_GPIOBEN   (1U << 1)
#define RCC_AHB2ENR_GPIOCEN   (1U << 2)

#define RCC_APB1ENR1_I2C1EN   (1U << 21)
#define RCC_APB1RSTR1_I2C1RST (1U << 21)

/* I2C CR1 */
#define I2C_CR1_PE            (1U << 0)

/* I2C CR2 */
#define I2C_CR2_RD_WRN        (1U << 10)
#define I2C_CR2_START         (1U << 13)
#define I2C_CR2_STOP          (1U << 14)
#define I2C_CR2_AUTOEND       (1U << 25)

/* I2C ISR */
#define I2C_ISR_TXIS          (1U << 1)
#define I2C_ISR_RXNE          (1U << 2)
#define I2C_ISR_TC            (1U << 6)
#define I2C_ISR_STOPF         (1U << 5)
#define I2C_ISR_BUSY          (1U << 15)

/* I2C ICR */
#define I2C_ICR_STOPCF        (1U << 5)

#endif /* __STM32L476xx_H */
