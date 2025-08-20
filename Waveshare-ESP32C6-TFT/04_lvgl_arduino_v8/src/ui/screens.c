#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 172);
    lv_obj_add_event_cb(obj, action_my_event, LV_EVENT_GESTURE, (void *)0);
    lv_obj_add_event_cb(obj, action_page1_clicked, LV_EVENT_CLICKED, (void *)0);
    {
        lv_obj_t *parent_obj = obj;
        {
            // lbl_Counter
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_counter = obj;
            lv_obj_set_pos(obj, 156, 6);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_PCT(25));
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffab00), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_ds40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_letter_space(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0000\n");
        }
        {
            // spin_loading
            lv_obj_t *obj = lv_spinner_create(parent_obj, 1000, 60);
            objects.spin_loading = obj;
            lv_obj_set_pos(obj, 10, 40);
            lv_obj_set_size(obj, 84, 78);
            lv_obj_add_event_cb(obj, action_spinner_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_blend_mode(obj, LV_BLEND_MODE_NORMAL, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_base_dir(obj, LV_BASE_DIR_RTL, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // cb_1
            lv_obj_t *obj = lv_checkbox_create(parent_obj);
            objects.cb_1 = obj;
            lv_obj_set_pos(obj, 112, 64);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_checkbox_set_text(obj, "Checkbox");
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_ds25, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // btn_onoff_1
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.btn_onoff_1 = obj;
            lv_obj_set_pos(obj, 112, 113);
            lv_obj_set_size(obj, 50, 25);
            lv_obj_add_event_cb(obj, action_cbx_on_off_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
        }
        {
            // prg_1
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.prg_1 = obj;
            lv_obj_set_pos(obj, 10, 148);
            lv_obj_set_size(obj, 292, 10);
            lv_bar_set_value(obj, 25, LV_ANIM_OFF);
        }
        {
            // lbl_on_off
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_on_off = obj;
            lv_obj_set_pos(obj, 175, 113);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &ui_font_ds25, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "lbl_on_off");
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}

void create_screen_second() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.second = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 172);
    lv_obj_add_event_cb(obj, action_my_event, LV_EVENT_GESTURE, (void *)0);
    lv_obj_add_event_cb(obj, action_page2_clicked, LV_EVENT_CLICKED, (void *)0);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_roller_create(parent_obj);
            lv_obj_set_pos(obj, 0, 62);
            lv_obj_set_size(obj, 195, 100);
            lv_roller_set_options(obj, "Option 1\nOption 2\nOption 3", LV_ROLLER_MODE_NORMAL);
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            lv_obj_set_pos(obj, 210, 62);
            lv_obj_set_size(obj, 100, 99);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Button");
                }
            }
        }
    }
    
    tick_screen_second();
}

void tick_screen_second() {
}

void create_screen_third() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.third = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 172);
    lv_obj_add_event_cb(obj, action_my_event, LV_EVENT_GESTURE, (void *)0);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_btnmatrix_create(parent_obj);
            lv_obj_set_pos(obj, 0, 34);
            lv_obj_set_size(obj, 305, 126);
            static const char *map[6] = {
                "Btn",
                "Btn",
                "\n",
                "Btn",
                "Btn",
                NULL,
            };
            lv_btnmatrix_set_map(obj, map);
        }
    }
    
    tick_screen_third();
}

void tick_screen_third() {
}

void create_screen_fourth() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.fourth = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 172);
    lv_obj_add_event_cb(obj, action_my_event, LV_EVENT_GESTURE, (void *)0);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 172);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_fourth();
}

void tick_screen_fourth() {
}

void create_screen_fifth() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.fifth = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 172);
    lv_obj_add_event_cb(obj, action_my_event, LV_EVENT_GESTURE, (void *)0);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_tileview_create(parent_obj);
            lv_obj_set_pos(obj, 0, 72);
            lv_obj_set_size(obj, 320, 100);
        }
    }
    
    tick_screen_fifth();
}

void tick_screen_fifth() {
}

void create_screen_sixt() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.sixt = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 172);
    lv_obj_add_event_cb(obj, action_my_event, LV_EVENT_GESTURE, (void *)0);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_tabview_create(parent_obj, LV_DIR_TOP, 32);
            lv_obj_set_pos(obj, 0, 72);
            lv_obj_set_size(obj, 320, 100);
        }
    }
    
    tick_screen_sixt();
}

void tick_screen_sixt() {
}

void create_screen_seventh() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.seventh = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 172);
    lv_obj_add_event_cb(obj, action_my_event, LV_EVENT_GESTURE, (void *)0);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            lv_obj_set_pos(obj, 5, 39);
            lv_obj_set_size(obj, 310, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "Option 1\nOption 2\nOption 3");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            lv_obj_t *obj = lv_list_create(parent_obj);
            lv_obj_set_pos(obj, 5, 96);
            lv_obj_set_size(obj, 180, 56);
        }
        {
            lv_obj_t *obj = lv_arc_create(parent_obj);
            lv_obj_set_pos(obj, 208, 87);
            lv_obj_set_size(obj, 98, 75);
            lv_arc_set_value(obj, 25);
        }
    }
    
    tick_screen_seventh();
}

void tick_screen_seventh() {
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_second,
    tick_screen_third,
    tick_screen_fourth,
    tick_screen_fifth,
    tick_screen_sixt,
    tick_screen_seventh,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
    create_screen_second();
    create_screen_third();
    create_screen_fourth();
    create_screen_fifth();
    create_screen_sixt();
    create_screen_seventh();
}
