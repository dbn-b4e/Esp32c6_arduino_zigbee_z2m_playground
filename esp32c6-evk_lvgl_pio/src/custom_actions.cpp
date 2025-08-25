#include <stdint.h>
#include "custom_actions.h"
#include "ui.h"
#include <Arduino.h>

// Variables globales liées à la gestion des écrans
static int16_t currentScreen = 1;         //Main Screen=1 keep track of which screen we are in.
int totalNumberOfScreens = 4;  //MUST change to number of screens
static lv_timer_t * Slidetimer = NULL;
bool splash_finished = false;
typedef struct  {
    uint32_t progress_max;
    uint32_t anim_steps;
    uint32_t anim_duration ;
} splash_cfg;


void action_my_event(lv_event_t *e) {
    lv_obj_t *screen = lv_event_get_current_target(e);
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());

    //Log which way we are swiping
    printf("Dir: %d\n", dir);
    switch (dir) {
        case LV_DIR_LEFT:
            printf("Dir: LEFT\n");
            //Go towards the last screen
            if (currentScreen < totalNumberOfScreens) {
                loadScreen(ScreensEnum(currentScreen + 1));
                currentScreen = currentScreen + 1;
                //lv_label_set_text(objects.label_info_last, "");
            } else {
                //lv_label_set_text(objects.label_info_last, "SWIPE RIGTH\nSWIPE RIGHT");
            }
            break;
        case LV_DIR_RIGHT:
            printf("Dir: RIGHT\n");
            //Go towards the Main/First screen
            if (currentScreen > 1) {
                loadScreen(ScreensEnum(currentScreen - 1));
                currentScreen = currentScreen - 1;
                //lv_label_set_text(objects.label_info_main, "");
            } else {
                //lv_label_set_text(objects.label_info_main, "SWIPE LEFT\nSWIPE LEFT");
            }
            break;
        default:
            break;
    }
}

// Hàm callback cho timer
static void set_value_task(lv_timer_t * timer) {

    static uint8_t value = 0;
    
    // Retrieve the splash_cfg pointer from timer's user_data
    splash_cfg *cfg = (splash_cfg *)timer->user_data;

    //splash_cfg *splash_cfg_local_ptr =  (splash_cfg *)t->user_data;
    //static uint8_t slider_step = (splash_cfg_local_ptr->anim_duration / splash_cfg_local_ptr->anim_steps);

    printf("Slide max: %d\n", cfg->progress_max);
    printf("Slide steps: %d\n", cfg->anim_steps);
    printf("Slide duration: %d\n", cfg->anim_duration);

    lv_bar_set_range(objects.pgs_splash, 0, cfg->progress_max);
    value += cfg->progress_max/cfg->anim_steps;

    if(value >cfg->progress_max)
    {
        lv_timer_del(Slidetimer);
        Slidetimer = NULL;

        loadScreen(SCREEN_ID_MAIN);
        
        return;
    }
    else
    {
        printf("Splash slider value: %d\n", value);
        lv_bar_set_value(objects.pgs_splash, value, LV_ANIM_ON); 
    }
}

void action_splashscreen_loaded(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    // Configure splash_cfg
    static splash_cfg my_cfg = {
        .progress_max = 100,
        .anim_steps = 20,
        .anim_duration = 3000 // ms
    };
    Slidetimer = lv_timer_create(set_value_task, (uint32_t) 150,&my_cfg);
}

void action_cb_value_changed(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    bool btn_state = lv_obj_has_state(obj, LV_STATE_CHECKED);

    if(code == LV_EVENT_VALUE_CHANGED) {
        //printf("State: %s\n", btn_state ? "On" : "Off");
        lv_checkbox_set_text(obj, btn_state ? "On" : "Off");
    }
}

void action_spinner_pressed(lv_event_t *e) {
    printf("Spinner pressed ");
}

void action_cbx_on_off_changed(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    bool btn_state = lv_obj_has_state(obj, LV_STATE_CHECKED);

    if(code == LV_EVENT_VALUE_CHANGED) {
        //printf("State: %s\n", btn_state ? "On" : "Off");
        lv_label_set_text(objects.lbl_on_off, btn_state ? "On" : "Off");
    }
}

void action_page1_clicked(lv_event_t *e) {
    // TODO: Implement action_page1_clicked here
}

void action_page2_clicked(lv_event_t *e) {
    // TODO: Implement action_page2_clicked here
}