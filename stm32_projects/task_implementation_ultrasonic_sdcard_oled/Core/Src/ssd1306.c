#include "ssd1306.h"
#include "fonts.h"

extern I2C_HandleTypeDef hi2c1;
#define SSD1306_I2C &hi2c1

/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))

/* SSD1306 data buffer */
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

/* Private SSD1306 structure */
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
} SSD1306_t;

/* Private variable */
static SSD1306_t SSD1306;

/* Private function prototypes */
static void ssd1306_I2C_Init(void);
static void ssd1306_I2C_Write(uint8_t address, uint8_t reg, uint8_t data);
static void ssd1306_I2C_WriteMulti(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count);
#define SSD1306_WRITECOMMAND(command)      ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x00, (command))
#define SSD1306_WRITEDATA(data)            ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x40, (data))

uint8_t SSD1306_Init(void) {
    /* Init I2C */
    ssd1306_I2C_Init();

    /* Check if LCD connected to I2C */
    if (HAL_I2C_IsDeviceReady(SSD1306_I2C, SSD1306_I2C_ADDR, 1, 20000) != HAL_OK) {
        return 0;
    }

    /* A little delay */
    uint32_t p = 2500;
    while(p>0) p--;

    /* Init LCD */
    SSD1306_WRITECOMMAND(0xAE); //display off
    SSD1306_WRITECOMMAND(0x20); //Set Memory Addressing Mode
    SSD1306_WRITECOMMAND(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    SSD1306_WRITECOMMAND(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
    SSD1306_WRITECOMMAND(0xC8); //Set COM Output Scan Direction
    SSD1306_WRITECOMMAND(0x00); //---set low column address
    SSD1306_WRITECOMMAND(0x10); //---set high column address
    SSD1306_WRITECOMMAND(0x40); //--set start line address
    SSD1306_WRITECOMMAND(0x81); //--set contrast control register
    SSD1306_WRITECOMMAND(0xFF);
    SSD1306_WRITECOMMAND(0xA1); //--set segment re-map 0 to 127
    SSD1306_WRITECOMMAND(0xA6); //--set normal display
    SSD1306_WRITECOMMAND(0xA8); //--set multiplex ratio(1 to 64)
    SSD1306_WRITECOMMAND(0x3F); // for 64px height
    SSD1306_WRITECOMMAND(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    SSD1306_WRITECOMMAND(0xD3); //-set display offset
    SSD1306_WRITECOMMAND(0x00); //-not offset
    SSD1306_WRITECOMMAND(0xD5); //--set display clock divide ratio/oscillator frequency
    SSD1306_WRITECOMMAND(0xF0); //--set divide ratio
    SSD1306_WRITECOMMAND(0xD9); //--set pre-charge period
    SSD1306_WRITECOMMAND(0x22); //
    SSD1306_WRITECOMMAND(0xDA); //--set com pins hardware configuration
    SSD1306_WRITECOMMAND(0x12); // for 64px height
    SSD1306_WRITECOMMAND(0xDB); //--set vcomh
    SSD1306_WRITECOMMAND(0x20); //0x20,0.77xVcc
    SSD1306_WRITECOMMAND(0x8D); //--set DC-DC enable
    SSD1306_WRITECOMMAND(0x14); //
    SSD1306_WRITECOMMAND(0xAF); //--turn on SSD1306 panel

    /* Clear screen */
    SSD1306_Fill(SSD1306_COLOR_BLACK);

    /* Update screen */
    SSD1306_UpdateScreen();

    /* Set default values */
    SSD1306.CurrentX = 0;
    SSD1306.CurrentY = 0;
    SSD1306.Initialized = 1;

    return 1;
}

void SSD1306_UpdateScreen(void) {
    uint8_t m;

    for (m = 0; m < 8; m++) {
        SSD1306_WRITECOMMAND(0xB0 + m);
        SSD1306_WRITECOMMAND(0x00);
        SSD1306_WRITECOMMAND(0x10);

        /* Write multi data */
        ssd1306_I2C_WriteMulti(SSD1306_I2C_ADDR, 0x40, &SSD1306_Buffer[SSD1306_WIDTH * m], SSD1306_WIDTH);
    }
}

void SSD1306_Fill(SSD1306_COLOR_t color) {
    memset(SSD1306_Buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
}

void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color) {
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        return;
    }

    if (SSD1306.Inverted) {
        color = (SSD1306_COLOR_t)!color;
    }

    if (color == SSD1306_COLOR_WHITE) {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    } else {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

void SSD1306_GotoXY(uint16_t x, uint16_t y) {
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}

char SSD1306_Putc(char ch, FontDef* Font, SSD1306_COLOR_t color) {  // Changed FontDef_t to FontDef
    uint32_t i, b, j;

    if (SSD1306_WIDTH <= (SSD1306.CurrentX + Font->FontWidth) ||
        SSD1306_HEIGHT <= (SSD1306.CurrentY + Font->FontHeight)) {
        return 0;
    }

    for (i = 0; i < Font->FontHeight; i++) {
        b = Font->data[(ch - 32) * Font->FontHeight + i];
        for (j = 0; j < Font->FontWidth; j++) {
            if ((b << j) & 0x8000) {
                SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), color);
            } else {
                SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);
            }
        }
    }

    SSD1306.CurrentX += Font->FontWidth;
    return ch;
}

char SSD1306_Puts(char* str, FontDef* Font, SSD1306_COLOR_t color) {  // Changed FontDef_t to FontDef
    while (*str) {
        if (SSD1306_Putc(*str, Font, color) != *str) {
            return *str;
        }
        str++;
    }
    return *str;
}

void SSD1306_Clear(void) {
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    SSD1306_UpdateScreen();
}

void SSD1306_PutsCentered(char* str, FontDef* Font, SSD1306_COLOR_t color) {  // Changed FontDef_t to FontDef
    uint16_t text_width = 0;
    char* temp = str;

    while (*temp) {
        text_width += Font->FontWidth;
        temp++;
    }

    uint16_t start_x = (SSD1306_WIDTH - text_width) / 2;
    uint16_t start_y = (SSD1306_HEIGHT - Font->FontHeight) / 2;

    SSD1306_GotoXY(start_x, start_y);
    SSD1306_Puts(str, Font, color);
}

/* Private I2C functions */
static void ssd1306_I2C_Init(void) {
    uint32_t p = 250000;
    while(p>0) p--;
}

static void ssd1306_I2C_WriteMulti(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
    uint8_t dt[256];
    dt[0] = reg;

    for(uint8_t i = 0; i < count; i++) {
        dt[i+1] = data[i];
    }

    HAL_I2C_Master_Transmit(SSD1306_I2C, address, dt, count+1, 10);
}

static void ssd1306_I2C_Write(uint8_t address, uint8_t reg, uint8_t data) {
    uint8_t dt[2];
    dt[0] = reg;
    dt[1] = data;
    HAL_I2C_Master_Transmit(SSD1306_I2C, address, dt, 2, 10);
}
