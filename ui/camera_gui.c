#include "camera_gui.h"
#include "camera_mode.h"

lv_obj_t * Screen = NULL;
lv_obj_t * Screen_Img = NULL;
lv_obj_t * Open_Key = NULL;
lv_obj_t * Open_key_Lab = NULL;
lv_obj_t * Close_Key = NULL;
lv_obj_t * Close_Key_Lab = NULL;
lv_obj_t * Back_Key = NULL;
lv_obj_t * Back_Key_Lab = NULL;
bool Stop_Camera_status = false;

// 开启按钮事件：启动摄像头
static void Open_Event_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED)
    {
        printf("点击开启摄像头\n");
        // 初始化摄像头系统 + 启动采集
        if (camera_system_init() == 0)
        {
            Stop_Camera_status = true;
            camera_system_run();
        }
    }
}

// 关闭按钮事件：停止摄像头
static void Close_Event_Key(lv_event_t * e)
{
     lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED)
    {
        Stop_Camera_status = false;
        printf("点击关闭摄像头\n");
        camera_system_stop();
    }
}

// 返回按钮事件：退出GUI + 释放资源
static void Back_Event_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED)
    {
        printf("点击返回，退出摄像头GUI\n");
        if(Stop_Camera_status)
        {
            Stop_Camera_status = false;
            printf("点击关闭摄像头\n");
            camera_system_stop();
        }
        LVGL_Screen(&style);
        lv_obj_del(Screen); // 销毁GUI界面
    }
}

void Open_Key_Btn(lv_style_t * style)
{
    Open_Key = lv_btn_create(Screen);
    lv_obj_set_size(Open_Key, 100, 80);
    lv_obj_align(Open_Key, LV_ALIGN_RIGHT_MID,  0, 20); 
    Open_key_Lab = lv_label_create(Open_Key);
    lv_obj_add_style(Open_key_Lab, style, NULL);
    lv_label_set_text(Open_key_Lab, "开启按钮");
    lv_obj_center(Open_key_Lab);
    lv_obj_add_event_cb(Open_Key, Open_Event_Key, LV_EVENT_ALL, NULL);
}

void Close_Key_Btn(lv_style_t * style)
{
    Close_Key = lv_btn_create(Screen);
    lv_obj_set_size(Close_Key, 100, 80);
    lv_obj_align_to(Close_Key, Open_Key, LV_ALIGN_OUT_BOTTOM_MID,  0, 20); 
    Close_Key_Lab = lv_label_create(Close_Key);
    lv_obj_add_style(Close_Key_Lab, style, NULL);
    lv_label_set_text(Close_Key_Lab, "关闭按钮");
    lv_obj_center(Close_Key_Lab);
    lv_obj_add_event_cb(Close_Key, Close_Event_Key, LV_EVENT_ALL, NULL);
}

void Back_Key_Btn(lv_style_t * style)
{
    Back_Key = lv_btn_create(Screen);
    lv_obj_set_size(Back_Key, 100, 80);
    lv_obj_align_to(Back_Key, Close_Key, LV_ALIGN_OUT_BOTTOM_MID,  0, 20); 
    Back_Key_Lab = lv_label_create(Back_Key);
    lv_obj_add_style(Back_Key_Lab, style, NULL);
    lv_label_set_text(Back_Key_Lab, "返回按钮"); // 修复标签错误
    lv_obj_center(Back_Key_Lab);
    lv_obj_add_event_cb(Back_Key, Back_Event_Key, LV_EVENT_ALL, NULL); // 绑定独立返回事件
}

void Screen_Gui(lv_style_t * style)
{
    Screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(Screen, 1024, 600);
    lv_obj_set_style_border_width(Screen, 0, NULL);
    lv_obj_center(Screen);

    // Screen_Img = lv_img_create(Screen);
    // lv_obj_set_size(Screen_Img, 600, 480);
    // lv_obj_center(Screen_Img);

    Open_Key_Btn(style);
    Close_Key_Btn(style);
    Back_Key_Btn(style);
}