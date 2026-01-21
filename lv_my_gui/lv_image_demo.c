#include "lv_image_demo.h"
#include "../modules/camera_mode.h"
#include "../modules/freetype_font_init.h"
#include <time.h>
#include <unistd.h>

// ==================== 全局变量定义 ====================
static lv_obj_t *gallery_screen = NULL;
static lv_obj_t *image_viewer = NULL;
static lv_obj_t *camera_canvas = NULL;
static lv_color_t *canvas_buffer = NULL;
static lv_obj_t *control_panel = NULL;
static lv_obj_t *info_label = NULL;
static lv_style_t font_style;

// 硬编码图片列表（1.jpg ~ 6.jpg）
static const char *hardcoded_images[] = {
    "A:gallery/1.jpg",
    "A:gallery/2.jpg",
    "A:gallery/3.jpg",
    "A:gallery/4.jpg",
    "A:gallery/5.jpg",
    "A:gallery/6.jpg"
};
static const int TOTAL_IMAGES = 6;
static int current_image_index = 0;

static gallery_mode_t current_mode = GALLERY_MODE_VIEW;
static bool camera_active = false;

// ==================== 摄像头显示回调 ====================
static void camera_display_to_lvgl(int *argb_buffer, int width, int height)
{
    if (!camera_canvas || !canvas_buffer) {
        return;
    }
    
    // ARGB转LVGL颜色格式（RGB565）
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = y * width + x;
            int argb = argb_buffer[idx];
            
            // 提取RGB分量
            uint8_t r = (argb >> 16) & 0xFF;
            uint8_t g = (argb >> 8) & 0xFF;
            uint8_t b = argb & 0xFF;
            
            // 转换为LVGL颜色
            canvas_buffer[idx] = lv_color_make(r, g, b);
        }
    }
    
    // 刷新Canvas
    lv_obj_invalidate(camera_canvas);
}

// ==================== 显示当前图片 ====================
static void display_current_image(void)
{
    if (TOTAL_IMAGES == 0 || !image_viewer) {
        return;
    }
    
    // 边界检查
    if (current_image_index < 0) {
        current_image_index = 0;
    }
    if (current_image_index >= TOTAL_IMAGES) {
        current_image_index = TOTAL_IMAGES - 1;
    }
    
    // 获取当前图片路径
    const char *image_path = hardcoded_images[current_image_index];
    
    printf("加载图片: %s\n", image_path);
    
    // 设置图片源
    lv_img_set_src(image_viewer, image_path);
    
    // 更新信息标签
    if (info_label) {
        char info[128];
        snprintf(info, sizeof(info), "Image %d / %d", 
                 current_image_index + 1, TOTAL_IMAGES);
        lv_label_set_text(info_label, info);
    }
}

// ==================== 按钮事件：上一张 ====================
static void prev_image_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        if (TOTAL_IMAGES == 0) return;
        
        current_image_index--;
        if (current_image_index < 0) {
            current_image_index = TOTAL_IMAGES - 1;  // 循环到最后一张
        }
        display_current_image();
    }
}

// ==================== 按钮事件：下一张 ====================
static void next_image_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        if (TOTAL_IMAGES == 0) return;
        
        current_image_index++;
        if (current_image_index >= TOTAL_IMAGES) {
            current_image_index = 0;  // 循环到第一张
        }
        display_current_image();
    }
}

// ==================== 按钮事件：切换到拍照模式 ====================
static void switch_to_camera_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        // 防止重复切换
        if (current_mode == GALLERY_MODE_CAMERA && camera_active) {
            printf("已在拍照模式\n");
            return;
        }
        
        current_mode = GALLERY_MODE_CAMERA;
        
        // ========== 隐藏图片查看器 ==========
        if (image_viewer) {
            lv_obj_add_flag(image_viewer, LV_OBJ_FLAG_HIDDEN);
        }
        
        // ========== 创建Canvas显示摄像头 ==========
        if (!camera_canvas) {
            camera_canvas = lv_canvas_create(gallery_screen);
            lv_obj_set_size(camera_canvas, 640, 480);
            lv_obj_align(camera_canvas, LV_ALIGN_LEFT_MID, 10, 0);
            
            // 分配Canvas缓冲区
            canvas_buffer = (lv_color_t *)malloc(640 * 480 * sizeof(lv_color_t));
            if (canvas_buffer) {
                lv_canvas_set_buffer(camera_canvas, canvas_buffer, 640, 480, LV_IMG_CF_TRUE_COLOR);
                lv_obj_set_style_bg_color(camera_canvas, lv_color_hex(0x000000), 0);
            } else {
                printf("Canvas缓冲区分配失败\n");
                lv_obj_del(camera_canvas);
                camera_canvas = NULL;
                lv_obj_clear_flag(image_viewer, LV_OBJ_FLAG_HIDDEN);
                current_mode = GALLERY_MODE_VIEW;
                return;
            }
        }
        
        lv_obj_clear_flag(camera_canvas, LV_OBJ_FLAG_HIDDEN);
        
        // ========== 启动摄像头 ==========
        if (!camera_active) {
            camera_set_display_callback(camera_display_to_lvgl);
            
            if (camera_system_init() == 0) {
                camera_system_run();
                camera_active = true;
                if (info_label) {
                    lv_label_set_text(info_label, "Camera Mode - Ready to capture");
                }
            } else {
                printf("摄像头初始化失败\n");
                if (info_label) {
                    lv_label_set_text(info_label, "Camera failed to start");
                }
                lv_obj_clear_flag(image_viewer, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(camera_canvas, LV_OBJ_FLAG_HIDDEN);
                current_mode = GALLERY_MODE_VIEW;
            }
        }
    }
}

// ==================== 按钮事件：切换回浏览模式 ====================
static void switch_to_view_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        // 防止重复切换
        if (current_mode == GALLERY_MODE_VIEW && !camera_active) {
            printf("已在浏览模式\n");
            return;
        }
        
        current_mode = GALLERY_MODE_VIEW;
        
        // ========== 停止摄像头 ==========
        if (camera_active) {
            camera_set_display_callback(NULL);
            camera_system_stop();
            usleep(500000);  // 延迟500ms确保资源释放
            camera_active = false;
        }
        
        // ========== 切换显示 ==========
        if (camera_canvas) {
            lv_obj_add_flag(camera_canvas, LV_OBJ_FLAG_HIDDEN);
        }
        if (image_viewer) {
            lv_obj_clear_flag(image_viewer, LV_OBJ_FLAG_HIDDEN);
        }
        
        // 重新显示当前图片
        display_current_image();
    }
}

// ==================== 按钮事件：拍照 ====================
static void capture_photo_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        if (!camera_active) {
            if (info_label) {
                lv_label_set_text(info_label, "Camera not started");
            }
            return;
        }
        
        // 生成文件名（时间戳）
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char filename[256];
        // TODO: 实际保存ARGB缓冲区为JPEG
        // int *frame = camera_get_current_frame();
        // save_argb_to_jpeg(frame, 640, 480, filename);
        
        if (info_label) {
            lv_label_set_text(info_label, "Photo captured!");
        }
        printf("照片已保存: %s\n", filename);
    }
}

// ==================== 按钮事件：刷新图片列表 ====================
static void refresh_gallery_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        // 硬编码模式下，刷新仅重新显示当前图片
        display_current_image();
        if (info_label) {
            lv_label_set_text(info_label, "Gallery refreshed");
        }
    }
}

// ==================== 按钮事件：返回主页 ====================
static void back_to_home_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        // ========== 第1步：停止摄像头 ==========
        if (camera_active) {
            camera_set_display_callback(NULL);
            camera_system_stop();
            usleep(300000);
            camera_active = false;
        }
        
        // ========== 第2步：释放Canvas缓冲区 ==========
        if (canvas_buffer) {
            free(canvas_buffer);
            canvas_buffer = NULL;
        }
        
        // ========== 第3步：删除相册界面 ==========
        if (gallery_screen) {
            lv_obj_del(gallery_screen);
            gallery_screen = NULL;
            image_viewer = NULL;
            camera_canvas = NULL;
            control_panel = NULL;
            info_label = NULL;
        }
        
        // ========== 第4步：重置全局变量 ==========
        current_image_index = 0;
        current_mode = GALLERY_MODE_VIEW;
        
        // ========== 第5步：返回主界面 ==========
        extern void main_grid(lv_style_t *style);
        extern lv_style_t style;
        main_grid(&style);
        
        printf("已返回主界面\n");
    }
}

// ==================== 创建控制面板 ====================
static void create_control_panel(lv_obj_t *parent)
{
    control_panel = lv_obj_create(parent);
    lv_obj_set_size(control_panel, 200, 600);
    lv_obj_align(control_panel, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_pad_all(control_panel, 10, 0);
    lv_obj_set_style_bg_color(control_panel, lv_color_hex(0x2C2C2C), 0);
    lv_obj_set_style_border_width(control_panel, 0, 0);
    
    // ========== 上一张按钮 ==========
    lv_obj_t *btn_prev = lv_btn_create(control_panel);
    lv_obj_set_size(btn_prev, 180, 60);
    lv_obj_align(btn_prev, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_bg_color(btn_prev, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_t *label_prev = lv_label_create(btn_prev);
    lv_obj_add_style(label_prev, &font_style, 0);
    lv_label_set_text(label_prev, "Prev");
    lv_obj_center(label_prev);
    lv_obj_add_event_cb(btn_prev, prev_image_event, LV_EVENT_CLICKED, NULL);
    
    // ========== 下一张按钮 ==========
    lv_obj_t *btn_next = lv_btn_create(control_panel);
    lv_obj_set_size(btn_next, 180, 60);
    lv_obj_align(btn_next, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_set_style_bg_color(btn_next, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_t *label_next = lv_label_create(btn_next);
    lv_obj_add_style(label_next, &font_style, 0);
    lv_label_set_text(label_next, "Next");
    lv_obj_center(label_next);
    lv_obj_add_event_cb(btn_next, next_image_event, LV_EVENT_CLICKED, NULL);
    
    // ========== 拍照模式按钮 ==========
    lv_obj_t *btn_camera = lv_btn_create(control_panel);
    lv_obj_set_size(btn_camera, 180, 60);
    lv_obj_align(btn_camera, LV_ALIGN_TOP_MID, 0, 150);
    lv_obj_set_style_bg_color(btn_camera, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_obj_t *label_camera = lv_label_create(btn_camera);
    lv_obj_add_style(label_camera, &font_style, 0);
    lv_label_set_text(label_camera, "Camera");
    lv_obj_center(label_camera);
    lv_obj_add_event_cb(btn_camera, switch_to_camera_event, LV_EVENT_CLICKED, NULL);
    
    // ========== 返回浏览按钮 ==========
    lv_obj_t *btn_view = lv_btn_create(control_panel);
    lv_obj_set_size(btn_view, 180, 60);
    lv_obj_align(btn_view, LV_ALIGN_TOP_MID, 0, 220);
    lv_obj_set_style_bg_color(btn_view, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_t *label_view = lv_label_create(btn_view);
    lv_obj_add_style(label_view, &font_style, 0);
    lv_label_set_text(label_view, "Browse");
    lv_obj_center(label_view);
    lv_obj_add_event_cb(btn_view, switch_to_view_event, LV_EVENT_CLICKED, NULL);
    
    // ========== 拍照按钮 ==========
    lv_obj_t *btn_capture = lv_btn_create(control_panel);
    lv_obj_set_size(btn_capture, 180, 60);
    lv_obj_align(btn_capture, LV_ALIGN_TOP_MID, 0, 290);
    lv_obj_set_style_bg_color(btn_capture, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_t *label_capture = lv_label_create(btn_capture);
    lv_obj_add_style(label_capture, &font_style, 0);
    lv_label_set_text(label_capture, "Capture");
    lv_obj_center(label_capture);
    lv_obj_add_event_cb(btn_capture, capture_photo_event, LV_EVENT_CLICKED, NULL);
    
    // ========== 返回主页按钮 ==========
    lv_obj_t *btn_back = lv_btn_create(control_panel);
    lv_obj_set_size(btn_back, 180, 60);
    lv_obj_align(btn_back, LV_ALIGN_TOP_MID, 0, 360);
    lv_obj_set_style_bg_color(btn_back, lv_palette_main(LV_PALETTE_ORANGE), 0);
    lv_obj_t *label_back = lv_label_create(btn_back);
    lv_obj_add_style(label_back, &font_style, 0);
    lv_label_set_text(label_back, "Home");
    lv_obj_center(label_back);
    lv_obj_add_event_cb(btn_back, back_to_home_event, LV_EVENT_CLICKED, NULL);
    
    // ========== 刷新按钮 ==========
    lv_obj_t *btn_refresh = lv_btn_create(control_panel);
    lv_obj_set_size(btn_refresh, 180, 60);
    lv_obj_align(btn_refresh, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_bg_color(btn_refresh, lv_palette_main(LV_PALETTE_CYAN), 0);
    lv_obj_t *label_refresh = lv_label_create(btn_refresh);
    lv_obj_add_style(label_refresh, &font_style, 0);
    lv_label_set_text(label_refresh, "Refresh");
    lv_obj_center(label_refresh);
    lv_obj_add_event_cb(btn_refresh, refresh_gallery_event, LV_EVENT_CLICKED, NULL);
}

// ==================== 公共接口：创建相册界面 ====================
void lv_image_demo_create(void)
{
    // ========== 第1步：初始化字体样式 ==========
    lv_style_init(&font_style);
    lv_style_set_text_font(&font_style, &lv_font_montserrat_16);
    lv_style_set_text_color(&font_style, lv_color_white());
    
    // ========== 第2步：创建主容器 ==========
    gallery_screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(gallery_screen, 1024, 600);
    lv_obj_set_style_border_width(gallery_screen, 0, 0);
    lv_obj_set_style_bg_color(gallery_screen, lv_color_hex(0x1A1A1A), 0);
    lv_obj_center(gallery_screen);
    
    // ========== 第3步：创建图片显示区域 ==========
    image_viewer = lv_img_create(gallery_screen);
    lv_obj_set_size(image_viewer, 800, 520);
    lv_obj_align(image_viewer, LV_ALIGN_LEFT_MID, 10, 0);
    lv_obj_set_style_bg_color(image_viewer, lv_color_hex(0x303030), 0);
    lv_obj_set_style_bg_opa(image_viewer, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(image_viewer, 10, 0);
    lv_img_set_zoom(image_viewer, 256);
    
    // ========== 第4步：创建信息标签 ==========
    info_label = lv_label_create(gallery_screen);
    lv_obj_add_style(info_label, &font_style, 0);
    lv_label_set_text(info_label, "Gallery Mode");
    lv_obj_align(info_label, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    
    // ========== 第5步：创建控制面板 ==========
    create_control_panel(gallery_screen);
    
    // ========== 第6步：显示第一张图片 ==========
    display_current_image();
    
    printf("相册界面创建完成 - 共 %d 张图片\n", TOTAL_IMAGES);
}

// ==================== 公共接口：销毁相册界面 ====================
void lv_image_demo_destroy(void)
{
    // 停止摄像头
    if (camera_active) {
        camera_set_display_callback(NULL);
        camera_system_stop();
        camera_active = false;
    }
    
    // 释放Canvas缓冲区
    if (canvas_buffer) {
        free(canvas_buffer);
        canvas_buffer = NULL;
    }
    
    // 删除界面
    if (gallery_screen) {
        lv_obj_del(gallery_screen);
        gallery_screen = NULL;
        image_viewer = NULL;
        camera_canvas = NULL;
        control_panel = NULL;
        info_label = NULL;
    }
    
    // 重置状态
    current_image_index = 0;
    current_mode = GALLERY_MODE_VIEW;
    
    printf("相册界面已销毁\n");
}

// ==================== 公共接口：刷新相册 ====================
void lv_image_demo_refresh(void)
{
    display_current_image();
}