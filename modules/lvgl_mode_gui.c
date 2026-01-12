#include "lvgl_mode_gui.h"
#include <stdbool.h>

// 按钮对象
lv_obj_t * btn1 = NULL;
lv_obj_t * btn2 = NULL;
lv_obj_t * btn3 = NULL;
lv_obj_t * btn4 = NULL;
lv_obj_t * btn5 = NULL; // 全开
lv_obj_t * btn6 = NULL; // 全关
lv_obj_t * btn7 = NULL; // 蜂鸣器

// 标签
lv_obj_t * Label1 = NULL;
lv_obj_t * Label2 = NULL;
lv_obj_t * Label3 = NULL;
lv_obj_t * Label4 = NULL;
lv_obj_t * Label5 = NULL;
lv_obj_t * Label6 = NULL;
lv_obj_t * Label7 = NULL;

// 状态标志
bool LED1_Status = false;
bool LED2_Status = false;
bool LED3_Status = false;
bool LED4_Status = false;
bool Buzzer_Status = false;

// LED控制函数



// LED1控制
static void LED1_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        LED1_Status = !LED1_Status;
        if(LED1_Status){
            led_ctrl(0, 1);
            lv_obj_set_style_bg_color(btn1, lv_color_hex(0xff0000), 0);
            lv_label_set_text(Label1, "LED1 ON");
        }else{
            led_ctrl(0, 0);
            lv_obj_set_style_bg_color(btn1, lv_color_hex(0x0000ff), 0);
            lv_label_set_text(Label1, "LED1 OFF");
        }
    }
}

// LED2控制
static void LED2_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        LED2_Status = !LED2_Status;
        if(LED2_Status){
            led_ctrl(1, 1);
            lv_obj_set_style_bg_color(btn2, lv_color_hex(0xff0000), 0);
            lv_label_set_text(Label2, "LED2 ON");
        }else{
            led_ctrl(1, 0);
            lv_obj_set_style_bg_color(btn2, lv_color_hex(0x0000ff), 0);
            lv_label_set_text(Label2, "LED2 OFF");
        }
    }
}

// LED3控制
static void LED3_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        LED3_Status = !LED3_Status;
        if(LED3_Status){
            led_ctrl(2, 1);
            lv_obj_set_style_bg_color(btn3, lv_color_hex(0xff0000), 0);
            lv_label_set_text(Label3, "LED3 ON");
        }else{
            led_ctrl(2, 0);
            lv_obj_set_style_bg_color(btn3, lv_color_hex(0x0000ff), 0);
            lv_label_set_text(Label3, "LED3 OFF");
        }
    }
}

// LED4控制
static void LED4_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        LED4_Status = !LED4_Status;
        if(LED4_Status){
            led_ctrl(3, 1);
            lv_obj_set_style_bg_color(btn4, lv_color_hex(0xff0000), 0);
            lv_label_set_text(Label4, "LED4 ON");
        }else{
            led_ctrl(3, 0);
            lv_obj_set_style_bg_color(btn4, lv_color_hex(0x0000ff), 0);
            lv_label_set_text(Label4, "LED4 OFF");
        }
    }
}

// 全部打开
static void All_On_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        // 打开所有LED
        led_ctrl(0, 1);
        led_ctrl(1, 1);
        led_ctrl(2, 1);
        led_ctrl(3, 1);
        
        // 更新状态
        LED1_Status = true;
        LED2_Status = true;
        LED3_Status = true;
        LED4_Status = true;

        // 更新界面
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

// 全部关闭
static void All_Off_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        // 关闭所有LED
        led_ctrl(0, 0);
        led_ctrl(1, 0);
        led_ctrl(2, 0);
        led_ctrl(3, 0);
        
        // 更新状态
        LED1_Status = false;
        LED2_Status = false;
        LED3_Status = false;
        LED4_Status = false;

        // 更新界面
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

// 蜂鸣器控制
static void Buzzer_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        Buzzer_Status = !Buzzer_Status;
        if(Buzzer_Status){
            buzz_ctrl(0,1);
            lv_obj_set_style_bg_color(btn7, lv_color_hex(0xffa500), 0); // 橙色表示激活
            lv_label_set_text(Label7, "Buzz ON");
        }else{
            buzz_ctrl(0,0);
            lv_obj_set_style_bg_color(btn7, lv_color_hex(0x555555), 0); // 灰色表示关闭
            lv_label_set_text(Label7, "Buzz OFF");
        }
    }
}

void lvgl_mode_gui_start(void)
{
    lv_obj_t * lvgl_mode_screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(lvgl_mode_screen, 1024, 600);
    lv_obj_set_style_bg_color(lvgl_mode_screen, lv_color_hex(0x222222), 0);
    lv_obj_center(lvgl_mode_screen);
    
    // 第一行: 4个LED控制按钮
    int btn_w = 150;
    int btn_h = 100;
    int start_x = 50;
    int gap_x = 220;
    int row1_y = 100;

    // 按钮1
    btn1 = lv_btn_create(lvgl_mode_screen);
    lv_obj_set_size(btn1, btn_w, btn_h);
    lv_obj_set_pos(btn1, start_x + 0*gap_x, row1_y);
    lv_obj_set_style_bg_color(btn1, lv_color_hex(0x0000ff), 0); // 蓝色表示关闭
    Label1 = lv_label_create(btn1);
    lv_label_set_text(Label1, "LED1 OFF");
    lv_obj_center(Label1);
    lv_obj_add_event_cb(btn1, LED1_Key, LV_EVENT_CLICKED, 0);

    // 按钮2
    btn2 = lv_btn_create(lvgl_mode_screen);
    lv_obj_set_size(btn2, btn_w, btn_h);
    lv_obj_set_pos(btn2, start_x + 1*gap_x, row1_y);
    lv_obj_set_style_bg_color(btn2, lv_color_hex(0x0000ff), 0);
    Label2 = lv_label_create(btn2);
    lv_label_set_text(Label2, "LED2 OFF");
    lv_obj_center(Label2);
    lv_obj_add_event_cb(btn2, LED2_Key, LV_EVENT_CLICKED, 0);

    // 按钮3
    btn3 = lv_btn_create(lvgl_mode_screen);
    lv_obj_set_size(btn3, btn_w, btn_h);
    lv_obj_set_pos(btn3, start_x + 2*gap_x, row1_y);
    lv_obj_set_style_bg_color(btn3, lv_color_hex(0x0000ff), 0);
    Label3 = lv_label_create(btn3);
    lv_label_set_text(Label3, "LED3 OFF");
    lv_obj_center(Label3);
    lv_obj_add_event_cb(btn3, LED3_Key, LV_EVENT_CLICKED, 0);

    // 按钮4
    btn4 = lv_btn_create(lvgl_mode_screen);
    lv_obj_set_size(btn4, btn_w, btn_h);
    lv_obj_set_pos(btn4, start_x + 3*gap_x, row1_y);
    lv_obj_set_style_bg_color(btn4, lv_color_hex(0x0000ff), 0);
    Label4 = lv_label_create(btn4);
    lv_label_set_text(Label4, "LED4 OFF");
    lv_obj_center(Label4);
    lv_obj_add_event_cb(btn4, LED4_Key, LV_EVENT_CLICKED, 0);


    // 第二行: 3个控制按钮(全开/全关/蜂鸣器)
    int row2_y = 350;
    int gap_x_row2 = 280; 
    int start_x_row2 = 100;

    // 按钮5: 全部打开
    btn5 = lv_btn_create(lvgl_mode_screen);
    lv_obj_set_size(btn5, btn_w, btn_h);
    lv_obj_set_pos(btn5, start_x_row2 + 0*gap_x_row2, row2_y);
    lv_obj_set_style_bg_color(btn5, lv_color_hex(0x00cc00), 0); // 绿色
    Label5 = lv_label_create(btn5);
    lv_label_set_text(Label5, "ALL ON");
    lv_obj_center(Label5);
    lv_obj_add_event_cb(btn5, All_On_Key, LV_EVENT_CLICKED, 0);

    // 按钮6: 全部关闭
    btn6 = lv_btn_create(lvgl_mode_screen);
    lv_obj_set_size(btn6, btn_w, btn_h);
    lv_obj_set_pos(btn6, start_x_row2 + 1*gap_x_row2, row2_y);
    lv_obj_set_style_bg_color(btn6, lv_color_hex(0x444444), 0); // 灰色
    Label6 = lv_label_create(btn6);
    lv_label_set_text(Label6, "ALL OFF");
    lv_obj_center(Label6);
    lv_obj_add_event_cb(btn6, All_Off_Key, LV_EVENT_CLICKED, 0);

    // 按钮7: 蜂鸣器开关
    btn7 = lv_btn_create(lvgl_mode_screen);
    lv_obj_set_size(btn7, btn_w, btn_h);
    lv_obj_set_pos(btn7, start_x_row2 + 2*gap_x_row2, row2_y);
    lv_obj_set_style_bg_color(btn7, lv_color_hex(0x555555), 0); // 默认灰色
    Label7 = lv_label_create(btn7);
    lv_label_set_text(Label7, "Buzz OFF");
    lv_obj_center(Label7);
    lv_obj_add_event_cb(btn7, Buzzer_Key, LV_EVENT_CLICKED, 0);
}