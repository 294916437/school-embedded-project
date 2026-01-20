#include "lv_setting_demo.h"
#include "lvgl.h"
#include "lv_run_main.h"


static lv_point_t line_points[] ={{0,0},{650,0}};

LV_FONT_DECLARE(setting_font);
LV_FONT_DECLARE(setting_font1);
LV_FONT_DECLARE(setting_img);
LV_FONT_DECLARE(setting_font_about);
LV_FONT_DECLARE(setting_font_about1);
LV_IMG_DECLARE(setting_about);

lv_anim_t about_anim;               /*左移动画*/
lv_obj_t *lv_setting_cont;          /* 主窗口 */
lv_obj_t *lv_about_cont;             /*关于界面主窗口 */
lv_obj_t *lv_setting_client_cont;   /* 窗口子容器 */
lv_obj_t *lv_setting_client_cont_about;
lv_obj_t *lv_setting_win_header;    /* 窗口标题 */

/**
 * 返回到主页面
 */
static void lv_setting_event_handler(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);

    if(code == LV_EVENT_CLICKED)    /*点击 */
    {
        lv_obj_del(lv_setting_cont);
        lv_run_main();  //回归主界面
    }
}

/**
 * 返回到设置页面
 */
static void lv_about_btn_event_handler(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);

    if(code == LV_EVENT_CLICKED)    /*点击 */
    {
        lv_obj_del(lv_about_cont);                                    /* 删除窗口 */
        lv_setting_demo();
    }
}

/**
 * 返回到设置中的关于页面
 */
static void  lv_about_event_handler(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);

    if(code == LV_EVENT_CLICKED)
    {
        lv_obj_del(lv_setting_cont);                                    /* 删除窗口 */
        lv_setting_about();
    }
}




void lv_setting_demo(void)
{
    /* 创建窗口 */
    lv_setting_cont = lv_win_create(lv_scr_act(),80);
    lv_obj_set_size(lv_setting_cont,lv_obj_get_width(lv_scr_act()),lv_obj_get_height(lv_scr_act()));

    /* 添加关闭按钮 */
    lv_obj_t *close_btn = lv_win_add_btn(lv_setting_cont, LV_SYMBOL_LEFT, 50);
    lv_obj_set_style_text_font(close_btn,&lv_font_montserrat_24,0);
    lv_obj_add_event_cb(close_btn, lv_setting_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_text_color(close_btn, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(close_btn, 0, LV_PART_MAIN);                                            /* 去除背景 */
    lv_obj_set_style_shadow_opa(close_btn, 0, 0);
    lv_obj_align(close_btn,LV_ALIGN_RIGHT_MID,0,0);                                                /* 去除阴影 */

    /* 设置标题 */
    lv_obj_t *title = lv_win_add_title(lv_setting_cont, "设置");                                    /* 设置文本 */
    lv_obj_set_style_text_font(title, &setting_font, LV_PART_MAIN);                                       /* 设置字体 */

    /* 标题右侧占位字符，为了优化布局 */
    lv_obj_t *null_btn = lv_win_add_btn(lv_setting_cont, " ", 50);
    lv_obj_set_style_bg_opa(null_btn, 0, 0);
    lv_obj_set_style_shadow_opa(null_btn, 0, 0);

    /* 获取标题 */
    lv_setting_win_header = lv_win_get_header(lv_setting_cont);
    lv_obj_set_style_bg_color(lv_setting_win_header,lv_color_hex(0xe9e9e9),LV_PART_MAIN);           /* 设置该区域的颜色为白色 */

    /* 获取主体容器 */
    lv_setting_client_cont = lv_win_get_content(lv_setting_cont);
    lv_obj_set_scrollbar_mode(lv_setting_client_cont, LV_SCROLLBAR_MODE_OFF);                       /* 去除滚动条 */
    lv_obj_set_style_bg_color(lv_setting_client_cont,lv_color_hex(0xe9e9e9),LV_PART_MAIN);          /* 设置该区域的颜色为白色 */
    lv_obj_set_style_pad_left(lv_setting_client_cont, 0, LV_STATE_DEFAULT);                         /* 去除左侧填充 */
    lv_obj_set_style_pad_right(lv_setting_client_cont, 0, LV_STATE_DEFAULT);                        /* 去除右侧填充 */

    /******************************* 子容器1 ********************************/
    lv_obj_t *obj_cont_1 = lv_obj_create(lv_setting_client_cont);
    lv_obj_set_size(obj_cont_1, 750, 500);
    lv_obj_set_style_bg_color(obj_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN);
   lv_obj_align(obj_cont_1, LV_ALIGN_TOP_MID, 0, -10);
    lv_obj_set_style_border_opa(obj_cont_1, 0, 0);
    lv_obj_set_style_radius(obj_cont_1, 30, 0);

  /******************************* WIFI ********************************/
    /* WIFI图标背景 */
    lv_obj_t *obj_wlan = lv_obj_create(obj_cont_1);                                                 /* 创建图标背景 */
    lv_obj_set_size(obj_wlan, 75, 75);                                                              /* 去除阴影 */
    lv_obj_align(obj_wlan, LV_ALIGN_TOP_LEFT, 0, 0);                                                /* 设置位置 */
    lv_obj_set_style_bg_color(obj_wlan, lv_color_hex(0xf8a01e), 0);                                 /* 设置背景颜色 */
    lv_obj_set_style_radius(obj_wlan, 50, 0);                                                       /* 设置圆角 */
    lv_obj_set_style_border_opa(obj_wlan, 0, 0);                                                    /* 去除边框 */
    lv_obj_set_style_shadow_opa(obj_wlan, 0, 0);  /* 去除阴影 */
    /* WIFI图标 */
    lv_obj_t *label_wlan = lv_label_create(obj_wlan);                                               /* 创建图标 */
    lv_obj_align(label_wlan, LV_ALIGN_CENTER, 0, 0);                                                /* 设置位置 */
    lv_label_set_text(label_wlan, LV_SYMBOL_WIFI);                                                  /* 设置图标字体 */
    lv_obj_set_style_text_font(label_wlan, &lv_font_montserrat_24, LV_PART_MAIN);                   /* 设置字体大小 */
    lv_obj_set_style_text_color(label_wlan, lv_color_hex(0xffffff), LV_PART_MAIN);                  /* 设置字体颜色 */
    /* WIFI按钮 */
    lv_obj_t *btn_wlan = lv_obj_create(obj_cont_1);                                                 /* WIFI按钮背景 */
    lv_obj_set_size(btn_wlan,600, 80);                                                             /* 设置大小 */
    lv_obj_align_to(btn_wlan,label_wlan, LV_ALIGN_OUT_RIGHT_MID, 30, 0);                                                /* 设置位置 */
    lv_obj_set_style_radius(btn_wlan, 0, 0);                                                        /* 设置圆角 */
    lv_obj_set_style_border_side(btn_wlan, LV_BORDER_SIDE_BOTTOM, 0);                               /* 单边框模式：显示下边框 */
    lv_obj_set_style_bg_color(btn_wlan, lv_color_hex(0xe9e9e9), LV_STATE_PRESSED);                  /* 设置背景颜色 */
    lv_obj_set_style_border_width(btn_wlan, 1, 0);                                                  /* 设置边框宽度 */
    /* WIFI按钮文本 */
    lv_obj_t *text_wlan = lv_label_create(btn_wlan);                                                /* 创建文本 */
    lv_label_set_text(text_wlan, "WLAN");                                                           /* 设置文本 */
    lv_obj_set_style_text_font(text_wlan, &lv_font_montserrat_24, LV_PART_MAIN);                    /* 设置字体大小 */
    lv_obj_align(text_wlan, LV_ALIGN_LEFT_MID, 0, 0);                                               /* 设置位置 */

   /******************************* 蓝牙 ********************************/
    /* 蓝牙图标背景 */
    lv_obj_t *obj_ble = lv_obj_create(obj_cont_1);
    lv_obj_set_size(obj_ble, 75, 75);
    lv_obj_align_to(obj_ble, obj_wlan, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_bg_color(obj_ble, lv_color_hex(0x46b1e2), 0);
    lv_obj_set_style_radius(obj_ble, 50, 0);
    lv_obj_set_style_border_opa(obj_ble, 0, 0);
    lv_obj_set_style_shadow_opa(obj_ble, 0, 0);
    /* 蓝牙图标 */
    lv_obj_t *label_ble = lv_label_create(obj_ble);
    lv_obj_align(label_ble, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label_ble, LV_SYMBOL_BLUETOOTH);
    lv_obj_set_style_text_font(label_ble, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_ble, lv_color_hex(0xffffff), LV_PART_MAIN);
    /* 蓝牙按钮 */
    lv_obj_t *btn_ble = lv_obj_create(obj_cont_1);
    lv_obj_set_size(btn_ble, 600, 80);
    lv_obj_align_to(btn_ble, btn_wlan, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_radius(btn_ble, 0, 0);
    lv_obj_set_style_border_side(btn_ble, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_bg_color(btn_ble, lv_color_hex(0xe9e9e9), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btn_ble, 1, 0);
    /* 蓝牙按钮文本 */
    lv_obj_t *text_ble = lv_label_create(btn_ble);
    lv_label_set_text(text_ble, "蓝牙");
    lv_obj_set_style_text_font(text_ble, &setting_font1, LV_PART_MAIN);
    lv_obj_align(text_ble, LV_ALIGN_LEFT_MID, 0, 0);

    /******************************* 连接 ********************************/
    /* 连接图标背景 */
    lv_obj_t *obj_link = lv_obj_create(obj_cont_1);
    lv_obj_set_size(obj_link, 75, 75);
    lv_obj_align_to(obj_link, obj_ble, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_bg_color(obj_link, lv_color_hex(0x65b859), 0);
    lv_obj_set_style_radius(obj_link, 50, 0);
    lv_obj_set_style_border_opa(obj_link, 0, 0);
    lv_obj_set_style_shadow_opa(obj_link, 0, 0);
    /* 连接图标 */
    lv_obj_t *label_link = lv_label_create(obj_link);
    lv_obj_align(label_link, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label_link, LV_SYMBOL_LOOP);
    lv_obj_set_style_text_font(label_link, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_link, lv_color_hex(0xffffff), LV_PART_MAIN);
    /* 连接按钮 */
    lv_obj_t *btn_link = lv_obj_create(obj_cont_1);
    lv_obj_set_size(btn_link, 600, 80);
    lv_obj_align_to(btn_link, btn_ble, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_radius(btn_link, 0, 0);
    lv_obj_set_style_border_side(btn_link, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_bg_color(btn_link, lv_color_hex(0xe9e9e9), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btn_link, 1, 0);
    /* 连接按钮文本 */
    lv_obj_t *text_link = lv_label_create(btn_link);
    lv_label_set_text(text_link, "移动网络");
    lv_obj_set_style_text_font(text_link, &setting_font1, LV_PART_MAIN);
    lv_obj_align(text_link, LV_ALIGN_LEFT_MID, 0, 0);
   /******************************* 壁纸 ********************************/
    /* 壁纸图标背景 */
    lv_obj_t *obj_img = lv_obj_create(obj_cont_1);
    lv_obj_set_size(obj_img, 75, 75);
    lv_obj_align_to(obj_img, obj_link, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_style_bg_color(obj_img, lv_color_hex(0x3CB371), 0);
    lv_obj_set_style_radius(obj_img, 50, 0);
    lv_obj_set_style_border_opa(obj_img, 0, 0);
    lv_obj_set_style_shadow_opa(obj_img, 0, 0);
    /* 壁纸图标 */
    lv_obj_t *label_img = lv_label_create(obj_img);
    lv_obj_align(label_img, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label_img, LV_SYMBOL_IMAGE);
    lv_obj_set_style_text_font(label_img, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_img, lv_color_hex(0xffffff), LV_PART_MAIN);
    /* 壁纸按钮 */
    lv_obj_t *btn_img = lv_obj_create(obj_cont_1);
    lv_obj_set_size(btn_img, 600, 80);
    lv_obj_align_to(btn_img, btn_link, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_radius(btn_img, 0, 0);
    lv_obj_set_style_border_side(btn_img, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_bg_color(btn_img, lv_color_hex(0xe9e9e9), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btn_img, 1, 0);
    /* 壁纸按钮文本 */
    lv_obj_t *text_img = lv_label_create(btn_img);
    lv_label_set_text(text_img, "桌面和个性化");
    lv_obj_set_style_text_font(text_img, &setting_font1, LV_PART_MAIN);
    lv_obj_align(text_img, LV_ALIGN_LEFT_MID, 0, 0);
  /******************************* 显示和亮度 ********************************/
    /* 显示和亮度图标背景 */
    lv_obj_t *obj_display = lv_obj_create(obj_cont_1);
    lv_obj_set_size(obj_display, 75, 75);
    lv_obj_align_to(obj_display, obj_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_bg_color(obj_display, lv_color_hex(0x00FA9A), 0);
    lv_obj_set_style_radius(obj_display, 50, 0);
    lv_obj_set_style_border_opa(obj_display, 0, 0);
    lv_obj_set_style_shadow_opa(obj_display, 0, 0);
    /* 显示和亮度图标 */
    lv_obj_t *label_display = lv_label_create(obj_display);
    lv_obj_align(label_display, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label_display, LV_SYMBOL_EYE_OPEN);
    lv_obj_set_style_text_font(label_display, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_display, lv_color_hex(0xffffff), LV_PART_MAIN);
    /* 显示和亮度按钮 */
    lv_obj_t *btn_display = lv_obj_create(obj_cont_1);
    lv_obj_set_size(btn_display, 600, 80);
    lv_obj_align_to(btn_display, btn_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_radius(btn_display, 0, 0);
    lv_obj_set_style_border_side(btn_display, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_bg_color(btn_display, lv_color_hex(0xe9e9e9), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btn_display, 1, 0);
    /* 显示和亮度按钮文本 */
    lv_obj_t *text_display = lv_label_create(btn_display);
    lv_label_set_text(text_display, "显示和亮度");
    lv_obj_set_style_text_font(text_display, &setting_font1, LV_PART_MAIN);
    lv_obj_align(text_display, LV_ALIGN_LEFT_MID, 0, 0);


    /******************************* 子容器2 ********************************/
    lv_obj_t *obj_cont_2 = lv_obj_create(lv_setting_client_cont);
    lv_obj_set_size(obj_cont_2, 750, 220);
    lv_obj_set_style_bg_color(obj_cont_2, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(obj_cont_2, obj_cont_1, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);
    lv_obj_set_style_border_opa(obj_cont_2, 0, 0);
    lv_obj_set_style_radius(obj_cont_2, 30, 0);
    /******************************* 声音 ********************************/
    /* 声音图标背景 */
    lv_obj_t *obj_sound = lv_obj_create(obj_cont_2);
    lv_obj_set_size(obj_sound, 75, 75);
    lv_obj_align(obj_sound, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_color(obj_sound, lv_color_hex(0x0000CD), 0);
    lv_obj_set_style_radius(obj_sound, 50, 0);
    lv_obj_set_style_border_opa(obj_sound, 0, 0);
    lv_obj_set_style_shadow_opa(obj_sound, 0, 0);
    /* 声音图标 */
    lv_obj_t *label_sound = lv_label_create(obj_sound);
    lv_obj_align(label_sound, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label_sound, LV_SYMBOL_VOLUME_MAX);
    lv_obj_set_style_text_font(label_sound, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_sound, lv_color_hex(0xffffff), LV_PART_MAIN);
    /* 声音按钮 */
    lv_obj_t *btn_sound = lv_obj_create(obj_cont_2);
    lv_obj_set_size(btn_sound, 600, 80);
    lv_obj_align(btn_sound, LV_ALIGN_TOP_MID, 25, 0);
    lv_obj_set_style_radius(btn_sound, 0, 0);
    lv_obj_set_style_border_side(btn_sound, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_bg_color(btn_sound, lv_color_hex(0xe9e9e9), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btn_sound, 1, 0);
    /* 声音按钮文本 */
    lv_obj_t *text_sound = lv_label_create(btn_sound);
    lv_label_set_text(text_sound, "声音和振动");
    lv_obj_set_style_text_font(text_sound, &setting_font1, LV_PART_MAIN);
    lv_obj_align(text_sound, LV_ALIGN_LEFT_MID, 0, 0);
    /******************************* 通知 ********************************/
    /* 通知图标背景 */
    lv_obj_t *obj_inform = lv_obj_create(obj_cont_2);
    lv_obj_set_size(obj_inform, 75, 75);
    lv_obj_align_to(obj_inform, obj_sound, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_bg_color(obj_inform, lv_color_hex(0xf8a01e), 0);
    lv_obj_set_style_radius(obj_inform, 50, 0);
    lv_obj_set_style_border_opa(obj_inform, 0, 0);
    lv_obj_set_style_shadow_opa(obj_inform, 0, 0);
    /* 通知图标 */
    lv_obj_t *label_inform = lv_label_create(obj_inform);
    lv_obj_align(label_inform, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label_inform, LV_SYMBOL_BELL);
    lv_obj_set_style_text_font(label_inform, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_inform, lv_color_hex(0xffffff), LV_PART_MAIN);
    /* 通知按钮 */
    lv_obj_t *btn_inform = lv_obj_create(obj_cont_2);
    lv_obj_set_size(btn_inform, 600, 80);
    lv_obj_align_to(btn_inform, btn_sound, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_radius(btn_inform, 0, 0);
    lv_obj_set_style_border_side(btn_inform, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_bg_color(btn_inform, lv_color_hex(0xe9e9e9), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btn_inform, 1, 0);
    /* 通知按钮文本 */
    lv_obj_t *text_inform = lv_label_create(btn_inform);
    lv_label_set_text(text_inform, "通知和状态栏");
    lv_obj_set_style_text_font(text_inform, &setting_font1, LV_PART_MAIN);
    lv_obj_align(text_inform, LV_ALIGN_LEFT_MID, 0, 0);


    /******************************* 子容器3 ********************************/
    lv_obj_t *obj_cont_3 = lv_obj_create(lv_setting_client_cont);
    lv_obj_set_size(obj_cont_3, 750, 680);
    lv_obj_set_style_bg_color(obj_cont_3, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(obj_cont_3, obj_cont_2, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);
    lv_obj_set_style_border_opa(obj_cont_3, 0, 0);
    lv_obj_set_style_radius(obj_cont_3, 30, 0);
    /******************************* 存储设置 ********************************/
    /* 存储设置图标背景 */
    lv_obj_t *obj_memory = lv_obj_create(obj_cont_3);
    lv_obj_set_size(obj_memory, 75, 75);
    lv_obj_align(obj_memory, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_color(obj_memory, lv_color_hex(0x0000FF), 0);
    lv_obj_set_style_radius(obj_memory, 50, 0);
    lv_obj_set_style_border_opa(obj_memory, 0, 0);
    lv_obj_set_style_shadow_opa(obj_memory, 0, 0);
    /* 存储设置图标 */
    lv_obj_t *label_memory = lv_label_create(obj_memory);
    lv_obj_align(label_memory, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label_memory, LV_SYMBOL_DRIVE);
    lv_obj_set_style_text_font(label_memory, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_memory, lv_color_hex(0xffffff), LV_PART_MAIN);
    /* 存储设置按钮 */
    lv_obj_t *btn_memory = lv_obj_create(obj_cont_3);
    lv_obj_set_size(btn_memory, 600, 80);
    lv_obj_align(btn_memory, LV_ALIGN_TOP_MID, 25, 0);
    lv_obj_set_style_radius(btn_memory, 0, 0);
    lv_obj_set_style_border_side(btn_memory, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_bg_color(btn_memory, lv_color_hex(0xe9e9e9), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btn_memory, 1, 0);
    /* 存储设置按钮文本 */
    lv_obj_t *text_memory = lv_label_create(btn_memory);
    lv_label_set_text(text_memory, "存储");
    lv_obj_set_style_text_font(text_memory, &setting_font1, LV_PART_MAIN);
    lv_obj_align(text_memory, LV_ALIGN_LEFT_MID, 0, 0);
   /******************************* 电池 ********************************/
    /* 电池图标背景 */
    lv_obj_t *obj_battery = lv_obj_create(obj_cont_3);
    lv_obj_set_size(obj_battery, 75, 75);
    lv_obj_align_to(obj_battery, obj_memory, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_style_bg_color(obj_battery, lv_color_hex(0x00FA9A), 0);
    lv_obj_set_style_radius(obj_battery, 50, 0);
    lv_obj_set_style_border_opa(obj_battery, 0, 0);
    lv_obj_set_style_shadow_opa(obj_battery, 0, 0);
    /* 电池图标 */
    lv_obj_t *label_battery = lv_label_create(obj_battery);
    lv_obj_align(label_battery, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label_battery, LV_SYMBOL_BATTERY_FULL);
    lv_obj_set_style_text_font(label_battery, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_battery, lv_color_hex(0xffffff), LV_PART_MAIN);
    /* 电池按钮 */
    lv_obj_t *btn_battery = lv_obj_create(obj_cont_3);
    lv_obj_set_size(btn_battery, 600, 80);
    lv_obj_align_to(btn_battery, btn_memory, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_radius(btn_battery, 0, 0);
    lv_obj_set_style_border_side(btn_battery, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_bg_color(btn_battery, lv_color_hex(0xe9e9e9), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btn_battery, 1, 0);
    /* 电池按钮文本 */
    lv_obj_t *text_battery = lv_label_create(btn_battery);
    lv_label_set_text(text_battery, "电池");
    lv_obj_set_style_text_font(text_battery, &setting_font1, LV_PART_MAIN);
    lv_obj_align(text_battery, LV_ALIGN_LEFT_MID, 0, 0);
    /******************************* 应用 ********************************/

    /* 应用设置图标背景 */
    lv_obj_t *obj_app = lv_obj_create(obj_cont_3);
    lv_obj_set_size(obj_app, 75, 75);
   lv_obj_align_to(obj_app, obj_battery, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_style_bg_color(obj_app, lv_color_hex(0xf8a01e), 0);
    lv_obj_set_style_radius(obj_app, 50, 0);
    lv_obj_set_style_border_opa(obj_app, 0, 0);
    lv_obj_set_style_shadow_opa(obj_app, 0, 0);
    /* 应用设置图标 */
    lv_obj_t *label_app = lv_label_create(obj_app);
    lv_obj_align(label_app, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label_app, "\uF009");
    lv_obj_set_style_text_font(label_app, &setting_img, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_app, lv_color_hex(0xffffff), LV_PART_MAIN);
    /* 应用按钮 */
    lv_obj_t *btn_app = lv_obj_create(obj_cont_3);
    lv_obj_set_size(btn_app, 600, 80);
    lv_obj_align_to(btn_app, btn_battery, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_radius(btn_app, 0, 0);
    lv_obj_set_style_border_side(btn_app, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_bg_color(btn_app, lv_color_hex(0xe9e9e9), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btn_app, 1, 0);
    /* 应用按钮文本 */
    lv_obj_t *text_app = lv_label_create(btn_app);
    lv_label_set_text(text_app, "应用和服务");
    lv_obj_set_style_text_font(text_app, &setting_font1, LV_PART_MAIN);
    lv_obj_align(text_app, LV_ALIGN_LEFT_MID, 0, 0);
    /******************************* 用户 ********************************/

    /* 用户设置图标背景 */
    lv_obj_t *obj_user = lv_obj_create(obj_cont_3);
    lv_obj_set_size(obj_user, 75, 75);
    lv_obj_align_to(obj_user, obj_app, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_bg_color(obj_user, lv_color_hex(0xFF4500), 0);
    lv_obj_set_style_radius(obj_user, 50, 0);
    lv_obj_set_style_border_opa(obj_user, 0, 0);
    lv_obj_set_style_shadow_opa(obj_user, 0, 0);
    /* 用户设置图标 */
    lv_obj_t *label_user = lv_label_create(obj_user);
    lv_obj_align(label_user, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label_user, "\uF007");
    lv_obj_set_style_text_font(label_user, &setting_img, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_user, lv_color_hex(0xffffff), LV_PART_MAIN);
    /* 用户按钮 */
    lv_obj_t *btn_user = lv_obj_create(obj_cont_3);
    lv_obj_set_size(btn_user, 600, 80);
    lv_obj_align_to(btn_user, btn_app, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_radius(btn_user, 0, 0);
    lv_obj_set_style_border_side(btn_user, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_bg_color(btn_user, lv_color_hex(0xe9e9e9), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btn_user, 1, 0);
    /* 用户按钮文本 */
    lv_obj_t *text_user = lv_label_create(btn_user);
    lv_label_set_text(text_user, "用户和账户");
    lv_obj_set_style_text_font(text_user, &setting_font1, LV_PART_MAIN);
    lv_obj_align(text_user, LV_ALIGN_LEFT_MID, 0, 0);
    /******************************* 安全********************************/
    /* 安全设置图标背景 */
    lv_obj_t *obj_safety = lv_obj_create(obj_cont_3);
    lv_obj_set_size(obj_safety, 75, 75);
   lv_obj_align_to(obj_safety, obj_user, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_bg_color(obj_safety, lv_color_hex(0x00FA9A), 0);
    lv_obj_set_style_radius(obj_safety, 50, 0);
    lv_obj_set_style_border_opa(obj_safety, 0, 0);
    lv_obj_set_style_shadow_opa(obj_safety, 0, 0);
    /* 安全设置图标 */
    lv_obj_t *label_safety = lv_label_create(obj_safety);
    lv_obj_align(label_safety, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label_safety, "\uF265");
    lv_obj_set_style_text_font(label_safety, &setting_img, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_safety, lv_color_hex(0xffffff), LV_PART_MAIN);
    /* 安全按钮 */
    lv_obj_t *btn_safety  = lv_obj_create(obj_cont_3);
    lv_obj_set_size(btn_safety, 600, 80);
    lv_obj_align_to(btn_safety, btn_user, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_radius(btn_safety, 0, 0);
    lv_obj_set_style_border_side(btn_safety, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_bg_color(btn_safety, lv_color_hex(0xe9e9e9), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btn_safety, 1, 0);
    /* 安全按钮文本 */
    lv_obj_t *text_safety = lv_label_create(btn_safety);
    lv_label_set_text(text_safety, "安全");
    lv_obj_set_style_text_font(text_safety, &setting_font1, LV_PART_MAIN);
    lv_obj_align(text_safety, LV_ALIGN_LEFT_MID, 0, 0);
    /******************************* 系统和更新********************************/
    /* 系统和更新设置图标背景 */
    lv_obj_t *obj_set = lv_obj_create(obj_cont_3);
    lv_obj_set_size(obj_set, 75, 75);
   lv_obj_align_to(obj_set, obj_safety, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_bg_color(obj_set, lv_color_hex(0x0000FF), 0);
    lv_obj_set_style_radius(obj_set, 50, 0);
    lv_obj_set_style_border_opa(obj_set, 0, 0);
    lv_obj_set_style_shadow_opa(obj_set, 0, 0);
    /* 系统和更新设置图标 */
    lv_obj_t *label_set = lv_label_create(obj_set);
    lv_obj_align(label_set, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label_set, "\uF013");
    lv_obj_set_style_text_font(label_set, &setting_img, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_set, lv_color_hex(0xffffff), LV_PART_MAIN);
    /* 系统和更新按钮 */
    lv_obj_t *btn_set  = lv_obj_create(obj_cont_3);
    lv_obj_set_size(btn_set, 600, 80);
    lv_obj_align_to(btn_set, btn_safety, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_radius(btn_set, 0, 0);
    lv_obj_set_style_border_side(btn_set, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_bg_color(btn_set, lv_color_hex(0xe9e9e9), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btn_set, 1, 0);
    /* 系统和更新按钮文本 */
    lv_obj_t *text_set = lv_label_create(btn_set);
    lv_label_set_text(text_set, "系统和更新");
    lv_obj_set_style_text_font(text_set, &setting_font1, LV_PART_MAIN);
    lv_obj_align(text_set, LV_ALIGN_LEFT_MID, 0, 0);
    /******************************* 关于********************************/
    /* 关于图标背景 */
    lv_obj_t *obj_about = lv_obj_create(obj_cont_3);
    lv_obj_set_size(obj_about, 75, 75);
    lv_obj_align_to(obj_about, obj_set, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_style_bg_color(obj_about, lv_color_hex(0x696969), 0);
    lv_obj_set_style_radius(obj_about, 50, 0);
    lv_obj_set_style_border_opa(obj_about, 0, 0);
    lv_obj_set_style_shadow_opa(obj_about, 0, 0);
    /* 关于图标 */
    lv_obj_t *label_about = lv_label_create(obj_about);
    lv_obj_align(label_about, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label_about, "\uF129");
    lv_obj_set_style_text_font(label_about, &setting_img, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_about, lv_color_hex(0xffffff), LV_PART_MAIN);
    /* 关于按钮 */
    lv_obj_t *btn_about  = lv_obj_create(obj_cont_3);
    lv_obj_set_size(btn_about, 600, 80);
    lv_obj_align_to(btn_about, btn_set, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_radius(btn_about, 0, 0);
    lv_obj_set_style_border_side(btn_about, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_bg_color(btn_about, lv_color_hex(0xe9e9e9), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btn_about, 1, 0);
    lv_obj_add_event_cb(btn_about, lv_about_event_handler, LV_EVENT_CLICKED, NULL);
    /* 关于按钮文本 */
    lv_obj_t *text_about = lv_label_create(btn_about);
    lv_label_set_text(text_about, "关于");
    lv_obj_set_style_text_font(text_about, &setting_font1, LV_PART_MAIN);
    lv_obj_align(text_about, LV_ALIGN_LEFT_MID, 0, 0);
}


//歌曲列表偏移动画回调函数
 static void about_set_offlex(void * var, int32_t v)
{
    lv_obj_set_x(var,v);
}


static void about_anim_off(void * var)
{
    lv_anim_del(lv_about_cont,about_set_offlex);
}

void lv_setting_about(void)
{
    /* 创建窗口 */
    lv_about_cont = lv_win_create(lv_scr_act(),80);
    lv_obj_set_size(lv_about_cont,lv_obj_get_width(lv_scr_act()),lv_obj_get_height(lv_scr_act()));
    /* 添加关闭按钮 */
    lv_obj_t *close_btn = lv_win_add_btn(lv_about_cont, LV_SYMBOL_LEFT, 50);
    lv_obj_set_style_text_font(close_btn,&lv_font_montserrat_24,0);
    lv_obj_add_event_cb(close_btn, lv_about_btn_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_text_color(close_btn, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(close_btn, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(close_btn, 0, 0);
    lv_obj_align(close_btn,LV_ALIGN_RIGHT_MID,0,0);
    /* 设置标题 */
    lv_obj_t *title = lv_win_add_title(lv_about_cont, "关于");
    lv_obj_set_style_text_font(title, &setting_font, LV_PART_MAIN);
    /* 标题右侧占位字符，为了优化布局 */
    lv_obj_t *null_btn = lv_win_add_btn(lv_about_cont, " ", 50);
    lv_obj_set_style_bg_opa(null_btn, 0, 0);
    lv_obj_set_style_shadow_opa(null_btn, 0, 0);
    /* 获取标题 */
    lv_setting_win_header = lv_win_get_header(lv_about_cont);
    lv_obj_set_style_bg_color(lv_setting_win_header,lv_color_hex(0xe9e9e9),LV_PART_MAIN);           /* 设置该区域的颜色为白色 */
    /* 获取主体容器 */
    lv_setting_client_cont_about = lv_win_get_content(lv_about_cont);
    lv_obj_set_scrollbar_mode(lv_setting_client_cont_about, LV_SCROLLBAR_MODE_OFF);                       /* 去除滚动条 */
    lv_obj_set_style_bg_color(lv_setting_client_cont_about,lv_color_hex(0xe9e9e9),LV_PART_MAIN);          /* 设置该区域的颜色为白色 */
    lv_obj_set_style_pad_left(lv_setting_client_cont_about, 0, LV_STATE_DEFAULT);                         /* 去除左侧填充 */
    lv_obj_set_style_pad_right(lv_setting_client_cont_about, 0, LV_STATE_DEFAULT);
    /*关于的图片和文字介绍*/
    lv_obj_t *obj_about = lv_obj_create(lv_setting_client_cont_about);
    lv_obj_set_size(obj_about, 750, 350);
    lv_obj_set_style_bg_color(obj_about, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(obj_about, LV_ALIGN_TOP_MID, 0, -10);
    lv_obj_set_style_border_opa(obj_about, 0, 0);
    lv_obj_set_style_radius(obj_about, 30, 0);
    lv_obj_set_style_bg_opa(obj_about,0,0);

    lv_obj_t * img_about = lv_img_create(obj_about);
    lv_img_set_src(img_about,&setting_about);
    lv_obj_align(img_about,LV_ALIGN_CENTER,0,0);
    lv_obj_set_style_radius(img_about,50,0);


    lv_obj_t *label_about = lv_label_create(img_about);
    lv_label_set_text(label_about,"LVGL");
    lv_obj_set_style_text_font(label_about,&setting_font_about,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_about,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_center(label_about);

    /**** 下面为其他事宜介绍***********************************************/
    /*容器1*/
    lv_obj_t *obj_cont1 = lv_obj_create(lv_setting_client_cont_about);
    lv_obj_set_size(obj_cont1, 700, 110);
    lv_obj_set_style_bg_color(obj_cont1, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(obj_cont1, obj_about, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);
    lv_obj_set_style_border_opa(obj_cont1, 0, 0);
    lv_obj_set_style_radius(obj_cont1, 30, 0);

    /*设备名称*/
    lv_obj_t *label_device_name = lv_label_create(obj_cont1);
    lv_label_set_text(label_device_name,"设备名称");
    lv_obj_set_style_text_font(label_device_name,&setting_font_about1,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_device_name,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align(label_device_name,LV_ALIGN_LEFT_MID,0,0);


    /*设备名称框*/
    lv_obj_t *  obj_device_name = lv_obj_create(obj_cont1);
    lv_obj_set_size(obj_device_name, 300, 65);
    lv_obj_set_style_bg_color(obj_device_name, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(obj_device_name,LV_ALIGN_RIGHT_MID,-10,0);
    lv_obj_set_style_border_opa(obj_device_name,0,0);
    /*设备名称改名字*/
    lv_obj_t *label_device_xname = lv_label_create(obj_device_name);
    lv_label_set_text(label_device_xname,"RK3568-Dev");
    lv_obj_set_style_text_font(label_device_xname,&lv_font_montserrat_20,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_device_xname,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align(label_device_xname,LV_ALIGN_RIGHT_MID,0,0);


    /*设备改名按钮*/
    lv_obj_t * btn_device_name = lv_btn_create(obj_cont1);
    lv_obj_set_size(btn_device_name, 650, 60);
    lv_obj_set_style_bg_opa(btn_device_name,0,0);
    lv_obj_align(btn_device_name, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_border_opa(btn_device_name,0,0);
    lv_obj_set_style_shadow_opa(btn_device_name,0,0);
    /*设备名称改名字图标按钮*/
    lv_obj_t *label_open = lv_label_create(obj_cont1);
    lv_label_set_text(label_open, LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_font(label_open, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_open, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_align(label_open, LV_ALIGN_RIGHT_MID, 0, 0);

    /***********容器2*************************/
    lv_obj_t *obj_cont2 = lv_obj_create(lv_setting_client_cont_about);
    lv_obj_set_size(obj_cont2, 700, 660);
    lv_obj_set_style_bg_color(obj_cont2, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(obj_cont2, obj_cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);
    lv_obj_set_style_border_opa(obj_cont2, 0, 0);
    lv_obj_set_style_radius(obj_cont2, 30, 0);


    /*芯片型号*/
    lv_obj_t *label_chip_name = lv_label_create(obj_cont2);
    lv_label_set_text(label_chip_name,"芯片型号");
    lv_obj_set_style_text_font(label_chip_name,&setting_font_about1,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_chip_name,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align(label_chip_name, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t *label_chip_data = lv_label_create(obj_cont2);
    lv_label_set_text(label_chip_data,"RK3568");
    lv_obj_set_style_text_font(label_chip_data,&lv_font_montserrat_20,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_chip_data,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_chip_data,label_chip_name, LV_ALIGN_OUT_RIGHT_MID, 350, 0);

    lv_obj_t * line = lv_line_create(obj_cont2);
    lv_line_set_points(line,line_points,2);
    lv_obj_align_to(line,label_chip_name,LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    lv_obj_set_style_line_color(line,lv_color_hex(0x696969),0);
    lv_obj_set_style_line_opa(line,50,0);


    /*LVGL版本*/
    lv_obj_t *label_lvgl_ver = lv_label_create(obj_cont2);
    lv_label_set_text(label_lvgl_ver,"LVGL版本");
    lv_obj_set_style_text_font(label_lvgl_ver,&setting_font_about1,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_lvgl_ver,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_lvgl_ver, label_chip_name, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);

    lv_obj_t *label_lvgl_data = lv_label_create(obj_cont2);
    lv_label_set_text(label_lvgl_data,"V8.3.0");
    lv_obj_set_style_text_font(label_lvgl_data,&lv_font_montserrat_20,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_lvgl_data,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_lvgl_data, label_chip_data, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 40);

    lv_obj_t * line1 = lv_line_create(obj_cont2);
    lv_line_set_points(line1,line_points,2);
    lv_obj_align_to(line1,label_lvgl_ver,LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    lv_obj_set_style_line_color(line1,lv_color_hex(0x696969),0);
    lv_obj_set_style_line_opa(line1,50,0);
    /*软件版本*/
    lv_obj_t *label_software_ver = lv_label_create(obj_cont2);
    lv_label_set_text(label_software_ver,"软件版本");
    lv_obj_set_style_text_font(label_software_ver,&setting_font_about1,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_software_ver,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_software_ver, label_lvgl_ver, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);

    lv_obj_t *label_software_data = lv_label_create(obj_cont2);
    lv_label_set_text(label_software_data,"V1.0");
    lv_obj_set_style_text_font(label_software_data,&lv_font_montserrat_20,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_software_data,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_software_data, label_lvgl_ver, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 40);

    lv_obj_t * line3 = lv_line_create(obj_cont2);
    lv_line_set_points(line3,line_points,2);
    lv_obj_align_to(line3,label_software_ver,LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    lv_obj_set_style_line_color(line3,lv_color_hex(0x696969),0);
    lv_obj_set_style_line_opa(line3,50,0);
//    /*内核*/
    lv_obj_t *label_kernel_name = lv_label_create(obj_cont2);
    lv_label_set_text(label_kernel_name,"内核");
    lv_obj_set_style_text_font(label_kernel_name,&setting_font_about1,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_kernel_name,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_kernel_name, label_software_ver, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);

    lv_obj_t *label_kernel_data = lv_label_create(obj_cont2);
    lv_label_set_text(label_kernel_data,"Cortex M4");
    lv_obj_set_style_text_font(label_kernel_data,&lv_font_montserrat_20,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_kernel_data,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_kernel_data, label_software_data, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 40);

    lv_obj_t * line4 = lv_line_create(obj_cont2);
    lv_line_set_points(line4,line_points,2);
    lv_obj_align_to(line4,label_kernel_name,LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    lv_obj_set_style_line_color(line4,lv_color_hex(0x696969),0);
    lv_obj_set_style_line_opa(line4,50,0);
    /*运行内存*/
    lv_obj_t *label_runmemory_name = lv_label_create(obj_cont2);
    lv_label_set_text(label_runmemory_name,"运行内存");
    lv_obj_set_style_text_font(label_runmemory_name,&setting_font_about1,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_runmemory_name,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_runmemory_name, label_kernel_name, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);

    lv_obj_t *label_runmemory_data = lv_label_create(obj_cont2);
    lv_label_set_text(label_runmemory_data,"xxxKB");
    lv_obj_set_style_text_font(label_runmemory_data,&lv_font_montserrat_20,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_runmemory_data,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_runmemory_data, label_kernel_data, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 40);


    lv_obj_t * line5 = lv_line_create(obj_cont2);
    lv_line_set_points(line5,line_points,2);
    lv_obj_align_to(line5,label_runmemory_name,LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    lv_obj_set_style_line_color(line5,lv_color_hex(0x696969),0);
    lv_obj_set_style_line_opa(line5,50,0);
     /*SDRAM*/
    lv_obj_t *label_sdram_name = lv_label_create(obj_cont2);
    lv_label_set_text(label_sdram_name,"SDRAM");
    lv_obj_set_style_text_font(label_sdram_name,&setting_font_about1,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_sdram_name,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_sdram_name, label_runmemory_name, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);

    lv_obj_t *label_sdram_data = lv_label_create(obj_cont2);
    lv_label_set_text(label_sdram_data,"xxxKB");
    lv_obj_set_style_text_font(label_sdram_data,&lv_font_montserrat_20,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_sdram_data,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_sdram_data, label_runmemory_data, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 40);

    lv_obj_t * line6 = lv_line_create(obj_cont2);
    lv_line_set_points(line6,line_points,2);
    lv_obj_align_to(line6,label_sdram_name,LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    lv_obj_set_style_line_color(line6,lv_color_hex(0x696969),0);
    lv_obj_set_style_line_opa(line6,50,0);
    /*SD卡存储*/
    lv_obj_t *label_sdcard_name = lv_label_create(obj_cont2);
    lv_label_set_text(label_sdcard_name,"SD卡存储");
    lv_obj_set_style_text_font(label_sdcard_name,&setting_font_about1,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_sdcard_name,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_sdcard_name, label_sdram_name, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);

    lv_obj_t *label_sdcard_data = lv_label_create(obj_cont2);
    lv_label_set_text(label_sdcard_data,"xxxxxMB");
    lv_obj_set_style_text_font(label_sdcard_data,&lv_font_montserrat_20,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_sdcard_data,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_sdcard_data, label_sdram_data, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 40);

    lv_obj_t * line7 = lv_line_create(obj_cont2);
    lv_line_set_points(line7,line_points,2);
    lv_obj_align_to(line7,label_sdcard_name,LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    lv_obj_set_style_line_color(line7,lv_color_hex(0x696969),0);
    lv_obj_set_style_line_opa(line7,50,0);
    /*norflash*/
    lv_obj_t *label_norfalsh_name = lv_label_create(obj_cont2);
    lv_label_set_text(label_norfalsh_name,"Nor Flash");
    lv_obj_set_style_text_font(label_norfalsh_name,&setting_font_about1,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_norfalsh_name,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_norfalsh_name, label_sdcard_name, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);

    lv_obj_t *label_norfalsh_data = lv_label_create(obj_cont2);
    lv_label_set_text(label_norfalsh_data,"xxxxxMB");
    lv_obj_set_style_text_font(label_norfalsh_data,&lv_font_montserrat_20,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_norfalsh_data,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_norfalsh_data, label_sdcard_data, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 40);

    lv_obj_t * line8 = lv_line_create(obj_cont2);
    lv_line_set_points(line8,line_points,2);
    lv_obj_align_to(line8,label_norfalsh_name,LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    lv_obj_set_style_line_color(line8,lv_color_hex(0x696969),0);
    lv_obj_set_style_line_opa(line8,50,0);
    /*屏幕尺寸*/
    lv_obj_t *label_rgblcd_name = lv_label_create(obj_cont2);
    lv_label_set_text(label_rgblcd_name,"屏幕");
    lv_obj_set_style_text_font(label_rgblcd_name,&setting_font_about1,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_rgblcd_name,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_rgblcd_name, label_norfalsh_name, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);

    lv_obj_t *label_rgblcd_data = lv_label_create(obj_cont2);
    lv_label_set_text(label_rgblcd_data,"1024x600");
    lv_obj_set_style_text_font(label_rgblcd_data,&lv_font_montserrat_20,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_rgblcd_data,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_align_to(label_rgblcd_data, label_norfalsh_data, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 40);

    lv_obj_t * line9 = lv_line_create(obj_cont2);
    lv_line_set_points(line9,line_points,2);
    lv_obj_align_to(line9,label_rgblcd_name,LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    lv_obj_set_style_line_color(line9,lv_color_hex(0x696969),0);
    lv_obj_set_style_line_opa(line9,50,0);


    lv_anim_init(&about_anim);
    lv_anim_set_var(&about_anim, lv_about_cont);
    lv_anim_set_exec_cb(&about_anim, about_set_offlex);  // 设置一个动画函数
    lv_anim_set_values(&about_anim, 800, 0);    // 设置动画的起始值和结束值
    lv_anim_set_time(&about_anim, 300);
    lv_anim_set_ready_cb(&about_anim,about_anim_off);
    lv_anim_set_repeat_count(&about_anim, 0); // 动画不重复
    lv_anim_start(&about_anim);
}





void lv_setting_demo_open(void)
{
    lv_setting_demo();
}
