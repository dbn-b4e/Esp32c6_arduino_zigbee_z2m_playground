#pragma once
#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

void action_my_event(lv_event_t *e);
void action_spinner_pressed(lv_event_t *e);
void action_cbx_on_off_changed(lv_event_t *e);
void action_cb_value_changed(lv_event_t * e);
void action_cb_checked(lv_event_t * e);
void action_page1_clicked(lv_event_t *e);
void action_page2_clicked(lv_event_t *e);

#ifdef __cplusplus
}
#endif