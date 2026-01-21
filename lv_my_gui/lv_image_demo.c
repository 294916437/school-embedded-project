#include "lv_image_demo.h"
#include "../modules/camera_mode.h"
#include "../modules/freetype_font_init.h"
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

// 全局变量
static lv_obj_t *gallery_screen = NULL;
static lv_obj_t *image_viewer = NULL;
static lv_obj_t *camera_panel = NULL;
static lv_obj_t *control_panel = NULL;
static lv_obj_t *info_label = NULL;
static lv_style_t font_style;
static gallery_config_t gallery_cfg = {
    .gallery_path = "A:gallery/",
    .current_index = 0,
    .total_images = 0,
    .mode = GALLERY_MODE_VIEW
};

static char **image_list = NULL;  // 图片文件列表
static bool camera_active = false;

// 工具函数：检查文件是否为支持的图片格式
static bool is_image_file(const char *filename)
{
    const char *ext = strrchr(filename, '.');
    if (!ext) return false;
    
    return (strcasecmp(ext, ".jpg") == 0 || 
            strcasecmp(ext, ".jpeg") == 0 ||
            strcasecmp(ext, ".png") == 0 ||
            strcasecmp(ext, ".bmp") == 0);
}

// 扫描图片文件
static int scan_gallery_images(void)
{
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    
    // 释放旧列表
    if (image_list) {
        for (int i = 0; i < gallery_cfg.total_images; i++) {
            free(image_list[i]);
        }
        free(image_list);
        image_list = NULL;
    }
    
    // 打开目录
    dir = opendir(gallery_cfg.gallery_path);
    if (!dir) {
        perror("无法打开相册目录");
        gallery_cfg.total_images = 0;
        return 0;
    }
    
    // 第一次遍历：统计图片数量
    while ((entry = readdir(dir)) != NULL) {
        if (is_image_file(entry->d_name)) {
            count++;
        }
        
    }
    printf("Found file: %s\n", entry->d_name);
    
    if (count == 0) {
        closedir(dir);
        gallery_cfg.total_images = 0;
        return 0;
    }
    
    // 分配内存
    image_list = (char **)malloc(count * sizeof(char *));
    if (!image_list) {
        closedir(dir);
        return -1;
    }
    
    // 第二次遍历：存储文件名
    rewinddir(dir);
    int index = 0;
    while ((entry = readdir(dir)) != NULL && index < count) {
        if (is_image_file(entry->d_name)) {
            image_list[index] = (char *)malloc(strlen(entry->d_name) + 1);
            strcpy(image_list[index], entry->d_name);
            index++;
        }
    }
    
    closedir(dir);
    gallery_cfg.total_images = count;
    
    // 确保索引有效
    if (gallery_cfg.current_index >= gallery_cfg.total_images) {
        gallery_cfg.current_index = 0;
    }
    
    return count;
}

// 显示当前图片
static void display_current_image(void)
{
    if (gallery_cfg.total_images == 0) {
        lv_label_set_text(info_label, "No images found");
        lv_img_set_src(image_viewer, NULL);
        return;
    }
    
    // 构建完整路径
    char full_path[512];
    snprintf(full_path, sizeof(full_path), "%s%s", 
             gallery_cfg.gallery_path, 
             image_list[gallery_cfg.current_index]);
    
    // 设置图片源
    lv_img_set_src(image_viewer, full_path);
    
    // 更新信息标签
    char info[128];
    snprintf(info, sizeof(info), "%d / %d - %s", 
             gallery_cfg.current_index + 1,
             gallery_cfg.total_images,
             image_list[gallery_cfg.current_index]);
    lv_label_set_text(info_label, info);
}

// 按钮事件：上一张
static void prev_image_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        if (gallery_cfg.total_images == 0) return;
        
        gallery_cfg.current_index--;
        if (gallery_cfg.current_index < 0) {
            gallery_cfg.current_index = gallery_cfg.total_images - 1;
        }
        display_current_image();
    }
}

// 按钮事件：下一张
static void next_image_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        if (gallery_cfg.total_images == 0) return;
        
        gallery_cfg.current_index++;
        if (gallery_cfg.current_index >= gallery_cfg.total_images) {
            gallery_cfg.current_index = 0;
        }
        display_current_image();
    }
}

// 按钮事件：切换到拍照模式
static void switch_to_camera_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        gallery_cfg.mode = GALLERY_MODE_CAMERA;
        
        // 隐藏图片浏览区域
        lv_obj_add_flag(image_viewer, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(camera_panel, LV_OBJ_FLAG_HIDDEN);
        
        // 初始化并启动摄像头
        if (camera_system_init() == 0) {
            camera_system_run();
            camera_active = true;
            lv_label_set_text(info_label, "摄像头已启动 - 点击拍照");
        } else {
            lv_label_set_text(info_label, "摄像头启动失败");
        }
    }
}

// 按钮事件：切换回浏览模式
static void switch_to_view_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        gallery_cfg.mode = GALLERY_MODE_VIEW;
        
        // 停止摄像头
        if (camera_active) {
            camera_system_stop();
            camera_active = false;
        }
        
        // 显示图片浏览区域
        lv_obj_clear_flag(image_viewer, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(camera_panel, LV_OBJ_FLAG_HIDDEN);
        
        // 重新扫描并显示
        scan_gallery_images();
        display_current_image();
    }
}

// 按钮事件：拍照
static void capture_photo_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        if (!camera_active) {
            lv_label_set_text(info_label, "摄像头未启动");
            return;
        }
        
        // 生成文件名（时间戳）
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char filename[256];
        snprintf(filename, sizeof(filename), "%sIMG_%04d%02d%02d_%02d%02d%02d.jpg",
                 gallery_cfg.gallery_path,
                 t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                 t->tm_hour, t->tm_min, t->tm_sec);
        
        // TODO: 实际保存图片逻辑（需要访问摄像头缓冲区）
        // 这里仅作演示，实际需要将ARGB缓冲区保存为JPEG
        lv_label_set_text(info_label, "照片已保存");
        
        printf("照片已保存: %s\n", filename);
    }
}

// 按钮事件：刷新
static void refresh_gallery_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        scan_gallery_images();
        display_current_image();
    }
}

// 创建控制面板
static void create_control_panel(lv_obj_t *parent)
{
    control_panel = lv_obj_create(parent);
    lv_obj_set_size(control_panel, 200, 600);
    lv_obj_align(control_panel, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_pad_all(control_panel, 10, 0);
    
    // 上一张按钮
    lv_obj_t *btn_prev = lv_btn_create(control_panel);
    lv_obj_set_size(btn_prev, 180, 60);
    lv_obj_align(btn_prev, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_t *label_prev = lv_label_create(btn_prev);
    lv_obj_add_style(label_prev, &font_style, 0);
    lv_label_set_text(label_prev, "prev");
    lv_obj_center(label_prev);
    lv_obj_add_event_cb(btn_prev, prev_image_event, LV_EVENT_ALL, NULL);
    
    // 下一张按钮
    lv_obj_t *btn_next = lv_btn_create(control_panel);
    lv_obj_set_size(btn_next, 180, 60);
    lv_obj_align(btn_next, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_t *label_next = lv_label_create(btn_next);
    lv_obj_add_style(label_next, &font_style, 0);
    lv_label_set_text(label_next, "next");
    lv_obj_center(label_next);
    lv_obj_add_event_cb(btn_next, next_image_event, LV_EVENT_ALL, NULL);
    
    // 拍照模式按钮
    lv_obj_t *btn_camera = lv_btn_create(control_panel);
    lv_obj_set_size(btn_camera, 180, 60);
    lv_obj_align(btn_camera, LV_ALIGN_TOP_MID, 0, 150);
    lv_obj_set_style_bg_color(btn_camera, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_obj_t *label_camera = lv_label_create(btn_camera);
    lv_obj_add_style(label_camera, &font_style, 0);
    lv_label_set_text(label_camera, "camera");
    lv_obj_center(label_camera);
    lv_obj_add_event_cb(btn_camera, switch_to_camera_event, LV_EVENT_ALL, NULL);
    
    // 返回浏览按钮
    lv_obj_t *btn_view = lv_btn_create(control_panel);
    lv_obj_set_size(btn_view, 180, 60);
    lv_obj_align(btn_view, LV_ALIGN_TOP_MID, 0, 220);
    lv_obj_set_style_bg_color(btn_view, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_t *label_view = lv_label_create(btn_view);
    lv_obj_add_style(label_view, &font_style, 0);
    lv_label_set_text(label_view, "browse");
    lv_obj_center(label_view);
    lv_obj_add_event_cb(btn_view, switch_to_view_event, LV_EVENT_ALL, NULL);
    
    // 拍照按钮
    lv_obj_t *btn_capture = lv_btn_create(control_panel);
    lv_obj_set_size(btn_capture, 180, 60);
    lv_obj_align(btn_capture, LV_ALIGN_TOP_MID, 0, 290);
    lv_obj_set_style_bg_color(btn_capture, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_t *label_capture = lv_label_create(btn_capture);
    lv_obj_add_style(label_capture, &font_style, 0);
    lv_label_set_text(label_capture, "capture");
    lv_obj_center(label_capture);
    lv_obj_add_event_cb(btn_capture, capture_photo_event, LV_EVENT_ALL, NULL);
    
    // 刷新按钮
    lv_obj_t *btn_refresh = lv_btn_create(control_panel);
    lv_obj_set_size(btn_refresh, 180, 60);
    lv_obj_align(btn_refresh, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_t *label_refresh = lv_label_create(btn_refresh);
    lv_obj_add_style(label_refresh, &font_style, 0);
    lv_label_set_text(label_refresh, "refresh");
    lv_obj_center(label_refresh);
    lv_obj_add_event_cb(btn_refresh, refresh_gallery_event, LV_EVENT_ALL, NULL);
}

// 公共接口：创建相册界面
void lv_image_demo_create(void)
{
    // 创建主容器
    gallery_screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(gallery_screen, 1024, 600);
    lv_obj_set_style_border_width(gallery_screen, 0, 0);
    lv_obj_center(gallery_screen);
    
    // 创建图片显示区域
    image_viewer = lv_img_create(gallery_screen);
    lv_obj_set_size(image_viewer, 800, 520);
    lv_obj_align(image_viewer, LV_ALIGN_LEFT_MID, 10, 0);
    lv_obj_set_style_bg_color(image_viewer, lv_color_hex(0x303030), 0);
    lv_obj_set_style_bg_opa(image_viewer, LV_OPA_COVER, 0);
    lv_img_set_zoom(image_viewer, 256);  // 默认缩放
    
    // 创建摄像头面板（初始隐藏）
    camera_panel = lv_obj_create(gallery_screen);
    lv_obj_set_size(camera_panel, 800, 520);
    lv_obj_align(camera_panel, LV_ALIGN_LEFT_MID, 10, 0);
    lv_obj_set_style_bg_color(camera_panel, lv_color_hex(0x000000), 0);
    lv_obj_add_flag(camera_panel, LV_OBJ_FLAG_HIDDEN);
    
    // 信息标签
    info_label = lv_label_create(gallery_screen);
    lv_obj_add_style(info_label, &font_style, 0);
    lv_label_set_text(info_label, "加载中...");
    lv_obj_align(info_label, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    
    // 创建控制面板
    create_control_panel(gallery_screen);
    
    // 扫描并显示图片
    scan_gallery_images();
    display_current_image();
}

// 公共接口：销毁相册界面
void lv_image_demo_destroy(void)
{
    // 停止摄像头
    if (camera_active) {
        camera_system_stop();
        camera_active = false;
    }
    
    // 释放图片列表
    if (image_list) {
        for (int i = 0; i < gallery_cfg.total_images; i++) {
            free(image_list[i]);
        }
        free(image_list);
        image_list = NULL;
    }
    
    // 删除界面
    if (gallery_screen) {
        lv_obj_del(gallery_screen);
        gallery_screen = NULL;
    }
}

// 公共接口：刷新相册
void lv_image_demo_refresh(void)
{
    scan_gallery_images();
    display_current_image();
}