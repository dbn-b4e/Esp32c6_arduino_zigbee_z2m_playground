#include "custom_actions.h"
#include "ui.h"
#include <Arduino.h>

// Variables globales liées à la gestion des écrans
static int16_t currentScreen = 1;         //Main Screen=1 keep track of which screen we are in.
int totalNumberOfScreens = 7;  //MUST change to number of screens

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


void action_cb_checked(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    bool btn_state = lv_obj_has_state(obj, LV_STATE_CHECKED);

    printf("State: %s\n", btn_state ? "On" : "Off");

    if(code == LV_EVENT_VALUE_CHANGED) {
        printf("State: %s\n", btn_state ? "On" : "Off");
        lv_checkbox_set_text(obj, String(btn_state).c_str());
    }
    else
    {
        lv_checkbox_set_text(obj, "--");
        printf("CB invalid\n");
    }
    
}
void action_cb_value_changed(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    bool btn_state = lv_obj_has_state(obj, LV_STATE_CHECKED);

    if(code == LV_EVENT_VALUE_CHANGED) {
        printf("State: %s\n", btn_state ? "On" : "Off");
        lv_checkbox_set_text(obj, String(btn_state).c_str());
    }
    else
    {
        lv_checkbox_set_text(obj, "--");
        printf("CB invalid\n");
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
        printf("State: %s\n", btn_state ? "On" : "Off");
        lv_label_set_text(objects.lbl_on_off, String(btn_state).c_str());
    }
}

void action_page1_clicked(lv_event_t *e) {
    // TODO: Implement action_page1_clicked here
}

void action_page2_clicked(lv_event_t *e) {
    // TODO: Implement action_page2_clicked here
}