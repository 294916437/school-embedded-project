#ifndef _LVGL_VIDEO_GUI_H
#define _LVGL_VIDEO_GUI_H
#include "../modules/common.h"
#include "../modules/video_mode.h"

#define FIFO_PATH "/rk3568/rk.fifo"
#define FIFO_OUT_PATH "/rk3568/rk_out.fifo"

#define VIDEO_PLAY "pause\n"
#define VIDEO_STOP "stop\n"
#define VIDEO_LENG "get_time_length\n"
#define VIDEO_TIME "get_time_pos\n"
#define VIDEO_VOLUME "volume %d 1\n"

void video_gui_init(lv_style_t* style);


#endif