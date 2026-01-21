#ifndef __LV_IMAGE_DEMO_H
#define __LV_IMAGE_DEMO_H

#include "lvgl/lvgl.h"
#include <stdbool.h>

// 相册模式枚举
typedef enum {
    GALLERY_MODE_VIEW = 0,  // 浏览模式
    GALLERY_MODE_CAMERA     // 拍照模式
} gallery_mode_t;

// 相册配置结构体
typedef struct {
    char gallery_path[256];      // 图片存储路径
    int current_index;           // 当前图片索引
    int total_images;            // 图片总数
    gallery_mode_t mode;         // 当前模式
} gallery_config_t;

// 公共接口函数
void lv_image_demo_create(void);
void lv_image_demo_destroy(void);
void lv_image_demo_refresh(void);

#endif