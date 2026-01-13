#ifndef _SHOW_IMG_H
#define _SHOW_IMG_H
#include "../lvgl/lvgl.h"
#include "../lvgl/demos/lv_demos.h"
#include "../lv_drivers/display/fbdev.h"
#include "../lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

// 图片显示函数 - 传递图片路径参数
void JPG_Mode(const char *img_src);
void PNG_Mode(const char *img_src);
void GIF_Mode(const char *img_src);

// 便捷函数 - 自动检测图片类型并显示
void show_image(const char *img_src);

#endif