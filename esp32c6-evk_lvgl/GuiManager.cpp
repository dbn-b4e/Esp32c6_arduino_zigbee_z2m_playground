#include "GuiManager.h"

GuiManager* GuiManager::instance = nullptr;

GuiManager::GuiManager() {
    instance = this;
    disp_draw_buf = nullptr;
}

GuiManager::~GuiManager() {
    if (disp_draw_buf) free(disp_draw_buf);
}

void GuiManager::begin() {
    // Init écran
    screen.begin();
    screen.lcd_reg_init();
    screen.setRotation(LCD_ROTATION);
    screen.fillScreen(RGB565_BLACK);

#ifdef LCD_BL
    pinMode(LCD_BL, OUTPUT);
    digitalWrite(LCD_BL, HIGH);
#endif


    // Init tactile
    touch.begin(LCD_ROTATION, LCD_WIDTH, LCD_HEIGHT);

    // Init LVGL
    lv_init();

    screenWidth = screen.width();
    screenHeight = screen.height();
    bufSize = screenWidth * 40;

    disp_draw_buf = (lv_color_t *)malloc(bufSize * sizeof(lv_color_t));
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, bufSize);

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = displayFlushCb;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // Touch driver
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchReadCb;
    lv_indev_drv_register(&indev_drv);
}

void GuiManager::displayFlushCb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    if (instance) {
        uint32_t w = (area->x2 - area->x1 + 1);
        uint32_t h = (area->y2 - area->y1 + 1);
        instance->screen.drawBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h, LV_COLOR_16_SWAP != 0);
    }
    lv_disp_flush_ready(disp_drv);
}

void GuiManager::touchReadCb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
    if (instance) {
        touch_data_t touch_data;
        bool pressed = instance->touch.read(&touch_data);
        if (pressed) {
            data->point.x = touch_data.coords[0].x;
            data->point.y = touch_data.coords[0].y;
            data->state = LV_INDEV_STATE_PRESSED;
        } else {
            data->state = LV_INDEV_STATE_RELEASED;
        }
    }
}

void GuiManager::startLvglTask() {
    xTaskCreateUniversal(lvglLoopTask, "lvgl_loop", 64*1024, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
    printf("Lvgl task created");
}

void GuiManager::lvglLoopTask(void* arg) {
    for (;;) {
        lv_timer_handler();
        delay(5);
    }
}

void GuiManager::update() {
    // Ajoute ici ta logique périodique utilisateur (ex: MAJ widgets)
}