#include "GuiManager.h"
#include "ui.h"
#include "custom_actions.h"

GuiManager gui;

void setup() {
    Serial.begin(115200);
    gui.begin();

    // --- UI ---
    ui_init();

    gui.startLvglTask();
    Serial.println("Setup done");
}



void loop() {
    gui.update();

    static uint32_t last_update = 0;
    static uint8_t counter = 0;
    uint16_t analogVolts = analogReadMilliVolts(BAT_PIN);
    float vbat = analogVolts * 3.0 / 1000;
    char str_buffer[20];
    char uptime_str[16];

    if (millis() - last_update >= 1000) {
        last_update = millis();

        // Calculate uptime
        unsigned long secs = millis() / 1000;
        unsigned int hours = secs / 3600;
        unsigned int mins = (secs % 3600) / 60;
        unsigned int seconds = secs % 60;

        // Format uptime string
        snprintf(uptime_str, sizeof(uptime_str), "%02u:%02u:%02u", hours, mins, seconds);
        lv_label_set_text(objects.lbl_uptime, uptime_str);

        int progress = (counter++) % 100;
        sprintf(str_buffer, "%.1f V", vbat);

        lv_bar_set_value(objects.prg_1, progress, LV_ANIM_OFF);
        lv_label_set_text(objects.lbl_counter, String(progress).c_str());
        lv_label_set_text(objects.lbl_vbat, String(vbat).c_str());

    }
    delay(100);
}