#include "TouchInput.h"

TouchInput::TouchInput() { wire = &Wire; }
TouchInput::~TouchInput() {}

void TouchInput::begin(uint8_t rotation, uint16_t width, uint16_t height) {
    wire->begin(TOUCH_I2C_SDA, TOUCH_I2C_SCL);
    bsp_touch_init(wire, TOUCH_RST, TOUCH_INT, rotation, width, height);
}

bool TouchInput::read(touch_data_t* data) {
    bsp_touch_read();
    return bsp_touch_get_coordinates(data);
}