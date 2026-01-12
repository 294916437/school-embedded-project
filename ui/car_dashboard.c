#include "car_dashboard.h"
#include <stdio.h>
#include <stdbool.h>
#include "../modules/LED.h"

/* 全局/静态对象定义 */
lv_obj_t *Da = NULL;
lv_obj_t *Sa = NULL;
lv_obj_t *Da_Label = NULL;
lv_meter_indicator_t * indic;

/* 额外仪表与指针 */
lv_meter_indicator_t * Sa_indic = NULL;
lv_obj_t *Sa_Label = NULL;

/* 转向 LED 控制 */
lv_obj_t *led_left = NULL;
lv_obj_t *led_right = NULL;
static lv_timer_t *left_timer = NULL;
static lv_timer_t *right_timer = NULL;
static bool left_led_state = false;
static bool right_led_state = false;
static bool left_blinking = false;
static bool right_blinking = false;

/* 调整闪烁间隔 */
static const uint32_t BLINK_MS = 500;

/* 当前速度状态 */
static int32_t current_val = 0;
/* 当前转速（rpm） */
static int32_t current_rpm = 0;

/**
 * @brief 统一更新速度值的内部函数
 * @param diff 变化量 (正数增加，负数减少)
 */
static void change_speed(int32_t diff)
{
    current_val += diff;

    /* 边界检查 */
    if(current_val < 0) current_val = 0;
    if(current_val > 240) current_val = 240;

    /* 更新指针位置 */
    if(Da && indic) {
        lv_meter_set_indicator_value(Da, indic, current_val);
    }

    /* 更新标签数字 */
    if(Da_Label) {
        lv_label_set_text_fmt(Da_Label, "%d", current_val);
    }

    /* 同步更新转速表 */
    if(Sa && Sa_indic && Sa_Label) {
        /* 0..240 -> 0..8000 rpm 映射 */
        current_rpm = (int32_t)((int64_t)current_val * 8000 / 240);
        lv_meter_set_indicator_value(Sa, Sa_indic, current_rpm);
        lv_label_set_text_fmt(Sa_Label, "%d rpm", current_rpm);
    }
}

/**
 * @brief 按钮事件回调
 * 支持短按单次调节，长按连续调节
 */
static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    /* 通过 user_data 传入步进值，例如 +2 或 -2 */
    intptr_t step = (intptr_t)lv_event_get_user_data(e);

    /* 
     * LV_EVENT_SHORT_CLICKED: 手指抬起且按下时间较短时触发一次
     * LV_EVENT_LONG_PRESSED_REPEAT: 保持按下状态时周期性触发
     */
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        change_speed((int32_t)step);
    }
}

/* 定时器回调：切换 LED 亮灭 */
static void led_timer_cb(lv_timer_t * t)
{
    int id = (int)(intptr_t)t->user_data; /* 0:left, 1:right */
    if(id == 0) {
        left_led_state = !left_led_state;
        if(left_led_state) {
            lv_obj_set_style_bg_color(led_left, lv_color_hex(0xff6b00), 0);
            led_ctrl(0, 1);
        } else {
            lv_obj_set_style_bg_color(led_left, lv_palette_main(LV_PALETTE_GREY), 0);
            led_ctrl(0, 0);
        }
    } else {
        right_led_state = !right_led_state;
        if(right_led_state) {
            lv_obj_set_style_bg_color(led_right, lv_color_hex(0xff6b00), 0);
            led_ctrl(1, 1);
        } else {
            lv_obj_set_style_bg_color(led_right, lv_palette_main(LV_PALETTE_GREY), 0);
            led_ctrl(1, 0);
        }
    }
}

/* 转向按钮回调：短按切换闪烁，再次按停止并熄灭 */
static void btn_turn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    intptr_t side = (intptr_t)lv_event_get_user_data(e); /* -1 left, +1 right */

    if(code == LV_EVENT_SHORT_CLICKED) {
        if(side < 0) {
            if(left_blinking) {
                if(left_timer) lv_timer_del(left_timer);
                left_timer = NULL;
                left_blinking = false;
                left_led_state = false;
                lv_obj_set_style_bg_color(led_left, lv_palette_main(LV_PALETTE_GREY), 0);
                led_ctrl(0, 0);
            } else {
                left_blinking = true;
                left_timer = lv_timer_create(led_timer_cb, BLINK_MS, (void*)(intptr_t)0);
            }
        } else {
            if(right_blinking) {
                if(right_timer) lv_timer_del(right_timer);
                right_timer = NULL;
                right_blinking = false;
                right_led_state = false;
                lv_obj_set_style_bg_color(led_right, lv_palette_main(LV_PALETTE_GREY), 0);
                led_ctrl(1, 0);
            } else {
                right_blinking = true;
                right_timer = lv_timer_create(led_timer_cb, BLINK_MS, (void*)(intptr_t)1);
            }
        }
    }
}



void car_dashboard_init(lv_style_t *style) {
    // 1. 两个同尺寸的仪表盘（速度和转速），居中左右分布
    const int meter_size = 260;

    lv_style_t meter_style;
    lv_style_init(&meter_style);
    lv_style_set_bg_color(&meter_style, lv_color_hex(0x071018));
    lv_style_set_border_width(&meter_style, 0);
    lv_style_set_radius(&meter_style, 8);
    lv_style_set_pad_all(&meter_style, 6);

    Da = lv_meter_create(lv_scr_act());
    lv_obj_set_size(Da, meter_size, meter_size);
    lv_obj_align(Da, LV_ALIGN_CENTER, -180, 0);
    lv_obj_add_style(Da, &meter_style, 0);
    // 显式设置对象背景颜色为紫色
    lv_obj_set_style_bg_color(Da, lv_color_hex(0x1a003f), 0);
    /* 去掉最外圈边框 */
    lv_obj_set_style_border_width(Da, 0, 0);
    lv_obj_set_style_border_opa(Da, LV_OPA_TRANSP, 0);

    // 2. 创建刻度（速度表）
    lv_meter_scale_t * scale = lv_meter_add_scale(Da);
    lv_meter_set_scale_ticks(Da, scale, 41, 2, 10, lv_color_hex(0xFFFFFF));
    lv_meter_set_scale_major_ticks(Da, scale, 8, 4, 15, lv_color_hex(0xFFFFFF), 10);
    lv_meter_set_scale_range(Da, scale, 0, 240, 270, 135); // 0-240km/h

    /* 霓虹蓝弧线表示正常速度区间 */
    lv_meter_indicator_t * indic_blue = lv_meter_add_arc(Da, scale, 6, lv_color_hex(0x00bfff), 0);
    lv_meter_set_indicator_start_value(Da, indic_blue, 0);
    lv_meter_set_indicator_end_value(Da, indic_blue, 180);

    /* 霓虹红弧线表示危险速度区间 */
    lv_meter_indicator_t * indic_red = lv_meter_add_arc(Da, scale, 6, lv_color_hex(0xff4d6d), 0);
    lv_meter_set_indicator_start_value(Da, indic_red, 180);
    lv_meter_set_indicator_end_value(Da, indic_red, 240);

    // 3. 指针（速度表）
    indic = lv_meter_add_needle_line(Da, scale, 4, lv_color_hex(0x00ffd5), -10);

    // 4. 速度标签
    Da_Label = lv_label_create(Da);
    lv_label_set_text(Da_Label, "0");
    lv_obj_set_style_text_font(Da_Label, &lv_font_montserrat_32, 0);
    lv_obj_set_style_text_color(Da_Label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(Da_Label, LV_ALIGN_CENTER, 0, 60);

    /* 单位标签 */
    lv_obj_t * unit_label = lv_label_create(Da);
    lv_label_set_text(unit_label, "km/h");
    lv_obj_set_style_text_color(unit_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(unit_label, LV_ALIGN_CENTER, 0, 100);
    /* 确保刻度数字为白色 */
    lv_obj_set_style_text_color(Da, lv_color_hex(0xFFFFFF), 0);

    /* 转速表 */
    Sa = lv_meter_create(lv_scr_act());
    lv_obj_set_size(Sa, meter_size, meter_size);
    lv_obj_align(Sa, LV_ALIGN_CENTER, 180, 0);
    lv_obj_add_style(Sa, &meter_style, 0);
    // 显式设置对象背景颜色为深蓝色
    lv_obj_set_style_bg_color(Sa, lv_color_hex(0x000f1a), 0);
    /* 去掉最外圈边框 */
    lv_obj_set_style_border_width(Sa, 0, 0);
    lv_obj_set_style_border_opa(Sa, LV_OPA_TRANSP, 0);

    lv_meter_scale_t * scale2 = lv_meter_add_scale(Sa);
    lv_meter_set_scale_ticks(Sa, scale2, 41, 2, 10, lv_color_hex(0xFFFFFF));
    lv_meter_set_scale_major_ticks(Sa, scale2, 8, 4, 15, lv_color_hex(0xFFFFFF), 10);
    lv_meter_set_scale_range(Sa, scale2, 0, 8000, 270, 135);
    Sa_indic = lv_meter_add_needle_line(Sa, scale2, 4, lv_color_hex(0x00ffd5), -10);
    Sa_Label = lv_label_create(Sa);
    lv_label_set_text(Sa_Label, "0 rpm");
    lv_obj_set_style_text_font(Sa_Label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(Sa_Label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(Sa_Label, LV_ALIGN_CENTER, 0, 60);
    /* 确保刻度数字为白色 */
    lv_obj_set_style_text_color(Sa, lv_color_hex(0xFFFFFF), 0);

    // 5. 创建控制按钮区域
    lv_obj_t * cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, 300, 80);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    // 设置背景透明
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    /* 去掉容器边框 */
    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_border_opa(cont, LV_OPA_TRANSP, 0);

    /* 两个转向指示灯（屏幕两侧） */
    led_left = lv_obj_create(lv_scr_act());
    lv_obj_set_size(led_left, 24, 24);
    lv_obj_align(led_left, LV_ALIGN_BOTTOM_LEFT, 20, -20);
    lv_obj_set_style_bg_color(led_left, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_radius(led_left, LV_RADIUS_CIRCLE, 0);

    led_right = lv_obj_create(lv_scr_act());
    lv_obj_set_size(led_right, 24, 24);
    lv_obj_align(led_right, LV_ALIGN_BOTTOM_RIGHT, -20, -20);
    lv_obj_set_style_bg_color(led_right, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_radius(led_right, LV_RADIUS_CIRCLE, 0);

    /* 左转按钮 */
    lv_obj_t * btn_turn_left = lv_btn_create(cont);
    lv_obj_set_size(btn_turn_left, 70, 50);
    lv_obj_add_event_cb(btn_turn_left, btn_turn_event_cb, LV_EVENT_ALL, (void*)-1);
    lv_obj_t * lbl_turn_l = lv_label_create(btn_turn_left);
    lv_label_set_text(lbl_turn_l, "L");
    lv_obj_center(lbl_turn_l);

    /* 减速按钮 */
    lv_obj_t * btn_dec = lv_btn_create(cont);
    lv_obj_set_size(btn_dec, 100, 50);
    lv_obj_add_event_cb(btn_dec, btn_event_cb, LV_EVENT_ALL, (void*)-2);
    lv_obj_t * lbl_dec = lv_label_create(btn_dec);
    lv_label_set_text(lbl_dec, LV_SYMBOL_MINUS);
    lv_obj_center(lbl_dec);

    /* 加速按钮 */
    lv_obj_t * btn_inc = lv_btn_create(cont);
    lv_obj_set_size(btn_inc, 100, 50);
    lv_obj_add_event_cb(btn_inc, btn_event_cb, LV_EVENT_ALL, (void*)2);
    lv_obj_t * lbl_inc = lv_label_create(btn_inc);
    lv_label_set_text(lbl_inc, LV_SYMBOL_PLUS);
    lv_obj_center(lbl_inc);

    /* 右转按钮 */
    lv_obj_t * btn_turn_right = lv_btn_create(cont);
    lv_obj_set_size(btn_turn_right, 70, 50);
    lv_obj_add_event_cb(btn_turn_right, btn_turn_event_cb, LV_EVENT_ALL, (void*)1);
    lv_obj_t * lbl_turn_r = lv_label_create(btn_turn_right);
    lv_label_set_text(lbl_turn_r, "R");
    lv_obj_center(lbl_turn_r);
}