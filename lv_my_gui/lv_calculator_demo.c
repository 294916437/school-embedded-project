#include "lv_calculator_demo.h"
#include "lvgl/lvgl.h"
#include "stdio.h"
#include "stdlib.h"
#include <ctype.h>

#include "lv_run_main.h"

LV_FONT_DECLARE(calculator_text);

static const char * kb_map_num[23] = {

                                    "7", "8", "9", "DEL","AC", "\n",
                                    "4", "5", "6", "#DC143C +#","#DC143C -#", "\n",
                                    "1", "2", "3", "#DC143C *#","#DC143C /#", "\n",
                                    "0", ".", "%", "#DC143C =#",""};

lv_obj_t * lv_calculator_client_cont;
lv_obj_t * phone_ta;               /* 账号文字区域控件 */
lv_obj_t * label_dec;


int lv_calc_num = 0;
int lv_calc_pos = 0;
int calc_ec =0;
int lv_cbp_and_negative = 0;
int lv_cbp_and_negativee = 0;
int lv_calc_math = 0;
uint8_t lv_math_flag = 0;
double lv_math_x1 = 0;
double lv_math_x2 = 0;
double lv_math_result = 0;
char lv_chx_buf[100];



lv_obj_t * lv_calculator_background ;    //矩阵按钮背景

/**
 * 返回按钮事件处理（返回主界面）
 */
static void lv_calculator_event_handler(lv_event_t * event)
{
    lv_obj_t* obj = lv_event_get_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    if(code == LV_EVENT_CLICKED)    /*点击 */
    {
        lv_obj_del(lv_calculator_background);
        lv_run_main();
    }
}

/**
 * 关闭计算器并返回主界面（供外部调用）
 */
void lv_calculator_demo_close(void)
{
    if(lv_calculator_background != NULL)
    {
        lv_obj_del(lv_calculator_background);
        lv_calculator_background = NULL;
    }
    lv_run_main();
}


void calc_fmt_show(int x2,const char * buf,uint8_t fmt)
{
    x2 = atof((const char *)buf); /* 转换为数字 */
    char *fmtstr="";
    char outbuf[17];
    char fmt_chx_buf[100];
    memset(fmt_chx_buf, 0, sizeof(fmt_chx_buf));
    fmtstr="HEX";
    sprintf((char*)outbuf,"%X",x2); /* 输出结果到outbuf里面 */
    lv_textarea_set_text(phone_ta,(const char *)outbuf);
    strcat((char *)fmt_chx_buf, fmtstr);
    lv_label_set_text(label_dec,fmt_chx_buf);

}

uint8_t lv_math_calc_label(uint8_t ctype)
{
    char *chx;
    memset(lv_chx_buf, 0, sizeof(lv_chx_buf));
    switch(ctype)
    {
        case 0:/* 加法计算 */
          chx = "+";
          break;
       case 1:/* 减法计算 */
          chx = "-";
          break;
       case 2:/* 乘法计算 */
          chx = "*";
          break;
       case 3:/* 除法计算 */
          chx = "/";
          break;
      case 4: /*百分比 */
          chx = "%";
          break;
    }

    strcat((char *)lv_chx_buf, chx);
    strcat((char *)lv_chx_buf, " ");
    lv_label_set_text(label_dec,lv_chx_buf);
    lv_obj_set_style_text_font(label_dec,&lv_font_montserrat_24,0);
    return  ctype;
}

double lv_math_calc(double x1,double x2,uint8_t ctype)
{
    switch(ctype)
    {
        case 0:/* 加法 */
          x1=x1+x2;
          break;
        case 1:/* 减法 */
          x1=x1-x2;
          break;
        case 2:/* 乘法 */
          x1=x1*(x2);
          break;
        case 3:/* 除法 */
          x1=x1/(x2);
          break;
        case 4:/* 百分比 */
          x1=x1/100;
          break;
        case 5:/* 没有任何运算符 */
          x1=x2;
          break;
    }

    return x1;
}



static void lv_event_handler(lv_event_t *event)
{
    char str[25];
    char chx_buf[100];
    memset(chx_buf, 0, sizeof(chx_buf));
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t * obj = lv_event_get_target(event);

    if(code == LV_EVENT_VALUE_CHANGED)
    {
        uint32_t id = lv_btnmatrix_get_selected_btn(obj);
        const char * txt =  lv_btnmatrix_get_btn_text (obj,id);
        uint32_t cur = lv_textarea_get_cursor_pos(phone_ta);

        if (atof(lv_textarea_get_text(phone_ta)) == 0&&lv_calc_pos == 0&&lv_cbp_and_negative ==0&&lv_calc_math == 0)
        {
            lv_textarea_set_text(phone_ta, "");
        }

        if (txt == kb_map_num[4]) /* AC */
        {
            lv_textarea_set_text(phone_ta, "0");
            lv_label_set_text(label_dec," ");
            lv_calc_pos = 0;
            lv_cbp_and_negativee = 0;
            lv_cbp_and_negative = 0;
        }
         if(txt == kb_map_num[3]) /* 删除一个字符 */
        {
            if (calc_ec == 0)
            {
                lv_textarea_del_char(phone_ta);

                if (atof(lv_textarea_get_text(phone_ta)) == 0)
                {
                    lv_textarea_set_text(phone_ta, "0");
                }
            }
            else
            {
                lv_textarea_set_text(phone_ta, "0");
                lv_calc_math = 0;
                lv_calc_pos = 0;
                lv_cbp_and_negativee = 0;
                lv_cbp_and_negative = 0;
            }
        }
        else if (txt == kb_map_num[0]||txt == kb_map_num[1]||txt == kb_map_num[2]
                 ||txt == kb_map_num[6]||txt == kb_map_num[7]||txt == kb_map_num[8]
                 ||txt == kb_map_num[12]||txt == kb_map_num[13]||txt == kb_map_num[14]
                 ||txt == kb_map_num[18]||txt == kb_map_num[19])
        {
            if (calc_ec == 1)
            {
                lv_textarea_set_text(phone_ta, "");
                lv_label_set_text(label_dec," ");
                calc_ec = 0;
                lv_calc_num = 0;
                lv_calc_pos = 0;
                lv_cbp_and_negativee = 0;
                lv_cbp_and_negative = 0;
            }

            if (lv_math_x1!=0&&atof(lv_textarea_get_text(phone_ta)) == 0&&!strstr(lv_textarea_get_text(phone_ta), ".")&&!strstr(lv_textarea_get_text(phone_ta), "-"))
            {
                lv_textarea_set_text(phone_ta, "");
            }

            if (lv_cbp_and_negative == 1&&strstr(lv_textarea_get_text(phone_ta), "-")&&lv_cbp_and_negativee == 0)
            {
                lv_textarea_del_char(phone_ta);
                lv_cbp_and_negative = 0;
                lv_cbp_and_negativee = 1;
            }

            lv_textarea_set_cursor_pos(phone_ta, (int32_t)cur);

            if (atof(lv_textarea_get_text(phone_ta)) == 0&&txt == kb_map_num[17]&&lv_calc_pos != 1)
            {
                lv_calc_pos = 1;
                strcat((char *)chx_buf, "0.");
                lv_textarea_add_text(phone_ta,chx_buf);
            }
            else
            {
                if (txt == kb_map_num[17]&&strstr(lv_textarea_get_text(phone_ta), "."))
                {

                }
                else
                {
                    lv_textarea_add_text(phone_ta,txt);
                }
            }

        }
       else if (txt == kb_map_num[9]) /* + */
       {
            lv_calc_math = 1;
            if (lv_math_x1 == 0)
            {
                lv_math_x1 = atof((const char *)lv_textarea_get_text(phone_ta));
                lv_textarea_set_text(phone_ta, " ");
            }
            else
            {
                lv_calc_math = 0;
            }
            lv_math_flag = lv_math_calc_label(0);

       }
       else if (txt == kb_map_num[10]) /* - */
       {
            lv_calc_math = 1;
            if (lv_math_x1 == 0)
            {
                lv_math_x1 = atof((const char *)lv_textarea_get_text(phone_ta));
                lv_textarea_set_text(phone_ta, " ");
            }
            else
            {
                lv_calc_math = 0;
            }
            lv_math_flag = lv_math_calc_label(1);
       }
       else if (txt == kb_map_num[15]) /* X */
       {
            lv_calc_math = 1;
            if (lv_math_x1 == 0)
            {
                lv_math_x1 = atof((const char *)lv_textarea_get_text(phone_ta));
                lv_textarea_set_text(phone_ta, " ");
            }
            else
            {
                lv_calc_math = 0;
            }
            lv_math_flag = lv_math_calc_label(2);
       }
       else if (txt == kb_map_num[16]) /* / */
       {
            lv_calc_math = 1;
            if (lv_math_x1 == 0)
            {
                lv_math_x1 = atof((const char *)lv_textarea_get_text(phone_ta));
                lv_textarea_set_text(phone_ta, " ");
            }
            else
            {
                lv_calc_math = 0;
            }
            lv_math_flag = lv_math_calc_label(3);
       }
       else if (txt == kb_map_num[20]) /* % */
       {
            lv_calc_math = 1;
            if (lv_math_x1 == 0)
            {
                lv_math_x1 = atof((const char *)lv_textarea_get_text(phone_ta));
                lv_textarea_set_text(phone_ta, " ");
            }
            else
            {
                lv_calc_math = 0;
            }
            lv_math_flag = lv_math_calc_label(4);
       }




       else if (txt == kb_map_num[21]) /* = */
       {

            lv_math_x2 = atof((const char *)lv_textarea_get_text(phone_ta));
            lv_math_result = lv_math_calc(lv_math_x1,lv_math_x2,lv_math_flag);
            lv_label_set_text(label_dec," ");
            memset(str, 0, sizeof(str));
            calc_ec = 1;
            lv_math_x1 = 0;
            sprintf(str,"%g",lv_math_result);
            lv_textarea_set_text(phone_ta,(const char *)str);
       }
    }
}






void lv_calculator_demo(void)
{

    lv_calculator_background = lv_obj_create(lv_scr_act());
    lv_obj_set_size(lv_calculator_background,lv_obj_get_width(lv_scr_act()),lv_obj_get_height(lv_scr_act()));
    lv_obj_set_style_radius(lv_calculator_background, 0, LV_PART_MAIN);
    lv_obj_clear_flag(lv_calculator_background, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(lv_calculator_background, lv_color_make(1, 27, 54), LV_STATE_DEFAULT);

    lv_obj_t * btnm = lv_btnmatrix_create(lv_calculator_background);
    lv_btnmatrix_set_map(btnm, kb_map_num);    //设置按键数组,前面必须定义数组


    lv_btnmatrix_set_btn_width(btnm, 0, 1);         /*7*/
    lv_btnmatrix_set_btn_width(btnm, 1, 1);         /*8*/
    lv_btnmatrix_set_btn_width(btnm, 2, 1);         /*9*/
    lv_btnmatrix_set_btn_width(btnm, 3, 1);         /*DEL*/
    lv_btnmatrix_set_btn_width(btnm, 4, 1);         /*AC*/

    lv_btnmatrix_set_btn_width(btnm, 5, 1);         /*4*/
    lv_btnmatrix_set_btn_width(btnm, 6, 1);         /*5*/
    lv_btnmatrix_set_btn_width(btnm, 7, 1);         /*6*/
    lv_btnmatrix_set_btn_width(btnm, 8, 1);         /*+*/
    lv_btnmatrix_set_btn_width(btnm, 9, 1);         /*-*/

    lv_btnmatrix_set_btn_width(btnm, 10, 2);        /*1*/
    lv_btnmatrix_set_btn_width(btnm, 11, 2);        /*2*/
    lv_btnmatrix_set_btn_width(btnm, 12, 2);        /*3*/
    lv_btnmatrix_set_btn_width(btnm, 13, 2);        /* * */
    lv_btnmatrix_set_btn_width(btnm, 14, 2);        /*/*/

    lv_btnmatrix_set_btn_width(btnm, 15, 2);        /*0*/
    lv_btnmatrix_set_btn_width(btnm, 16, 2);        /*.*/
    lv_btnmatrix_set_btn_width(btnm, 17, 2);        /*%*/
    lv_btnmatrix_set_btn_width(btnm, 18, 2);        /*=*/


    lv_btnmatrix_set_btn_ctrl(btnm,8,LV_BTNMATRIX_CTRL_RECOLOR);
    lv_btnmatrix_set_btn_ctrl(btnm,9,LV_BTNMATRIX_CTRL_RECOLOR);
    lv_btnmatrix_set_btn_ctrl(btnm,13,LV_BTNMATRIX_CTRL_RECOLOR);
    lv_btnmatrix_set_btn_ctrl(btnm,14,LV_BTNMATRIX_CTRL_RECOLOR);
    lv_btnmatrix_set_btn_ctrl(btnm,18,LV_BTNMATRIX_CTRL_RECOLOR);

    lv_obj_set_size(btnm,lv_obj_get_width(lv_scr_act()), lv_obj_get_height(lv_scr_act())/1.6);
    lv_obj_align(btnm,LV_ALIGN_BOTTOM_MID,0,20);
    lv_obj_set_style_bg_color(btnm,lv_color_hex(0xFFFFFF),LV_PART_ITEMS);
    lv_obj_set_style_radius(btnm,0,0);
    lv_obj_set_style_border_opa(btnm,100,0);

    lv_obj_add_event_cb(btnm, lv_event_handler, LV_EVENT_ALL, NULL);


    /* 创建输入框 */
    phone_ta = lv_textarea_create(lv_calculator_background);
    lv_textarea_set_text(phone_ta, "0");
    lv_textarea_set_one_line(phone_ta, true);           /* 将文本区域配置为一行或恢复正常 */
    lv_textarea_set_cursor_click_pos(phone_ta,false);   /* 隐藏光标 */
    lv_obj_set_size(phone_ta, lv_obj_get_width(lv_scr_act()),130);
    lv_obj_clear_flag(phone_ta, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align_to(phone_ta, btnm, LV_ALIGN_OUT_TOP_MID, 0, 0);
    lv_obj_set_style_text_font(phone_ta,&lv_font_montserrat_24,0);
    lv_obj_set_style_radius(phone_ta,0,0);
    lv_obj_set_style_bg_opa(phone_ta,250,0);
    lv_obj_set_style_border_opa(phone_ta,100,0);


    /*运算符**/
    label_dec = lv_label_create(lv_calculator_background);
    lv_label_set_text(label_dec," ");
    lv_obj_set_style_text_font(label_dec,&lv_font_montserrat_14,0);
    lv_obj_align_to(label_dec,phone_ta,LV_ALIGN_BOTTOM_RIGHT,-50,-10);

    /*标题文字*/
    lv_obj_t *label_obj = lv_obj_create(lv_calculator_background);
    lv_obj_set_size(label_obj,lv_obj_get_width(lv_scr_act()),50);
    lv_obj_align_to(label_obj,phone_ta,LV_ALIGN_OUT_TOP_MID,0,0);
    lv_obj_set_style_radius(label_obj,0,0);
    lv_obj_set_style_bg_opa(label_obj, 0, LV_PART_MAIN);

    lv_obj_t *label_name = lv_label_create(label_obj);
    lv_label_set_text(label_name,"计算器");
    lv_obj_set_style_text_letter_space(label_name, 2, LV_PART_MAIN);  //字间距
    lv_obj_set_style_text_line_space(label_name,0,LV_PART_MAIN);      //行间距
    lv_obj_set_style_text_font(label_name,&calculator_text,0);
    lv_obj_align(label_name,LV_ALIGN_TOP_MID,0,-10);

    /*关闭按钮*/
    lv_obj_t *btn_quit = lv_btn_create(label_obj);
    lv_obj_set_size(btn_quit,10,10);
    lv_obj_align_to(btn_quit,label_name,LV_ALIGN_OUT_RIGHT_MID,300,0);
    lv_obj_set_style_bg_opa(btn_quit, 0, LV_PART_MAIN);                                          
    lv_obj_set_style_shadow_opa(btn_quit, 0, 0);                                              
    lv_obj_t *label = lv_label_create(btn_quit);
    lv_label_set_text(label, LV_SYMBOL_CLOSE); // 设置标签文本为“X”
    lv_obj_center(label); // 将标签居中
    lv_obj_set_style_text_color(label, lv_color_hex(0x3a3a3a), 0);
    lv_obj_add_event_cb(btn_quit, lv_calculator_event_handler, LV_EVENT_CLICKED, NULL);

}



void lv_calculator_demo_open(void)
{
    lv_calculator_demo();
}
