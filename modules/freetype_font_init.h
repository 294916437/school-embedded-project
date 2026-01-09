#ifndef _FREETYPE_INIT_H
#define _FREETYPE_INIT_H
#include "../lvgl/lvgl.h"
#include "../lvgl/demos/lv_demos.h"
#include "../lv_drivers/display/fbdev.h"
#include "../lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>


static lv_ft_info_t info;
static lv_style_t style;

void font_init(char * fontPathName, int fontSize,lv_style_t * style);

#endif  /*_FREETYPE_INIT_H*/