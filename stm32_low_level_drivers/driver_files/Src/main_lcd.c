/*
 * rtc_lcd.c
 *
 *  Created on: Sep 4, 2025
 *      Author: VINOD
 */
#include "stm32l4xx.h"
#include "stm32l4xx_gpio_driver.h"
#include"lcd.h"

int main(void)
{
    // Initialize LCD
    lcd_init();

    // Print "Hello"
    lcd_print_string("Hello");

    // Set cursor to 2nd row, 1st column
    lcd_set_cursor(2, 1);
    lcd_print_string("STM32L4");

    while(1)
    {
        // main loop (you can add scrolling or blinking here)
    }
}
