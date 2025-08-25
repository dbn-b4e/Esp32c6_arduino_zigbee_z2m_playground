#pragma once
#include "targ_def.h"
#include <Wire.h>
#include "esp_lcd_touch_axs5106l.h"

class TouchInput {
public:
    TouchInput();
    ~TouchInput();

    void begin(uint8_t rotation, uint16_t width, uint16_t height);
    bool read(touch_data_t* data);

private:
    TwoWire* wire;
};