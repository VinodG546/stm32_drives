#ifndef INC_FONTS_H_
#define INC_FONTS_H_

#include <stdint.h>

typedef struct {
    uint8_t FontWidth;
    uint8_t FontHeight;
    const uint16_t *data;
} FontDef;  // Changed from FontDef_t to FontDef

extern FontDef Font_7x10;

#endif /* INC_FONTS_H_ */
