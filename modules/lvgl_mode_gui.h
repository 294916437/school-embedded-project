#ifndef _LVGL_MODE_GUI_H
#define _LVGL_MODE_GUI_H
#include "../lvgl/lvgl.h"
#include "../lvgl/demos/lv_demos.h"
#include "../lv_drivers/display/fbdev.h"
#include "../lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "../modules/buzzer.h"
#include "../modules/LED.h"

void lvgl_mode_gui_start(void);
#endif


