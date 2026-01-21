#include "lv_run_main.h"
#include "lvgl.h"

#include "lv_boot_anim.h"
#include "lv_setting_demo.h"
#include "lv_calculator_demo.h"
#include "lv_video_demo.h"
#include "airplane/air.h"

LV_FONT_DECLARE(main_imgfont);
LV_FONT_DECLARE(main_clock);
LV_FONT_DECLARE(mytext18);
LV_IMG_DECLARE(game);

LV_IMG_DECLARE(main_bg1);
LV_IMG_DECLARE(main_photo);
LV_IMG_DECLARE(main_music);
LV_IMG_DECLARE(main_video);
LV_IMG_DECLARE(main_control);
LV_IMG_DECLARE(main_file);
LV_IMG_DECLARE(main_set);
LV_IMG_DECLARE(main_calculator);
LV_IMG_DECLARE(main_calendar);
LV_IMG_DECLARE(main_drawing_board);

static lv_obj_t *tileview;

lv_obj_t* cont;  //主界面 容器(父类)

//---------------------------------------------------------------------------------------------------------------------

// 游戏入口回调（替换原控制台功能）
static void lv_event_game_cb(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);

    if(code == LV_EVENT_CLICKED)
    {
        printf("game\n");
        // 清理当前界面
        lv_obj_del(tileview);
        // 清理顶层（如果有）
        if(lv_layer_top()) {
            lv_obj_clean(lv_layer_top());
        }
        // 初始化游戏界面
        game_init();
    }
}

// 视频入口回调（替换原音乐功能）
static void lv_event_video_cb(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);

    if(code == LV_EVENT_CLICKED)
    {
        printf("video\n");
        // 清理当前界面
        lv_obj_del(tileview);
        // 清理顶层（如果有）
        if(lv_layer_top()) {
            lv_obj_clean(lv_layer_top());
        }
        // 初始化视频界面
        video_init();
    }
}

/********************************************************************/
/*计算器*/
static void lv_event_calculator_cb(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    if(code == LV_EVENT_CLICKED)
    {
        printf("calculator\n");
        lv_obj_del(tileview);
        lv_calculator_demo_open();
    }
}

/********************************************************************/
/*设置*/
static void lv_event_setting_cb(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    if(code == LV_EVENT_CLICKED)
    {
        printf("setting\n");
        lv_obj_del(tileview);
        lv_setting_demo_open();
    }
}

//-----------------------------------------------------------------
void main_grid(void)
{
    /* 网格布局定义 - 4列3行，针对1024x600优化 */
    static lv_coord_t col_dsc[] = { 200, 160, 160, 160, LV_GRID_TEMPLATE_LAST };
    static lv_coord_t row_dsc[] = { 130, 150, 150, LV_GRID_TEMPLATE_LAST };

    /* 创建瓦片视图容器 */
    tileview = lv_tileview_create(lv_scr_act());
    lv_obj_set_style_bg_img_src(tileview, "A:img/paint.jpg", LV_PART_MAIN);
    lv_obj_set_style_bg_img_opa(tileview, 255, LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(tileview, LV_SCROLLBAR_MODE_OFF);

    /* 添加两个页面：主界面和扩展界面 */
    lv_obj_t *tile1 = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_RIGHT);
    lv_obj_t *tile2 = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_LEFT);

    /* 扩展页面占位容器 */
    lv_obj_t *title_text = lv_obj_create(tile2);
    lv_obj_set_size(title_text, 600, 300);
    lv_obj_center(title_text);
    lv_obj_set_style_bg_opa(title_text, 0, 0);
    lv_obj_set_style_border_opa(title_text, 0, 0);

    /* 主网格容器 */
    cont = lv_obj_create(tile1);
    lv_obj_set_size(cont, 720, 460);  // 700 = 200 + 160*3 + 40(间距), 460 = 130 + 150*2 + 30(间距)
    lv_obj_center(cont);
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);
    lv_obj_set_style_bg_opa(cont, 0, LV_PART_MAIN);
    lv_obj_set_style_border_opa(cont, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(cont, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_row(cont, 10, LV_PART_MAIN);     // 行间距10px
    lv_obj_set_style_pad_column(cont, 10, LV_PART_MAIN);  // 列间距10px

    /*========================================
     * 第一行：时间日期模块（跨3列）
     *========================================*/
    lv_obj_t *obj_time = lv_btn_create(cont);
    lv_obj_set_style_bg_color(obj_time, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_bg_opa(obj_time, 30, LV_PART_MAIN);
    lv_obj_set_style_border_opa(obj_time, 150, LV_PART_MAIN);
    lv_obj_set_style_border_width(obj_time, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(obj_time, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_radius(obj_time, 15, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(obj_time, 10, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(obj_time, 80, LV_PART_MAIN);
    lv_obj_set_style_shadow_color(obj_time, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_add_state(obj_time, LV_STATE_DISABLED);

    LV_FONT_DECLARE(mian_date_font);

    /* 日期显示 - 紧凑布局 */
    lv_obj_t *label_date_year0 = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_year0, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_label_set_text(label_date_year0, "20");
    lv_obj_set_style_text_color(label_date_year0, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label_date_year0, LV_ALIGN_TOP_LEFT, 12, 10);

    lv_obj_t *label_date_year = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_year, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_label_set_text(label_date_year, "24");
    lv_obj_set_style_text_color(label_date_year, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(label_date_year, label_date_year0, LV_ALIGN_OUT_RIGHT_MID, 2, 0);

    lv_obj_t *label_date_year_text = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_year_text, &mian_date_font, LV_PART_MAIN);
    lv_label_set_text(label_date_year_text, "年");
    lv_obj_set_style_text_color(label_date_year_text, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(label_date_year_text, label_date_year, LV_ALIGN_OUT_RIGHT_MID, 4, 0);

    lv_obj_t *label_date_month = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_month, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_label_set_text(label_date_month, "12");
    lv_obj_set_style_text_color(label_date_month, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(label_date_month, label_date_year_text, LV_ALIGN_OUT_RIGHT_MID, 6, 0);

    lv_obj_t *label_date_month_text = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_month_text, &mian_date_font, LV_PART_MAIN);
    lv_label_set_text(label_date_month_text, "月");
    lv_obj_set_style_text_color(label_date_month_text, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(label_date_month_text, label_date_month, LV_ALIGN_OUT_RIGHT_MID, 4, 0);

    lv_obj_t *label_date_day = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_day, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_label_set_text(label_date_day, "12");
    lv_obj_set_style_text_color(label_date_day, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(label_date_day, label_date_month_text, LV_ALIGN_OUT_RIGHT_MID, 6, 0);

    lv_obj_t *label_date_day_text = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_day_text, &mian_date_font, LV_PART_MAIN);
    lv_label_set_text(label_date_day_text, "日");
    lv_obj_set_style_text_color(label_date_day_text, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(label_date_day_text, label_date_day, LV_ALIGN_OUT_RIGHT_MID, 4, 0);

    lv_obj_t *label_date_week_text = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_week_text, &mian_date_font, LV_PART_MAIN);
    lv_label_set_text(label_date_week_text, "星期");
    lv_obj_set_style_text_color(label_date_week_text, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(label_date_week_text, label_date_day_text, LV_ALIGN_OUT_RIGHT_MID, 12, 0);

    lv_obj_t *label_date_week = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_week, &mian_date_font, LV_PART_MAIN);
    lv_label_set_text(label_date_week, "一");
    lv_obj_set_style_text_color(label_date_week, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(label_date_week, label_date_week_text, LV_ALIGN_OUT_RIGHT_MID, 3, 0);

    /* 时间显示 */
    lv_obj_t *label_time_hour = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_time_hour, &main_clock, LV_PART_MAIN);
    lv_label_set_text(label_time_hour, "23");
    lv_obj_set_style_text_color(label_time_hour, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label_time_hour, LV_ALIGN_BOTTOM_LEFT, 30, -10);

    lv_obj_t *label_time_lab1 = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_time_lab1, &main_clock, LV_PART_MAIN);
    lv_label_set_text(label_time_lab1, ":");
    lv_obj_set_style_text_color(label_time_lab1, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(label_time_lab1, label_time_hour, LV_ALIGN_OUT_RIGHT_BOTTOM, 3, 0);

    lv_obj_t *label_time_min = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_time_min, &main_clock, LV_PART_MAIN);
    lv_label_set_text(label_time_min, "12");
    lv_obj_set_style_text_color(label_time_min, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(label_time_min, label_time_lab1, LV_ALIGN_OUT_RIGHT_BOTTOM, 3, 0);

    lv_obj_t *label_time_lab2 = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_time_lab2, &main_clock, LV_PART_MAIN);
    lv_label_set_text(label_time_lab2, ":");
    lv_obj_set_style_text_color(label_time_lab2, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(label_time_lab2, label_time_min, LV_ALIGN_OUT_RIGHT_BOTTOM, 3, 0);

    lv_obj_t *label_time_sec = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_time_sec, &main_clock, LV_PART_MAIN);
    lv_label_set_text(label_time_sec, "30");
    lv_obj_set_style_text_color(label_time_sec, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(label_time_sec, label_time_lab2, LV_ALIGN_OUT_RIGHT_BOTTOM, 3, 0);

    lv_obj_set_grid_cell(obj_time, LV_GRID_ALIGN_STRETCH, 0, 3,
                         LV_GRID_ALIGN_STRETCH, 0, 1);

    /*========================================
     * 功能模块样式宏定义
     *========================================*/
    #define CREATE_APP_TILE(parent, icon_img, text, col, row, col_span, row_span) \
        do { \
            lv_obj_t *tile = lv_obj_create(parent); \
            lv_obj_set_style_border_opa(tile, 150, LV_PART_MAIN); \
            lv_obj_set_style_border_width(tile, 2, LV_PART_MAIN); \
            lv_obj_set_style_border_color(tile, lv_color_hex(0xffffff), LV_PART_MAIN); \
            lv_obj_set_style_bg_opa(tile, 30, LV_PART_MAIN); \
            lv_obj_set_style_bg_color(tile, lv_color_hex(0xffffff), LV_PART_MAIN); \
            lv_obj_set_style_radius(tile, 15, LV_PART_MAIN); \
            lv_obj_set_style_shadow_width(tile, 8, LV_PART_MAIN); \
            lv_obj_set_style_shadow_opa(tile, 60, LV_PART_MAIN); \
            lv_obj_set_style_shadow_color(tile, lv_color_hex(0x000000), LV_PART_MAIN); \
            \
            lv_obj_t *icon = lv_img_create(tile); \
            lv_img_set_src(icon, icon_img); \
            lv_obj_set_size(icon, 55, 55); \
            lv_obj_align(icon, LV_ALIGN_CENTER, 0, -10); \
            lv_obj_clear_flag(icon, LV_OBJ_FLAG_CLICKABLE); \
            \
            lv_obj_t *label = lv_label_create(tile); \
            lv_obj_set_style_text_font(label, &main_imgfont, LV_PART_MAIN); \
            lv_label_set_text(label, text); \
            lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN); \
            lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -8); \
            lv_obj_clear_flag(label, LV_OBJ_FLAG_CLICKABLE); \
            \
            lv_obj_set_grid_cell(tile, LV_GRID_ALIGN_STRETCH, col, col_span, \
                                 LV_GRID_ALIGN_STRETCH, row, row_span); \
            obj = tile; \
        } while(0)

    /*========================================
     * 第一行右侧：相册
     *========================================*/
    lv_obj_t *obj;
    CREATE_APP_TILE(cont, &main_photo, "相册", 3, 0, 1, 1);

    /*========================================
     * 第二行：视频、音乐、游戏
     *========================================*/
    CREATE_APP_TILE(cont, &main_video, "视频", 1, 1, 1, 1);
    lv_obj_add_event_cb(obj, lv_event_video_cb, LV_EVENT_CLICKED, NULL);

    CREATE_APP_TILE(cont, &main_music, "音乐", 2, 1, 1, 1);

    CREATE_APP_TILE(cont, &game, "游戏", 3, 1, 1, 1);
    lv_obj_add_event_cb(obj, lv_event_game_cb, LV_EVENT_CLICKED, NULL);

    /*========================================
     * 第三行：文件、设置、计算器
     *========================================*/
    CREATE_APP_TILE(cont, &main_file, "文件", 1, 2, 1, 1);

    CREATE_APP_TILE(cont, &main_set, "设置", 2, 2, 1, 1);
    lv_obj_add_event_cb(obj, lv_event_setting_cb, LV_EVENT_CLICKED, NULL);

    CREATE_APP_TILE(cont, &main_calculator, "计算器", 3, 2, 1, 1);
    lv_obj_add_event_cb(obj, lv_event_calculator_cb, LV_EVENT_CLICKED, NULL);

    /*========================================
     * 左侧大卡片：欢迎信息（跨2行）
     *========================================*/
    lv_obj_t *welcome_card = lv_obj_create(cont);
    lv_obj_set_style_border_opa(welcome_card, 150, LV_PART_MAIN);
    lv_obj_set_style_border_width(welcome_card, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(welcome_card, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(welcome_card, 40, LV_PART_MAIN);
    lv_obj_set_style_bg_color(welcome_card, lv_color_hex(0x3498db), LV_PART_MAIN);
    lv_obj_set_style_radius(welcome_card, 15, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(welcome_card, 12, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(welcome_card, 80, LV_PART_MAIN);
    lv_obj_set_style_shadow_color(welcome_card, lv_color_hex(0x000000), LV_PART_MAIN);

    lv_obj_t *welcome_label = lv_label_create(welcome_card);
    lv_label_set_long_mode(welcome_label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(welcome_label, "LVGL\n综合界面");
    lv_obj_set_style_text_font(welcome_label, &mytext18, 0);
    lv_obj_set_style_text_color(welcome_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_width(welcome_label, 180);
    lv_obj_set_style_text_align(welcome_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_line_space(welcome_label, 8, 0);
    lv_obj_center(welcome_label);

    lv_obj_set_grid_cell(welcome_card, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 2);

    /* 清理宏定义 */
    #undef CREATE_APP_TILE
}