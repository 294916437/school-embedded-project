#include "car_dashboard.h"
#include <stdio.h>

/* 全局/静态对象定义 */
lv_obj_t *Da = NULL;
lv_obj_t *Sa = NULL;
lv_obj_t *Da_Label = NULL;
lv_meter_indicator_t * indic;

/* 当前速度状态 */
static int32_t current_val = 0;

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

void car_dashboard_init(lv_style_t *style) {
    // 1. 创建仪表盘圆盘
    Da = lv_meter_create(lv_scr_act());
    lv_obj_set_size(Da, 400, 400); // 调整尺寸以适应常见屏幕
    lv_obj_center(Da);
    
    // 如果有样式，应用它 (可选)
    if(style) lv_obj_add_style(Da, style, 0);

    // 2. 创建仪表盘刻度
    lv_meter_scale_t * scale = lv_meter_add_scale(Da);
    lv_meter_set_scale_ticks(Da, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY)); // 小刻度
    lv_meter_set_scale_major_ticks(Da, scale, 8, 4, 15, lv_color_black(), 10);      // 大刻度
    lv_meter_set_scale_range(Da, scale, 0, 240, 270, 135); // 0-240km/h, 270度范围, 起始角度135

    /* 添加蓝色弧线表示正常速度区间 */
    lv_meter_indicator_t * indic_blue = lv_meter_add_arc(Da, scale, 3, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_meter_set_indicator_start_value(Da, indic_blue, 0);
    lv_meter_set_indicator_end_value(Da, indic_blue, 180);

    /* 添加红色弧线表示危险速度区间 */
    lv_meter_indicator_t * indic_red = lv_meter_add_arc(Da, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    lv_meter_set_indicator_start_value(Da, indic_red, 180);
    lv_meter_set_indicator_end_value(Da, indic_red, 240);

    // 3. 创建仪表盘指针
    indic = lv_meter_add_needle_line(Da, scale, 4, lv_palette_main(LV_PALETTE_RED), -10);

    // 4. 创建仪表盘标签 (速度数值)
    Da_Label = lv_label_create(Da);
    lv_label_set_text(Da_Label, "0");
    /* 设置大号字体 */
    lv_obj_set_style_text_font(Da_Label, &lv_font_montserrat_48, 0); 
    lv_obj_align(Da_Label, LV_ALIGN_CENTER, 0, 60);

    /* 添加单位标签 */
    lv_obj_t * unit_label = lv_label_create(Da);
    lv_label_set_text(unit_label, "单位:km/h");
    lv_obj_align(unit_label, LV_ALIGN_CENTER, 0, 100);

    // 5. 创建控制按钮区域
    lv_obj_t * cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, 300, 80);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    /* 减速按钮 */
    lv_obj_t * btn_dec = lv_btn_create(cont);
    lv_obj_set_size(btn_dec, 100, 50);
    /* 传递 -5 作为步进值 */
    lv_obj_add_event_cb(btn_dec, btn_event_cb, LV_EVENT_ALL, (void*)-2);
    
    lv_obj_t * lbl_dec = lv_label_create(btn_dec);
    lv_label_set_text(lbl_dec, LV_SYMBOL_MINUS);
    lv_obj_center(lbl_dec);

    /* 加速按钮 */
    lv_obj_t * btn_inc = lv_btn_create(cont);
    lv_obj_set_size(btn_inc, 100, 50);
    /* 传递 +5 作为步进值 */
    lv_obj_add_event_cb(btn_inc, btn_event_cb, LV_EVENT_ALL, (void*)2);

    lv_obj_t * lbl_inc = lv_label_create(btn_inc);
    lv_label_set_text(lbl_inc, LV_SYMBOL_PLUS);
    lv_obj_center(lbl_inc);
}