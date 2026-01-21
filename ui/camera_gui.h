#ifndef __CAMERA_MODE_LVGL_GUI_H
#define __CAMERA_MODE_LVGL_GUI_H
#include "../modules/common.h"
#include "camera_gui.h"
#include "../modules/freetype_font_init.h"



#define GUI_REFRESH_INTERVAL_MS 10  // GUI刷新间隔(ms)
#define GUI_BTN_DEBOUNCE_MS     500 // 按钮防抖时间(ms)

/**
 * @brief 初始化视频监控GUI
 * @return 0:成功 其他:失败
 */
int video_gui_init(void);

/**
 * @brief 销毁视频监控GUI
 */
void video_gui_deinit(void);

/**
 * @brief 更新GUI显示的视频帧
 * @param frame 视频帧数据
 */
void video_gui_update_frame(const video_frame_t *frame);

/**
 * @brief 更新GUI显示的帧率
 * @param fps 帧率值
 */
void video_gui_update_fps(int fps);

/**
 * @brief 更新GUI状态提示
 * @param status 状态文本
 */
void video_gui_update_status(const char *status);


#endif