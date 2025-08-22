#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *second;
    lv_obj_t *third;
    lv_obj_t *fourth;
    lv_obj_t *fifth;
    lv_obj_t *sixt;
    lv_obj_t *seventh;
    lv_obj_t *lbl_counter;
    lv_obj_t *spin_loading;
    lv_obj_t *cb_1;
    lv_obj_t *btn_onoff_1;
    lv_obj_t *prg_1;
    lv_obj_t *lbl_on_off;
    lv_obj_t *lbl_vbat;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_SECOND = 2,
    SCREEN_ID_THIRD = 3,
    SCREEN_ID_FOURTH = 4,
    SCREEN_ID_FIFTH = 5,
    SCREEN_ID_SIXT = 6,
    SCREEN_ID_SEVENTH = 7,
};

void create_screen_main();
void tick_screen_main();

void create_screen_second();
void tick_screen_second();

void create_screen_third();
void tick_screen_third();

void create_screen_fourth();
void tick_screen_fourth();

void create_screen_fifth();
void tick_screen_fifth();

void create_screen_sixt();
void tick_screen_sixt();

void create_screen_seventh();
void tick_screen_seventh();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/