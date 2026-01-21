#include "lv_calculator_demo.h"
#include "lvgl/lvgl.h"
#include "stdio.h"
#include "stdlib.h"
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "lv_run_main.h"

/* ==================== 字体声明 ==================== */
LV_FONT_DECLARE(calculator_text);

/* ==================== 常量定义 ==================== */
#define MAX_DISPLAY_LENGTH 16
#define BUFFER_SIZE 100
#define ERROR_MESSAGE "Error"
#define DIVISION_BY_ZERO_MSG "除数不能为零"

/* 按键布局映射 */
static const char * kb_map_num[23] = {
    "7", "8", "9", "DEL", "AC", "\n",
    "4", "5", "6", "#DC143C +#", "#DC143C -#", "\n",
    "1", "2", "3", "#DC143C *#", "#DC143C /#", "\n",
    "0", ".", "%", "#DC143C =#", ""
};

/* 运算符类型枚举 */
typedef enum {
    OP_ADD = 0,      /* 加法 */
    OP_SUBTRACT,     /* 减法 */
    OP_MULTIPLY,     /* 乘法 */
    OP_DIVIDE,       /* 除法 */
    OP_PERCENT,      /* 百分比 */
    OP_NONE          /* 无运算符 */
} operator_type_t;

/* ==================== 全局变量 ==================== */
/* UI 组件 */
static lv_obj_t *calculator_window = NULL;      /* 计算器窗口容器 */
static lv_obj_t *display_textarea = NULL;       /* 显示文本框 */
static lv_obj_t *operator_label = NULL;         /* 运算符提示标签 */

/* 计算状态变量 */
typedef struct {
    double operand1;                    /* 第一个操作数 */
    double operand2;                    /* 第二个操作数 */
    double result;                      /* 计算结果 */
    operator_type_t current_operator;   /* 当前运算符 */
    uint8_t has_decimal_point;          /* 是否已输入小数点 */
    uint8_t calculation_completed;      /* 是否刚完成计算 */
    uint8_t is_error_state;             /* 是否处于错误状态 */
    uint8_t operator_entered;           /* 是否已输入运算符 */
    uint8_t should_clear_display;       /* 下次输入是否需要清空显示 */
} calculator_state_t;

static calculator_state_t calc_state = {0};

/* ==================== 内部函数声明 ==================== */
static void reset_calculator_state(void);
static void update_operator_display(operator_type_t op);
static double perform_calculation(double x1, double x2, operator_type_t op);
static void handle_number_key(const char *text);
static void handle_operator_key(operator_type_t op);
static void handle_equal_key(void);
static void handle_delete_key(void);
static void handle_clear_key(void);
static void handle_decimal_point(void);
static void show_error_message(const char *msg);
static uint8_t is_valid_number(const char *str);

/* ==================== 状态管理函数 ==================== */

/**
 * @brief 重置计算器所有状态变量
 */
static void reset_calculator_state(void)
{
    calc_state.operand1 = 0.0;
    calc_state.operand2 = 0.0;
    calc_state.result = 0.0;
    calc_state.current_operator = OP_NONE;
    calc_state.has_decimal_point = 0;
    calc_state.calculation_completed = 0;
    calc_state.is_error_state = 0;
    calc_state.operator_entered = 0;
    calc_state.should_clear_display = 0;
}

/**
 * @brief 更新运算符显示标签
 * @param op 运算符类型
 */
static void update_operator_display(operator_type_t op)
{
    const char *op_symbol;
    
    switch(op) {
        case OP_ADD:      op_symbol = " + "; break;
        case OP_SUBTRACT: op_symbol = " - "; break;
        case OP_MULTIPLY: op_symbol = " × "; break;
        case OP_DIVIDE:   op_symbol = " ÷ "; break;
        case OP_PERCENT:  op_symbol = " % "; break;
        default:          op_symbol = " "; break;
    }
    
    lv_label_set_text(operator_label, op_symbol);
    lv_obj_set_style_text_font(operator_label, &lv_font_montserrat_24, 0);
}

/**
 * @brief 显示错误信息
 * @param msg 错误消息
 */
static void show_error_message(const char *msg)
{
    lv_textarea_set_text(display_textarea, ERROR_MESSAGE);
    lv_label_set_text(operator_label, " ");
    calc_state.is_error_state = 1;
    
    /* 可选：在控制台输出详细错误信息 */
    printf("[Calculator Error] %s\n", msg);
}

/**
 * @brief 验证字符串是否为有效数字
 * @param str 待验证字符串
 * @return 1-有效, 0-无效
 */
static uint8_t is_valid_number(const char *str)
{
    if (str == NULL || strlen(str) == 0) {
        return 0;
    }
    
    char *endptr;
    strtod(str, &endptr);
    
    /* 如果整个字符串都被成功转换，endptr应指向字符串末尾 */
    return (*endptr == '\0' || *endptr == ' ');
}

/* ==================== 计算引擎 ==================== */

/**
 * @brief 执行数学运算
 * @param x1 第一个操作数
 * @param x2 第二个操作数
 * @param op 运算符类型
 * @return 计算结果
 */
static double perform_calculation(double x1, double x2, operator_type_t op)
{
    double result = 0.0;
    
    switch(op) {
        case OP_ADD:
            result = x1 + x2;
            break;
            
        case OP_SUBTRACT:
            result = x1 - x2;
            break;
            
        case OP_MULTIPLY:
            result = x1 * x2;
            break;
            
        case OP_DIVIDE:
            /* 除零检测 */
            if (fabs(x2) < 1e-10) {
                show_error_message(DIVISION_BY_ZERO_MSG);
                return 0.0;
            }
            result = x1 / x2;
            break;
            
        case OP_PERCENT:
            result = x1 / 100.0;
            break;
            
        case OP_NONE:
            result = x2;  /* 无运算符，返回第二个操作数 */
            break;
            
        default:
            result = 0.0;
            break;
    }
    
    /* 溢出检测 */
    if (isinf(result)) {
        show_error_message("数值溢出");
        return 0.0;
    }
    
    if (isnan(result)) {
        show_error_message("无效运算");
        return 0.0;
    }
    
    return result;
}

/* ==================== 按键处理函数 ==================== */

/**
 * @brief 处理数字键输入
 * @param text 数字文本
 */
static void handle_number_key(const char *text)
{
    /* 错误状态下忽略输入 */
    if (calc_state.is_error_state) {
        return;
    }
    
    /* 计算完成后输入数字，开始新计算 */
    if (calc_state.calculation_completed) {
        lv_textarea_set_text(display_textarea, "");
        lv_label_set_text(operator_label, " ");
        reset_calculator_state();
    }
    
    /* 如果标记了需要清空显示，则清空后再输入 */
    if (calc_state.should_clear_display) {
        lv_textarea_set_text(display_textarea, "");
        calc_state.should_clear_display = 0;
        calc_state.has_decimal_point = 0;
    }
    
    /* 获取当前显示内容 */
    const char *current = lv_textarea_get_text(display_textarea);
    
    /* 如果当前显示为"0"且输入的不是"0"，则替换而非追加 */
    if (strlen(current) == 1 && current[0] == '0' && text[0] != '0') {
        lv_textarea_set_text(display_textarea, text);
        return;
    }
    
    /* 检查显示长度限制 */
    if (strlen(current) >= MAX_DISPLAY_LENGTH) {
        return;  /* 超长限制，忽略输入 */
    }
    
    /* 追加数字 */
    lv_textarea_add_text(display_textarea, text);
}

/**
 * @brief 处理小数点输入
 */
static void handle_decimal_point(void)
{
    if (calc_state.is_error_state) {
        return;
    }
    
    /* 如果标记了需要清空显示，则清空后输入"0." */
    if (calc_state.should_clear_display) {
        lv_textarea_set_text(display_textarea, "0.");
        calc_state.should_clear_display = 0;
        calc_state.has_decimal_point = 1;
        return;
    }
    
    const char *current_text = lv_textarea_get_text(display_textarea);
    
    /* 已有小数点则忽略 */
    if (strstr(current_text, ".")) {
        return;
    }
    
    /* 计算完成后输入小数点，开始新的小数 */
    if (calc_state.calculation_completed) {
        lv_textarea_set_text(display_textarea, "0.");
        reset_calculator_state();
        calc_state.has_decimal_point = 1;
        return;
    }
    
    /* 显示为0时追加小数点 */
    double current_value = atof(current_text);
    if (fabs(current_value) < 1e-10 && strlen(current_text) <= 1) {
        lv_textarea_set_text(display_textarea, "0.");
    } else {
        lv_textarea_add_text(display_textarea, ".");
    }
    
    calc_state.has_decimal_point = 1;
}

/**
 * @brief 处理运算符键
 * @param op 运算符类型
 */
static void handle_operator_key(operator_type_t op)
{
    if (calc_state.is_error_state) {
        handle_clear_key();  /* 错误状态下按运算符相当于清除 */
        return;
    }
    
    const char *current_text = lv_textarea_get_text(display_textarea);
    
    /* 验证当前输入是否为有效数字 */
    if (!is_valid_number(current_text)) {
        show_error_message("无效输入");
        return;
    }
    
    double current_value = atof(current_text);
    
    /* 首次输入运算符 */
    if (calc_state.current_operator == OP_NONE) {
        calc_state.operand1 = current_value;
        calc_state.current_operator = op;
        calc_state.operator_entered = 1;
        calc_state.should_clear_display = 1;  /* 标记下次输入需要清空显示 */
        update_operator_display(op);
    } 
    /* 连续运算 */
    else if (!calc_state.should_clear_display) {
        /* 只有在已经输入了第二个操作数的情况下才进行计算 */
        calc_state.operand2 = current_value;
        calc_state.result = perform_calculation(
            calc_state.operand1, 
            calc_state.operand2, 
            calc_state.current_operator
        );
        
        if (!calc_state.is_error_state) {
            /* 显示中间结果 */
            char result_str[32];
            snprintf(result_str, sizeof(result_str), "%g", calc_state.result);
            lv_textarea_set_text(display_textarea, result_str);
            
            /* 准备下一次运算 */
            calc_state.operand1 = calc_state.result;
            calc_state.current_operator = op;
            calc_state.should_clear_display = 1;
            update_operator_display(op);
        }
    } 
    /* 更换运算符（还未输入第二个操作数） */
    else {
        calc_state.current_operator = op;
        update_operator_display(op);
    }
}

/**
 * @brief 处理等号键
 */
static void handle_equal_key(void)
{
    if (calc_state.is_error_state || calc_state.current_operator == OP_NONE) {
        return;
    }
    
    const char *current_text = lv_textarea_get_text(display_textarea);
    
    if (!is_valid_number(current_text)) {
        show_error_message("无效输入");
        return;
    }
    
    calc_state.operand2 = atof(current_text);
    calc_state.result = perform_calculation(
        calc_state.operand1, 
        calc_state.operand2, 
        calc_state.current_operator
    );
    
    if (!calc_state.is_error_state) {
        /* 格式化并显示结果 */
        char result_str[32];
        snprintf(result_str, sizeof(result_str), "%g", calc_state.result);
        lv_textarea_set_text(display_textarea, result_str);
        
        /* 清除运算符显示 */
        lv_label_set_text(operator_label, " ");
        
        /* 标记计算完成 */
        calc_state.calculation_completed = 1;
        calc_state.current_operator = OP_NONE;
        calc_state.should_clear_display = 0;
    }
}

/**
 * @brief 处理删除键
 */
static void handle_delete_key(void)
{
    /* 错误状态或计算完成后，DEL相当于清除 */
    if (calc_state.is_error_state || calc_state.calculation_completed) {
        handle_clear_key();
        return;
    }
    
    /* 如果刚按完运算符还未输入数字，DEL相当于清除 */
    if (calc_state.should_clear_display) {
        handle_clear_key();
        return;
    }
    
    /* 删除一个字符 */
    const char *current_text = lv_textarea_get_text(display_textarea);
    
    /* 检查是否删除小数点 */
    if (strlen(current_text) > 0) {
        char last_char = current_text[strlen(current_text) - 1];
        if (last_char == '.') {
            calc_state.has_decimal_point = 0;
        }
    }
    
    lv_textarea_del_char(display_textarea);
    
    /* 删空后填充0 */
    if (strlen(lv_textarea_get_text(display_textarea)) == 0) {
        lv_textarea_set_text(display_textarea, "0");
    }
}

/**
 * @brief 处理清除键（AC）
 */
static void handle_clear_key(void)
{
    lv_textarea_set_text(display_textarea, "0");
    lv_label_set_text(operator_label, " ");
    reset_calculator_state();
}

/* ==================== 事件回调函数 ==================== */

/**
 * @brief 按键矩阵事件处理
 */
static void button_matrix_event_handler(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t *obj = lv_event_get_target(event);
    
    if (code != LV_EVENT_VALUE_CHANGED) {
        return;
    }
    
    uint32_t btn_id = lv_btnmatrix_get_selected_btn(obj);
    const char *btn_text = lv_btnmatrix_get_btn_text(obj, btn_id);
    
    /* AC键 */
    if (strcmp(btn_text, kb_map_num[4]) == 0) {
        handle_clear_key();
    }
    /* DEL键 */
    else if (strcmp(btn_text, kb_map_num[3]) == 0) {
        handle_delete_key();
    }
    /* 数字键 0-9 */
    else if (strcmp(btn_text, "0") == 0 || strcmp(btn_text, "1") == 0 ||
             strcmp(btn_text, "2") == 0 || strcmp(btn_text, "3") == 0 ||
             strcmp(btn_text, "4") == 0 || strcmp(btn_text, "5") == 0 ||
             strcmp(btn_text, "6") == 0 || strcmp(btn_text, "7") == 0 ||
             strcmp(btn_text, "8") == 0 || strcmp(btn_text, "9") == 0) {
        handle_number_key(btn_text);
    }
    /* 小数点 */
    else if (strcmp(btn_text, ".") == 0) {
        handle_decimal_point();
    }
    /* 加法 */
    else if (strcmp(btn_text, kb_map_num[9]) == 0) {
        handle_operator_key(OP_ADD);
    }
    /* 减法 */
    else if (strcmp(btn_text, kb_map_num[10]) == 0) {
        handle_operator_key(OP_SUBTRACT);
    }
    /* 乘法 */
    else if (strcmp(btn_text, kb_map_num[15]) == 0) {
        handle_operator_key(OP_MULTIPLY);
    }
    /* 除法 */
    else if (strcmp(btn_text, kb_map_num[16]) == 0) {
        handle_operator_key(OP_DIVIDE);
    }
    /* 百分比 */
    else if (strcmp(btn_text, "%") == 0) {
        handle_operator_key(OP_PERCENT);
    }
    /* 等号 */
    else if (strcmp(btn_text, kb_map_num[21]) == 0) {
        handle_equal_key();
    }
}

/**
 * @brief 返回按钮事件处理
 */
static void back_button_event_handler(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    
    if (code == LV_EVENT_CLICKED) {
        if (calculator_window != NULL) {
            lv_obj_del(calculator_window);
            calculator_window = NULL;
        }
        reset_calculator_state();
        main_grid(&style);
    }
}

/* ==================== UI创建函数 ==================== */

/**
 * @brief 创建计算器界面
 */
static void create_calculator_ui(void)
{
    /* 创建窗口容器 */
    calculator_window = lv_win_create(lv_scr_act(), 80);
    lv_obj_set_size(calculator_window, 
                    lv_obj_get_width(lv_scr_act()), 
                    lv_obj_get_height(lv_scr_act()));
    
    /* 添加返回按钮 */
    lv_obj_t *back_btn = lv_win_add_btn(calculator_window, LV_SYMBOL_LEFT, 50);
    lv_obj_set_style_text_font(back_btn, &lv_font_montserrat_24, 0);
    lv_obj_add_event_cb(back_btn, back_button_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_text_color(back_btn, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(back_btn, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(back_btn, 0, 0);
    lv_obj_align(back_btn, LV_ALIGN_RIGHT_MID, 0, 0);
    
    /* 设置标题 */
    lv_obj_t *title = lv_win_add_title(calculator_window, "计算器");
    lv_obj_set_style_text_font(title, &calculator_text, LV_PART_MAIN);
    
    /* 标题右侧占位 */
    lv_obj_t *spacer_btn = lv_win_add_btn(calculator_window, " ", 50);
    lv_obj_set_style_bg_opa(spacer_btn, 0, 0);
    lv_obj_set_style_shadow_opa(spacer_btn, 0, 0);
    
    /* 获取并配置标题栏 */
    lv_obj_t *header = lv_win_get_header(calculator_window);
    lv_obj_set_style_bg_color(header, lv_color_hex(0xe9e9e9), LV_PART_MAIN);
    
    /* 获取内容容器 */
    lv_obj_t *content = lv_win_get_content(calculator_window);
    lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(content, lv_color_make(1, 27, 54), LV_PART_MAIN);
    lv_obj_set_style_pad_left(content, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(content, 0, LV_STATE_DEFAULT);
    
    /* 创建显示文本框 */
    display_textarea = lv_textarea_create(content);
    lv_textarea_set_text(display_textarea, "0");
    lv_textarea_set_one_line(display_textarea, true);
    lv_textarea_set_cursor_click_pos(display_textarea, false);
    lv_obj_set_size(display_textarea, lv_obj_get_width(lv_scr_act()), 130);
    lv_obj_clear_flag(display_textarea, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(display_textarea, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_text_font(display_textarea, &lv_font_montserrat_24, 0);
    lv_obj_set_style_radius(display_textarea, 0, 0);
    lv_obj_set_style_bg_opa(display_textarea, 250, 0);
    lv_obj_set_style_border_opa(display_textarea, 100, 0);
    
    /* 运算符提示标签 */
    operator_label = lv_label_create(content);
    lv_label_set_text(operator_label, " ");
    lv_obj_set_style_text_font(operator_label, &lv_font_montserrat_14, 0);
    lv_obj_align_to(operator_label, display_textarea, LV_ALIGN_BOTTOM_RIGHT, -50, -10);
    
    /* 创建按键矩阵 */
    lv_obj_t *button_matrix = lv_btnmatrix_create(content);
    lv_btnmatrix_set_map(button_matrix, kb_map_num);
    
    /* 设置按钮宽度 */
    lv_btnmatrix_set_btn_width(button_matrix, 0, 1);   /* 7 */
    lv_btnmatrix_set_btn_width(button_matrix, 1, 1);   /* 8 */
    lv_btnmatrix_set_btn_width(button_matrix, 2, 1);   /* 9 */
    lv_btnmatrix_set_btn_width(button_matrix, 3, 1);   /* DEL */
    lv_btnmatrix_set_btn_width(button_matrix, 4, 1);   /* AC */
    
    lv_btnmatrix_set_btn_width(button_matrix, 5, 1);   /* 4 */
    lv_btnmatrix_set_btn_width(button_matrix, 6, 1);   /* 5 */
    lv_btnmatrix_set_btn_width(button_matrix, 7, 1);   /* 6 */
    lv_btnmatrix_set_btn_width(button_matrix, 8, 1);   /* + */
    lv_btnmatrix_set_btn_width(button_matrix, 9, 1);   /* - */
    
    lv_btnmatrix_set_btn_width(button_matrix, 10, 2);  /* 1 */
    lv_btnmatrix_set_btn_width(button_matrix, 11, 2);  /* 2 */
    lv_btnmatrix_set_btn_width(button_matrix, 12, 2);  /* 3 */
    lv_btnmatrix_set_btn_width(button_matrix, 13, 2);  /* * */
    lv_btnmatrix_set_btn_width(button_matrix, 14, 2);  /* / */
    
    lv_btnmatrix_set_btn_width(button_matrix, 15, 2);  /* 0 */
    lv_btnmatrix_set_btn_width(button_matrix, 16, 2);  /* . */
    lv_btnmatrix_set_btn_width(button_matrix, 17, 2);  /* % */
    lv_btnmatrix_set_btn_width(button_matrix, 18, 2);  /* = */
    
    /* 设置运算符颜色 */
    lv_btnmatrix_set_btn_ctrl(button_matrix, 8, LV_BTNMATRIX_CTRL_RECOLOR);
    lv_btnmatrix_set_btn_ctrl(button_matrix, 9, LV_BTNMATRIX_CTRL_RECOLOR);
    lv_btnmatrix_set_btn_ctrl(button_matrix, 13, LV_BTNMATRIX_CTRL_RECOLOR);
    lv_btnmatrix_set_btn_ctrl(button_matrix, 14, LV_BTNMATRIX_CTRL_RECOLOR);
    lv_btnmatrix_set_btn_ctrl(button_matrix, 18, LV_BTNMATRIX_CTRL_RECOLOR);
    
    /* 设置按键矩阵样式 */
    lv_obj_set_size(button_matrix, 
                    lv_obj_get_width(lv_scr_act()), 
                    lv_obj_get_height(lv_scr_act()) / 1.8);
    lv_obj_align(button_matrix, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_bg_color(button_matrix, lv_color_hex(0xFFFFFF), LV_PART_ITEMS);
    lv_obj_set_style_radius(button_matrix, 0, 0);
    lv_obj_set_style_border_opa(button_matrix, 100, 0);
    
    /* 添加事件回调 */
    lv_obj_add_event_cb(button_matrix, button_matrix_event_handler, LV_EVENT_ALL, NULL);
}

/* ==================== 对外接口函数 ==================== */

/**
 * @brief 打开计算器模块
 */
void lv_calculator_demo_open(void)
{
    reset_calculator_state();
    create_calculator_ui();
}

/**
 * @brief 关闭计算器模块
 */
void lv_calculator_demo_close(void)
{
    if (calculator_window != NULL) {
        lv_obj_del(calculator_window);
        calculator_window = NULL;
    }
    reset_calculator_state();
    main_grid(&style);
}