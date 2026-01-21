#include "screen_gui.h"

lv_obj_t * Screen = NULL;
lv_obj_t * camera_Icon = NULL;
lv_obj_t * Music_Icon = NULL;

static void Camera_Key_Event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED)
    {
         // 初始化视频采集模块（摄像头）
        if (video_capture_init() != 0)
        {
            return -1;
        }
        // 初始化视频GUI模块
        if (video_gui_init() != 0)
        {
            video_capture_deinit();  // 失败时释放已初始化资源
            return -1;
        }
    }
}

//倒车影像（摄像头）
void Reversing_Radar_Camera_Mode(lv_style_t * style)
{
    camera_Icon = lv_btn_create(Screen);
    lv_obj_set_size(camera_Icon, 130, 130);
    lv_obj_set_style_bg_img_src(camera_Icon, "A:camera_Icon.png", NULL);
    lv_obj_align(camera_Icon, LV_ALIGN_TOP_MID, -100, 100);

    lv_obj_set_style_bg_opa(camera_Icon, LV_OPA_0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(camera_Icon, LV_OPA_0, LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(camera_Icon, LV_OPA_0, LV_STATE_CHECKED);
    
    lv_obj_set_style_border_width(camera_Icon, 0, NULL);
    lv_obj_set_style_outline_opa(camera_Icon, 0, NULL);
    lv_obj_set_style_radius(camera_Icon, 100, NULL);

    lv_obj_add_event_cb(camera_Icon, Camera_Key_Event, LV_EVENT_ALL, NULL);
}

static void Music_Key_Event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED)
    {
        lv_obj_del(Music_Icon);
        lv_obj_del(camera_Icon);
        //lv_obj_del(Screen);
        Music_Screen_Gui(&style); 
    }
}

//车载音乐
void User_Music_Mode(lv_style_t * style)
{
    Music_Icon = lv_btn_create(Screen);
    lv_obj_set_size(Music_Icon, 130, 130);
    lv_obj_align_to(Music_Icon, camera_Icon, LV_ALIGN_OUT_RIGHT_MID, 80, 0);
    lv_obj_set_style_bg_img_src(Music_Icon, "A:QQ_Music.png", NULL);

    lv_obj_set_style_bg_opa(Music_Icon, LV_OPA_0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(Music_Icon, LV_OPA_0, LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(Music_Icon, LV_OPA_0, LV_STATE_CHECKED);
    
    lv_obj_set_style_border_width(Music_Icon, 0, NULL);
    lv_obj_set_style_outline_opa(Music_Icon, 0, NULL);
    lv_obj_set_style_radius(Music_Icon, 100, NULL);

    lv_obj_add_event_cb(Music_Icon, Music_Key_Event, LV_EVENT_ALL, NULL);
}

//车载视频
void User_Video_Mode(lv_style_t * style)
{
    
}

//相册
void User_Photo_Mode(lv_style_t * style)
{

}

//设置
void User_Settings_Mode(lv_style_t * style)
{

}

//主要界面功能
void Screen_Icon_Mode(lv_style_t * style)
{
    Screen = lv_scr_act();

    Reversing_Radar_Camera_Mode(&style);
    User_Music_Mode(&style);

}