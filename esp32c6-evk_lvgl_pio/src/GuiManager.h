#pragma once
#include "ScreenGFX.h"
#include "TouchInput.h"
#include <lvgl.h>
#include <Arduino.h>

class GuiManager {
public:
    GuiManager();
    ~GuiManager();

    void begin();
    void startLvglTask();
    void update();

    // Pour usage dans les callbacks statiques
    static void displayFlushCb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
    static void touchReadCb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

    ScreenGFX screen;
    TouchInput touch;

private:
    static GuiManager* instance;
    lv_disp_draw_buf_t draw_buf;
    lv_color_t *disp_draw_buf;
    lv_disp_drv_t disp_drv;
    uint32_t bufSize;
    uint32_t screenWidth;
    uint32_t screenHeight;

    static void lvglLoopTask(void* arg);
};