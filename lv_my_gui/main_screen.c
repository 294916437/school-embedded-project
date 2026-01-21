#include "../lvgl/lvgl.h"
#include "../airplane/air.h"  // 游戏功能
#include "lv_video_demo.h"  // 视频功能

// 游戏按钮回调
static void game_btn_click(lv_event_t *e);

// 视频按钮回调
static void video_btn_click(lv_event_t *e);

// 应用图标路径数组（根据实际资源修改路径）
static lv_obj_t *screen;
// 应用信息结构体（名称、图标路径、点击回调）
typedef struct {
    char *name;         // 应用名称
    char *img_path;     // 图标路径
    lv_event_cb_t click_cb;   // 点击事件回调
} AppInfo;

// 应用列表（顺序与图片路径数组对应）
static AppInfo app_list[] = {
    {"Game", "A:img/game.bmp", game_btn_click},
    {"Video", "A:img/video.bmp", video_btn_click}
};

// 计算应用数量
#define APP_COUNT (sizeof(app_list) / sizeof(AppInfo))

// 样式定义
static lv_style_t main_bg_style;      // 桌面背景样式
static lv_style_t app_container_style; // 应用图标容器样式
static lv_style_t app_container_pressed_style; // 按下状态样式
static lv_style_t app_label_style;    // 应用名称样式

// 游戏按钮回调
static void game_btn_click(lv_event_t *e) {
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_obj_clean(screen);
        game_init(); // 游戏界面初始化
    }
}

// 视频按钮回调
static void video_btn_click(lv_event_t *e) {
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_obj_clean(screen);
        if(lv_layer_top())
        lv_obj_clean(lv_layer_top());
        
        video_init(); // 视频界面初始化
    }
}


// 初始化手机桌面样式
static void init_phone_styles() {
    // 桌面背景（模拟手机深色主题）
    lv_style_init(&main_bg_style);
    lv_style_set_bg_opa(&main_bg_style, LV_OPA_100);

    // 应用图标容器默认样式
    lv_style_init(&app_container_style);
    lv_style_set_radius(&app_container_style, 12); // 圆角
    lv_style_set_pad_all(&app_container_style, 10);
   

    // 应用图标容器按下样式（分离状态样式，解决参数过多问题）
    lv_style_init(&app_container_pressed_style);
    lv_style_set_bg_color(&app_container_pressed_style, lv_color_hex(0x3A3A3A)); // 按下效果

    // 应用名称文字样式
    lv_style_init(&app_label_style);
    lv_style_set_text_color(&app_label_style, lv_color_hex(0xFFFFFF)); // 白色文字
    lv_style_set_text_font(&app_label_style, &lv_font_montserrat_16); // 小字体
    lv_style_set_pad_top(&app_label_style, 5); // 图标与文字间距
}

// 创建单个应用图标（容器+图片+文字）
static void create_app_icon(lv_obj_t *parent, AppInfo *app, int x, int y) {
    // 应用容器（承载图标和文字）
    lv_obj_t *app_container = lv_obj_create(parent);
    lv_obj_set_size(app_container, 128, 128); // 图标容器大小
    lv_obj_set_pos(app_container, x, y);      // 位置
    lv_style_set_bg_img_src(&app_container_style,app->img_path);  // 设置背景图片
    lv_obj_add_style(app_container, &app_container_style, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(app_container, app->click_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_scroll_dir(app_container, false); // 禁止滚动
    lv_obj_set_scrollbar_mode(app_container, LV_SCROLLBAR_MODE_OFF); // 隐藏滚动条
    // 应用图标
    lv_obj_t *app_img = lv_img_create(app_container);
    lv_img_set_src(app_img, app->img_path); // 从路径加载图片
    lv_obj_set_align(app_img,LV_ALIGN_CENTER);

}

// 创建手机样式主界面
void main_screen_init() {
    init_phone_styles();
    screen = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(screen);
    lv_obj_set_size(screen,1024,600);
    lv_obj_add_style(screen, &main_bg_style, LV_STATE_DEFAULT);
    lv_obj_set_scroll_dir(screen, false); // 禁止滚动
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF); // 隐藏滚动条
    // 初始化样式
    

    // 手机桌面标题（类似手机状态栏下方的标题）
    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "My Phone");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30); // 顶部居中

    // 图标网格布局参数（模拟手机桌面）
    int start_x = 60;    // 起始X坐标
    int start_y = 120;   // 起始Y坐标（标题下方）
    int col_spacing = 150; // 列间距
    int row_spacing = 150; // 行间距
    int col_count = 2;   // 每行显示2个图标

    // 循环创建所有应用图标
    for(int i = 0; i < APP_COUNT; i++) {
        int col = i % col_count;
        int row = i / col_count;
        int x = start_x + col * col_spacing;
        int y = start_y + row * row_spacing;
        create_app_icon(screen, &app_list[i], x, y);
    }

    // game_init();
}
