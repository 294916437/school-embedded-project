#ifndef _MUS_H_
#define _MUS_H_

// 视频控制命令
#define FIFO_PATH "/my/data/rk.fifo"
#define FIFO_OUT_PATH "/my/data/rk_out.fifo"

#define VIDEO_PLAY "pause\n"
#define VIDEO_STOP "stop\n"
#define VIDEO_NEXT "pt_step 1\n"
#define VIDEO_PREV "pt_step -1\n"
#define VIDEO_FFWD "seek 10 1\n"
#define VIDEO_SBAC "seek 0 1\n"
#define VIDEO_VOSE "volume %d 1\n",volume_
#define VIDEO_LENG "get_time_length\n"
#define VIDEO_TIME "get_time_pos\n"
#define VIDEO_VOLUME "volume %d 1\n"
#define VIDEO_QUIT "quit\n"

// 样式相关宏定义
#define BTN_RADIUS 15          // 按钮圆角
#define BTN_BG_COLOR lv_color_hex(0x2E3440)  // 按钮背景色
#define BTN_PRESSED_COLOR lv_color_hex(0x4C566A)  // 按钮按下色
#define BTN_BORDER_COLOR lv_color_hex(0x5E81AC)  // 按钮边框色
#define LABEL_COLOR lv_color_hex(0xECEFF4)  // 标签文字色
#define STATUS_COLOR_PLAY lv_color_hex(0xA3BE8C)  // 播放状态色
#define STATUS_COLOR_STOP lv_color_hex(0xBF616A)  // 停止状态色
#define STATUS_COLOR_PAUSE lv_color_hex(0xEBCB8B)  // 暂停状态色
#define SLIDER_BG lv_color_hex(0x4C566A)  // 滑块背景
#define SLIDER_INDICATOR lv_color_hex(0x5E81AC)  // 滑块已选部分
#define SLIDER_KNOB lv_color_hex(0xA3BE8C)  // 滑块按钮
#define VIDEO_BORDER_COLOR lv_color_hex(0x3B4252)  // 视频区域边框色


void video_init();
void video_cleanup();
void video_demo_close(void);
#endif
