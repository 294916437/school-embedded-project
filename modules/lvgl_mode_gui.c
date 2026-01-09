#include "lvgl_mode_gui.h"
#include <stdbool.h>

// Global button objects
lv_obj_t * btn1 = NULL;
lv_obj_t * btn2 = NULL;
lv_obj_t * btn3 = NULL;
lv_obj_t * btn4 = NULL;
lv_obj_t * btn5 = NULL; // All ON
lv_obj_t * btn6 = NULL; // All OFF
lv_obj_t * btn7 = NULL; // Buzzer

// Labels for buttons
lv_obj_t * Label1 = NULL;
lv_obj_t * Label2 = NULL;
lv_obj_t * Label3 = NULL;
lv_obj_t * Label4 = NULL;
lv_obj_t * Label5 = NULL;
lv_obj_t * Label6 = NULL;
lv_obj_t * Label7 = NULL;

// Status flags
bool LED1_Status = false;
bool LED2_Status = false;
bool LED3_Status = false;
bool LED4_Status = false;
bool Buzzer_Status = false;

// Helper to set LED hardware (protected for preview)
static void safe_led_ctrl(int led_num, int status) {
#ifndef LVGL_LIVE_PREVIEW
    led_ctrl(led_num, status);
#endif
}

// Helper to set Buzzer hardware (protected for preview)
static void safe_buzz_ctrl(int status) {
#ifndef LVGL_LIVE_PREVIEW
    buzz_ctrl(0,status);
#endif
}

// --- Individual LED Handlers ---

static void LED1_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        LED1_Status = !LED1_Status;
        if(LED1_Status){
            safe_led_ctrl(0, 1);
            lv_obj_set_style_bg_color(btn1, lv_color_hex(0xff0000), 0);
            lv_label_set_text(Label1, "LED1 ON");
        }else{
            safe_led_ctrl(0, 0);
            lv_obj_set_style_bg_color(btn1, lv_color_hex(0x0000ff), 0);
            lv_label_set_text(Label1, "LED1 OFF");
        }
    }
}

static void LED2_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        LED2_Status = !LED2_Status;
        if(LED2_Status){
            safe_led_ctrl(1, 1);
            lv_obj_set_style_bg_color(btn2, lv_color_hex(0xff0000), 0);
            lv_label_set_text(Label2, "LED2 ON");
        }else{
            safe_led_ctrl(1, 0);
            lv_obj_set_style_bg_color(btn2, lv_color_hex(0x0000ff), 0);
            lv_label_set_text(Label2, "LED2 OFF");
        }
    }
}

static void LED3_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        LED3_Status = !LED3_Status;
        if(LED3_Status){
            safe_led_ctrl(2, 1);
            lv_obj_set_style_bg_color(btn3, lv_color_hex(0xff0000), 0);
            lv_label_set_text(Label3, "LED3 ON");
        }else{
            safe_led_ctrl(2, 0);
            lv_obj_set_style_bg_color(btn3, lv_color_hex(0x0000ff), 0);
            lv_label_set_text(Label3, "LED3 OFF");
        }
    }
}

static void LED4_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        LED4_Status = !LED4_Status;
        if(LED4_Status){
            safe_led_ctrl(3, 1);
            lv_obj_set_style_bg_color(btn4, lv_color_hex(0xff0000), 0);
            lv_label_set_text(Label4, "LED4 ON");
        }else{
            safe_led_ctrl(3, 0);
            lv_obj_set_style_bg_color(btn4, lv_color_hex(0x0000ff), 0);
            lv_label_set_text(Label4, "LED4 OFF");
        }
    }
}

// --- Global Control Handlers ---

static void All_On_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        // Turn ON hardware
        safe_led_ctrl(0, 1);
        safe_led_ctrl(1, 1);
        safe_led_ctrl(2, 1);
        safe_led_ctrl(3, 1);
        
        // Update states
        LED1_Status = true;
        LED2_Status = true;
        LED3_Status = true;
        LED4_Status = true;

        // Update UI styles
        lv_obj_set_style_bg_color(btn1, lv_color_hex(0xff0000), 0);
        lv_label_set_text(Label1, "LED1 ON");
        lv_obj_set_style_bg_color(btn2, lv_color_hex(0xff0000), 0);
        lv_label_set_text(Label2, "LED2 ON");
        lv_obj_set_style_bg_color(btn3, lv_color_hex(0xff0000), 0);
        lv_label_set_text(Label3, "LED3 ON");
        lv_obj_set_style_bg_color(btn4, lv_color_hex(0xff0000), 0);
        lv_label_set_text(Label4, "LED4 ON");
    }
}

static void All_Off_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        // Turn OFF hardware
        safe_led_ctrl(0, 0);
        safe_led_ctrl(1, 0);
        safe_led_ctrl(2, 0);
        safe_led_ctrl(3, 0);
        
        // Update states
        LED1_Status = false;
        LED2_Status = false;
        LED3_Status = false;
        LED4_Status = false;

        // Update UI styles
        lv_obj_set_style_bg_color(btn1, lv_color_hex(0x0000ff), 0);
        lv_label_set_text(Label1, "LED1 OFF");
        lv_obj_set_style_bg_color(btn2, lv_color_hex(0x0000ff), 0);
        lv_label_set_text(Label2, "LED2 OFF");
        lv_obj_set_style_bg_color(btn3, lv_color_hex(0x0000ff), 0);
        lv_label_set_text(Label3, "LED3 OFF");
        lv_obj_set_style_bg_color(btn4, lv_color_hex(0x0000ff), 0);
        lv_label_set_text(Label4, "LED4 OFF");
    }
}

static void Buzzer_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        Buzzer_Status = !Buzzer_Status;
        if(Buzzer_Status){
            safe_buzz_ctrl(1);
            lv_obj_set_style_bg_color(btn7, lv_color_hex(0xffa500), 0); // Orange for active
            lv_label_set_text(Label7, "Buzz ON");
        }else{
            safe_buzz_ctrl(0);
            lv_obj_set_style_bg_color(btn7, lv_color_hex(0x555555), 0); // Gray for inactive
            lv_label_set_text(Label7, "Buzz OFF");
        }
    }
}

void lvgl_mode_gui_start(void)
{
    lv_obj_t * lvgl_mode_screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(lvgl_mode_screen, 1024, 600); // Adjusted to LCD size from main.c
    lv_obj_set_style_bg_color(lvgl_mode_screen, lv_color_hex(0x222222), 0);
    lv_obj_center(lvgl_mode_screen);
    
    // --- Row 1: 4 LED Control Buttons ---
    int btn_w = 150;
    int btn_h = 100;
    int start_x = 50;
    int gap_x = 220; // Distance between button starts
    int row1_y = 100;

    // Button 1
    btn1 = lv_btn_create(lvgl_mode_screen);
    lv_obj_set_size(btn1, btn_w, btn_h);
    lv_obj_set_pos(btn1, start_x + 0*gap_x, row1_y);
    lv_obj_set_style_bg_color(btn1, lv_color_hex(0x0000ff), 0); // Default Blue (OFF)
    Label1 = lv_label_create(btn1);
    lv_label_set_text(Label1, "LED1 OFF");
    lv_obj_center(Label1);
    lv_obj_add_event_cb(btn1, LED1_Key, LV_EVENT_CLICKED, 0);

    // Button 2
    btn2 = lv_btn_create(lvgl_mode_screen);
    lv_obj_set_size(btn2, btn_w, btn_h);
    lv_obj_set_pos(btn2, start_x + 1*gap_x, row1_y);
    lv_obj_set_style_bg_color(btn2, lv_color_hex(0x0000ff), 0);
    Label2 = lv_label_create(btn2);
    lv_label_set_text(Label2, "LED2 OFF");
    lv_obj_center(Label2);
    lv_obj_add_event_cb(btn2, LED2_Key, LV_EVENT_CLICKED, 0);

    // Button 3
    btn3 = lv_btn_create(lvgl_mode_screen);
    lv_obj_set_size(btn3, btn_w, btn_h);
    lv_obj_set_pos(btn3, start_x + 2*gap_x, row1_y);
    lv_obj_set_style_bg_color(btn3, lv_color_hex(0x0000ff), 0);
    Label3 = lv_label_create(btn3);
    lv_label_set_text(Label3, "LED3 OFF");
    lv_obj_center(Label3);
    lv_obj_add_event_cb(btn3, LED3_Key, LV_EVENT_CLICKED, 0);

    // Button 4
    btn4 = lv_btn_create(lvgl_mode_screen);
    lv_obj_set_size(btn4, btn_w, btn_h);
    lv_obj_set_pos(btn4, start_x + 3*gap_x, row1_y);
    lv_obj_set_style_bg_color(btn4, lv_color_hex(0x0000ff), 0);
    Label4 = lv_label_create(btn4);
    lv_label_set_text(Label4, "LED4 OFF");
    lv_obj_center(Label4);
    lv_obj_add_event_cb(btn4, LED4_Key, LV_EVENT_CLICKED, 0);


    // --- Row 2: 3 Control Buttons (All ON, All OFF, Buzzer) ---
    int row2_y = 350;
    // Recalculate gap for 3 buttons in same width or center them
    // Let's center 3 buttons roughly below the 4 above
    int gap_x_row2 = 280; 
    int start_x_row2 = 100;

    // Button 5: All ON
    btn5 = lv_btn_create(lvgl_mode_screen);
    lv_obj_set_size(btn5, btn_w, btn_h);
    lv_obj_set_pos(btn5, start_x_row2 + 0*gap_x_row2, row2_y);
    lv_obj_set_style_bg_color(btn5, lv_color_hex(0x00cc00), 0); // Greenish
    Label5 = lv_label_create(btn5);
    lv_label_set_text(Label5, "ALL ON");
    lv_obj_center(Label5);
    lv_obj_add_event_cb(btn5, All_On_Key, LV_EVENT_CLICKED, 0);

    // Button 6: All OFF
    btn6 = lv_btn_create(lvgl_mode_screen);
    lv_obj_set_size(btn6, btn_w, btn_h);
    lv_obj_set_pos(btn6, start_x_row2 + 1*gap_x_row2, row2_y);
    lv_obj_set_style_bg_color(btn6, lv_color_hex(0x444444), 0); // Grayish
    Label6 = lv_label_create(btn6);
    lv_label_set_text(Label6, "ALL OFF");
    lv_obj_center(Label6);
    lv_obj_add_event_cb(btn6, All_Off_Key, LV_EVENT_CLICKED, 0);

    // Button 7: Buzzer Toggle
    btn7 = lv_btn_create(lvgl_mode_screen);
    lv_obj_set_size(btn7, btn_w, btn_h);
    lv_obj_set_pos(btn7, start_x_row2 + 2*gap_x_row2, row2_y);
    lv_obj_set_style_bg_color(btn7, lv_color_hex(0x555555), 0); // Default inactive
    Label7 = lv_label_create(btn7);
    lv_label_set_text(Label7, "Buzz OFF");
    lv_obj_center(Label7);
    lv_obj_add_event_cb(btn7, Buzzer_Key, LV_EVENT_CLICKED, 0);
}