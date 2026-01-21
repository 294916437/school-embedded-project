#ifndef __SCREEN_GUI_H
#define __SCREEN_GUI_H
#include "../modules/common.h"
#include "../modules/freetype_font_init.h"
#include "camera_gui.h"
#include "lvgl_music_gui.h"

//倒车影像（摄像头）
void Reversing_Radar_Camera_Mode(lv_style_t * style);
//车载音乐
void User_Music_Mode(lv_style_t * style);
//车载视频
void User_Video_Mode(lv_style_t * style);
//相册
void User_Photo_Mode(lv_style_t * style);
//设置
void User_Settings_Mode(lv_style_t * style);
//主要界面功能
void Screen_Icon_Mode(lv_style_t * style);




#endif