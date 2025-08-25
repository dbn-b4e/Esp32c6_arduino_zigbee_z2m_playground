#pragma once
#include <Arduino_GFX_Library.h>
#include "targ_def.h"

class ScreenGFX {
public:
    ScreenGFX();
    ~ScreenGFX();

    bool begin();
    void lcd_reg_init(void);
    void setRotation(uint8_t r);
    void fillScreen(uint16_t color);
    uint16_t width();
    uint16_t height();
    void drawBitmap(int16_t x, int16_t y, uint16_t *data, uint32_t w, uint32_t h, bool swap = false);
    Arduino_GFX* getGfx();

private:
    Arduino_DataBus *bus;
    Arduino_GFX *gfx;
};