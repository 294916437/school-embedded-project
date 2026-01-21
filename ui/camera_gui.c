#include "camera_gui.h"
#include <string.h>
#include <time.h>
#include "screen_gui.h"


#define LV_TIMER_REPEAT_INFINITE -1  
   
static lv_obj_t *main_container = NULL;
static lv_obj_t *camera_img = NULL;
static lv_obj_t *fps_label = NULL;
static lv_obj_t *status_label = NULL;
static lv_timer_t *refresh_timer = NULL;
static lv_img_dsc_t img_dsc = {0};
static uint32_t last_btn_click = 0;

// 视频帧缓冲区
static uint8_t *gui_frame_buf = NULL;

static void start_btn_event_cb(lv_event_t *e);
static void stop_btn_event_cb(lv_event_t *e);
static void gui_refresh_timer_cb(lv_timer_t *timer);

int video_gui_init(void)
{
    Font_Freetyle_Init("simfang.ttf", 24, &style);
    // 创建GUI布局
    create_gui_layout();
    // 分配GUI帧缓冲区
    gui_frame_buf = (uint8_t *)malloc(CAM_FRAME_WIDTH * CAM_FRAME_HEIGHT * 3);
    if (gui_frame_buf == NULL) 
    {
        video_gui_deinit();
        return -1;
    }

    // 初始化LVGL图像描述符
    memset(&img_dsc, 0, sizeof(img_dsc));
    img_dsc.header.always_zero = 0;
    img_dsc.header.w = CAM_FRAME_WIDTH;
    img_dsc.header.h = CAM_FRAME_HEIGHT;
    img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR;
    img_dsc.data_size = CAM_FRAME_WIDTH * CAM_FRAME_HEIGHT * 3;
    img_dsc.data = gui_frame_buf;
    // 初始化状态
    video_gui_update_status("监控系统: 未启动");
    video_gui_update_fps(0);

    return 0;
}

void video_gui_deinit(void)
{
    // 停止刷新定时器
    if (refresh_timer) 
    {
        lv_timer_del(refresh_timer);
        refresh_timer = NULL;
    }

    // 释放帧缓冲区
    if (gui_frame_buf) 
    {
        free(gui_frame_buf);
        gui_frame_buf = NULL;
    }

    // 删除所有控件
    if (main_container) 
    {
        lv_obj_del(main_container);
        main_container = NULL;
    }

    camera_img = NULL;
    fps_label = NULL;
    status_label = NULL;
}

void video_gui_update_frame(const video_frame_t *frame)
{
    if (frame == NULL || !frame->valid || gui_frame_buf == NULL || camera_img == NULL) {
        return;
    }

    // 拷贝帧数据到GUI缓冲区（避免直接操作视频采集缓冲区）
    memcpy(gui_frame_buf, frame->data, frame->data_size);

    // 更新LVGL图像
    lv_img_set_src(camera_img, &img_dsc);
    lv_obj_invalidate(camera_img);
}

void video_gui_update_fps(int fps)
{
    if (fps_label == NULL) 
    {
        return;
    }

    char fps_buf[32] = {0};
    snprintf(fps_buf, sizeof(fps_buf), "帧率: %d FPS", fps);
    lv_obj_add_style(fps_label, &style, NULL);
    lv_label_set_text(fps_label, fps_buf);
    lv_obj_refresh_style(fps_label, LV_PART_MAIN, LV_STYLE_PROP_ALL);
}

void video_gui_update_status(const char *status)
{
    if (status_label == NULL || status == NULL) 
    {
        return;
    }

    char status_buf[128] = {0};
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    
    snprintf(status_buf, sizeof(status_buf), "%s", status);
    lv_obj_add_style(status_label, &style, NULL);
    lv_label_set_text(status_label, status_buf);
    lv_obj_refresh_style(status_label, LV_PART_MAIN, LV_STYLE_PROP_ALL);
}

static void Back_Btn_Event_Key(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED)
    {
        lv_obj_del(main_container);
        Screen_Icon_Mode(&style);
    }
}

void create_gui_layout(void)
{
    Font_Freetyle_Init("simfang.ttf", 24, &style);
    // 创建主容器
    main_container = lv_obj_create(lv_scr_act());
    lv_obj_set_size(main_container, 1024, 600);
    lv_obj_center(main_container);
    lv_obj_set_style_border_width(main_container, 0, NULL);
    lv_obj_set_style_border_color(main_container, lv_color_hex(0x000000), NULL);
    lv_obj_set_style_bg_color(main_container, lv_color_hex(0x000000), NULL);
    // 摄像头画面控件
    camera_img = lv_img_create(main_container);
    lv_img_set_src(camera_img, "Camera_backGroud.png"); // 默认占位图
    lv_obj_set_size(camera_img, CAM_FRAME_WIDTH, CAM_FRAME_HEIGHT);
    lv_obj_align(camera_img, LV_ALIGN_CENTER, 0, 10);
    // 帧率标签
    fps_label = lv_label_create(main_container);
    lv_obj_add_style(fps_label, &style, 0);
    lv_label_set_text(fps_label, "帧率: 0 FPS");
    lv_obj_set_style_text_color(fps_label, lv_color_hex(0x00FF00), NULL);
    lv_obj_align(fps_label, LV_ALIGN_TOP_RIGHT, -20, 20);

    // 状态标签
    status_label = lv_label_create(main_container);
    lv_obj_add_style(status_label, &style, NULL);
    lv_label_set_text(status_label, "倒车映像: 未启动");
    lv_obj_set_style_text_color(status_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_align(status_label, LV_ALIGN_TOP_LEFT, 20, 20);

    // 开始按钮
    lv_obj_t *start_btn = lv_btn_create(main_container);
    lv_obj_set_size(start_btn, 100, 50);
    lv_obj_set_style_bg_color(start_btn, lv_color_hex(0x008000), LV_PART_MAIN);
    lv_obj_t *start_btn_lab = lv_label_create(start_btn);
    lv_obj_add_style(start_btn_lab, &style, NULL);
    lv_label_set_text(start_btn_lab, "开始监控");
    lv_obj_center(start_btn_lab);
    lv_obj_align(start_btn, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_add_event_cb(start_btn, start_btn_event_cb, LV_EVENT_CLICKED, NULL);

    // 停止按钮
    lv_obj_t *stop_btn = lv_btn_create(main_container);
    lv_obj_set_size(stop_btn, 100, 50);
    lv_obj_set_style_bg_color(stop_btn, lv_color_hex(0xFF0000), LV_PART_MAIN);
    lv_obj_t *stop_btn_lab = lv_label_create(stop_btn);
    lv_obj_add_style(stop_btn_lab, &style, NULL);
    lv_label_set_text(stop_btn_lab, "停止监控");
    lv_obj_center(stop_btn_lab);
    lv_obj_align(stop_btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_add_event_cb(stop_btn, stop_btn_event_cb, LV_EVENT_CLICKED, NULL);

    //返回主菜单
    lv_obj_t * Back_Btn = lv_btn_create(main_container);
    lv_obj_set_size(Back_Btn, 100, 50);
    lv_obj_align(Back_Btn, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t * Back_Btn_Lab = lv_label_create(Back_Btn);
    lv_obj_add_style(Back_Btn_Lab, &style, NULL);
    lv_label_set_text(Back_Btn_Lab, "返回");
    lv_obj_center(Back_Btn_Lab);
    lv_obj_add_event_cb(Back_Btn, Back_Btn_Event_Key, LV_EVENT_ALL, NULL);
}

static void start_btn_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) 
    {
        return;
    }

    // 按钮防抖
    uint32_t now = lv_tick_get();
    if (now - last_btn_click < GUI_BTN_DEBOUNCE_MS) 
    {
        return;
    }
    last_btn_click = now;

    // 启动视频采集
    if (video_capture_start() == 0) 
    {
        video_gui_update_status("倒车影像: 正在运行");
        
        // 创建GUI刷新定时器
        if (refresh_timer == NULL) 
        {
            refresh_timer = lv_timer_create(gui_refresh_timer_cb, GUI_REFRESH_INTERVAL_MS, NULL);
            lv_timer_set_repeat_count(refresh_timer, LV_TIMER_REPEAT_INFINITE);
        }
    } 
    else 
    {
        video_gui_update_status("监控系统: 启动失败");
    }
}

static void stop_btn_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) 
    {
        return;
    }

    // 按钮防抖
    uint32_t now = lv_tick_get();
    if (now - last_btn_click < GUI_BTN_DEBOUNCE_MS) 
    {
        return;
    }
    last_btn_click = now;

    // 停止视频采集
    video_capture_stop();
    video_gui_update_status("倒车映像: 已停止");
    video_gui_update_fps(0);

    // 停止GUI刷新定时器
    if (refresh_timer) 
    {
        lv_timer_del(refresh_timer);
        refresh_timer = NULL;
    }

    // 恢复默认占位图
    if (camera_img) 
    {
        lv_img_set_src(camera_img, "Camera_backGroud.png");
    }
}

static void gui_refresh_timer_cb(lv_timer_t *timer)
{
    // 1. 获取视频帧
    video_frame_t frame = {0};
    int ret = video_capture_get_frame(&frame);
    
    // 2. 更新帧显示
    if (ret == 0 && frame.valid) {
        video_gui_update_frame(&frame);
    }

    // 3. 更新帧率显示
    int fps = video_capture_get_fps();
    video_gui_update_fps(fps);

    // 4. 帧率异常提示
    if (fps > 0 && fps < 5) 
    {
        video_gui_update_status("监控系统: 帧率过低，可能存在异常");
    }
}