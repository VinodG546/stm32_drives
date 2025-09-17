#ifndef INC_SSD1306_H_
#define INC_SSD1306_H_

#include "stm32l4xx_hal.h"
#include "fonts.h"

#define SSD1306_I2C_ADDR    0x78
#define SSD1306_WIDTH       128
#define SSD1306_HEIGHT      64

/* SSD1306 color */
typedef enum {
    SSD1306_COLOR_BLACK = 0x00,
    SSD1306_COLOR_WHITE = 0x01
} SSD1306_COLOR_t;

/* Functions */
uint8_t SSD1306_Init(void);
void SSD1306_UpdateScreen(void);
void SSD1306_Fill(SSD1306_COLOR_t color);
void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);
void SSD1306_GotoXY(uint16_t x, uint16_t y);
char SSD1306_Putc(char ch, FontDef* Font, SSD1306_COLOR_t color);  // Changed FontDef_t to FontDef
char SSD1306_Puts(char* str, FontDef* Font, SSD1306_COLOR_t color); // Changed FontDef_t to FontDef
void SSD1306_Clear(void);
void SSD1306_PutsCentered(char* str, FontDef* Font, SSD1306_COLOR_t color); // Added this function

#endif /* INC_SSD1306_H_ */
