#include "lv_run_main.h"
#include "lvgl.h"

#include "lv_boot_anim.h"
#include "lv_setting_demo.h"
#include "lv_calculator_demo.h"

LV_FONT_DECLARE(main_imgfont);
LV_FONT_DECLARE(main_clock);
LV_FONT_DECLARE(mytext18);

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



static  lv_obj_t *tileview;
static void  main_grid(void);


lv_obj_t* cont;  //主界面 容器(父类)


//---------------------------------------------------------------------------------------------------------------------

static void lv_event_contor_cb(lv_event_t *event)
{
   lv_event_code_t code = lv_event_get_code(event);

   if(code == LV_EVENT_CLICKED)
   {
       printf("contor\n");
       lv_obj_del(cont);
       /*控制台*/
//       lv_obj_set_style_bg_img_src(lv_scr_act(),NULL,LV_PART_MAIN);  //删除背景图片
       lv_obj_set_style_bg_img_opa(lv_scr_act(),0,LV_PART_MAIN);   //背景图片透明度为0
      lv_obj_t * label = lv_label_create(lv_scr_act());
      lv_label_set_text(label,"contor");

   }
}


static void lv_event_music_cb(lv_event_t *event)
{
   lv_event_code_t code = lv_event_get_code(event);

   if(code == LV_EVENT_CLICKED)
   {
       printf("music\n");
       lv_obj_del(cont);
       /*音乐*/
//       lv_obj_set_style_bg_img_src(lv_scr_act(),NULL,LV_PART_MAIN);  //删除背景图片
       lv_obj_set_style_bg_img_opa(lv_scr_act(),0,LV_PART_MAIN);   //背景图片透明度为0

//        lv_music_demo_open();

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

//       lv_obj_set_style_bg_img_src(lv_scr_act(),NULL,LV_PART_MAIN);  //删除背景图片
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
static void  main_grid(void)
{
    static lv_coord_t col_dsc[] = { 110,110,110,110,110, LV_GRID_TEMPLATE_LAST };        /* 4 列 110 ps 宽度 */
    static lv_coord_t row_dsc[] = { 100,110,110,LV_GRID_TEMPLATE_LAST };                 /* 3 个 100或110 像素高的行*/

    tileview = lv_tileview_create(lv_scr_act());
    lv_obj_set_style_bg_img_src(tileview,&main_bg1,LV_PART_MAIN);
    lv_obj_set_style_bg_img_opa(tileview,255,LV_PART_MAIN);
    lv_obj_t *tile1 = lv_tileview_add_tile(tileview,0,0,LV_DIR_RIGHT);
    lv_obj_t *tile2 = lv_tileview_add_tile(tileview,1,0,LV_DIR_LEFT);

    lv_obj_t * title_text = lv_obj_create(tile2);  //暂无内容
    lv_obj_set_size(title_text,600,300);
    lv_obj_center(title_text);
    lv_obj_set_style_bg_opa(title_text,0,0);


    lv_obj_set_scrollbar_mode(tileview,LV_SCROLLBAR_MODE_OFF);  //隐藏滚动条


    cont = lv_obj_create(tile1);
    lv_obj_set_size(cont, 650, 400);        /* 设置容器大小 */
    lv_obj_center(cont);
    lv_obj_set_grid_dsc_array(cont,col_dsc,row_dsc);
    lv_obj_set_style_bg_opa(cont,0,LV_PART_MAIN);
    lv_obj_set_style_border_opa(cont,0,LV_PART_MAIN);

    lv_obj_set_layout(cont, LV_LAYOUT_GRID); /* 开启 GRID网格容器 */

//----------------------------------时间、日期、日历-------------------------------------------------
    lv_obj_t *obj_time;
    obj_time = lv_btn_create(cont);
    lv_obj_set_style_bg_color(obj_time,lv_color_hex(0xffffff),0);
    lv_obj_set_style_bg_opa(obj_time, 25, LV_PART_MAIN);
    lv_obj_set_style_border_opa(obj_time,100, LV_PART_MAIN);
    lv_obj_set_style_border_width(obj_time,2, LV_PART_MAIN);
    lv_obj_set_style_border_color(obj_time,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(obj_time,0,0);
    lv_obj_add_state(obj_time,LV_STATE_DISABLED);

    LV_FONT_DECLARE(mian_date_font);
//
    lv_obj_t*label_date_year0 = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_year0,&lv_font_montserrat_16,LV_PART_MAIN);
    lv_label_set_text(label_date_year0,"20");
    lv_obj_set_style_text_letter_space(label_date_year0,5,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_date_year0,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align(label_date_year0,LV_ALIGN_TOP_LEFT,5,0);


    lv_obj_t*label_date_year = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_year,&lv_font_montserrat_16,LV_PART_MAIN);
    lv_label_set_text(label_date_year,"24");
    lv_obj_set_style_text_letter_space(label_date_year,5,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_date_year,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align(label_date_year,LV_ALIGN_TOP_LEFT,35,0);

    lv_obj_t*label_date_year_text = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_year_text,&mian_date_font,LV_PART_MAIN);
    lv_label_set_text(label_date_year_text,"年");
    lv_obj_set_style_text_letter_space(label_date_year_text,5,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_date_year_text,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align_to(label_date_year_text,label_date_year,LV_ALIGN_OUT_RIGHT_MID,10,0);

    lv_obj_t*label_date_month = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_month,&lv_font_montserrat_16,LV_PART_MAIN);
    lv_label_set_text(label_date_month,"12");
    lv_obj_set_style_text_letter_space(label_date_month,5,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_date_month,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align_to(label_date_month,label_date_year_text,LV_ALIGN_OUT_RIGHT_MID,10,0);

    lv_obj_t*label_date_month_text = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_month_text,&mian_date_font,LV_PART_MAIN);
    lv_label_set_text(label_date_month_text,"月");
    lv_obj_set_style_text_letter_space(label_date_month_text,5,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_date_month_text,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align_to(label_date_month_text,label_date_month,LV_ALIGN_OUT_RIGHT_MID,10,0);

    lv_obj_t*label_date_day = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_day,&lv_font_montserrat_16,LV_PART_MAIN);
    lv_label_set_text(label_date_day,"12");
    lv_obj_set_style_text_letter_space(label_date_day,5,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_date_day,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align_to(label_date_day,label_date_month_text,LV_ALIGN_OUT_RIGHT_MID,10,0);

    lv_obj_t*label_date_day_text = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_day_text,&mian_date_font,LV_PART_MAIN);
    lv_label_set_text(label_date_day_text,"日");
    lv_obj_set_style_text_letter_space(label_date_day_text,5,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_date_day_text,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align_to(label_date_day_text,label_date_day,LV_ALIGN_OUT_RIGHT_MID,10,0);

    lv_obj_t*label_date_week_text = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_week_text,&mian_date_font,LV_PART_MAIN);
    lv_label_set_text(label_date_week_text,"星期");
    lv_obj_set_style_text_letter_space(label_date_week_text,5,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_date_week_text,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align_to(label_date_week_text,label_date_day_text,LV_ALIGN_OUT_RIGHT_MID,20,0);

    lv_obj_t*label_date_week = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_date_week,&mian_date_font,LV_PART_MAIN);
    lv_label_set_text(label_date_week,"一");
    lv_obj_set_style_text_letter_space(label_date_week,5,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_date_week,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align_to(label_date_week,label_date_week_text,LV_ALIGN_OUT_RIGHT_MID,5,0);


    lv_obj_t*label_time_hour = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_time_hour,&main_clock,LV_PART_MAIN);
    lv_label_set_text(label_time_hour,"23");
    lv_obj_set_style_text_letter_space(label_time_hour,10,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_time_hour,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align(label_time_hour,LV_ALIGN_BOTTOM_LEFT,30,0);


    lv_obj_t*label_time_lab1 = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_time_lab1,&main_clock,LV_PART_MAIN);
    lv_label_set_text(label_time_lab1,":");
    lv_obj_set_style_text_letter_space(label_time_lab1,10,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_time_lab1,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align(label_time_lab1,LV_ALIGN_BOTTOM_LEFT,100,-4);

    lv_obj_t*label_time_min = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_time_min,&main_clock,LV_PART_MAIN);
    lv_label_set_text(label_time_min,"12");
    lv_obj_set_style_text_letter_space(label_time_min,10,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_time_min,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align(label_time_min,LV_ALIGN_BOTTOM_LEFT,120,0);

    lv_obj_t*label_time_lab12 = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_time_lab12,&main_clock,LV_PART_MAIN);
    lv_label_set_text(label_time_lab12,":");
    lv_obj_set_style_text_letter_space(label_time_lab12,10,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_time_lab12,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align(label_time_lab12,LV_ALIGN_BOTTOM_LEFT,190,-4);

    lv_obj_t*label_time_sec = lv_label_create(obj_time);
    lv_obj_set_style_text_font(label_time_sec,&main_clock,LV_PART_MAIN);
    lv_label_set_text(label_time_sec,"30");
    lv_obj_set_style_text_letter_space(label_time_sec,10,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_time_sec,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align(label_time_sec,LV_ALIGN_BOTTOM_LEFT,210,0);
    lv_obj_set_grid_cell(obj_time, LV_GRID_ALIGN_STRETCH, 0, 3,    //  X坐标为0，2格
                              LV_GRID_ALIGN_STRETCH, 0, 1);        //Y坐标为0，1格

//----------------------------------照片-------------------------------------------------------------------------
    lv_obj_t *obj;
    obj = lv_obj_create(cont);
    lv_obj_set_style_border_opa(obj,100, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(obj, 25, LV_PART_MAIN);

    lv_obj_t *photo_imgbtn = lv_imgbtn_create(obj);
    lv_imgbtn_set_src(photo_imgbtn,LV_IMGBTN_STATE_RELEASED,NULL,&main_photo, NULL);
    lv_obj_set_size(photo_imgbtn,55,55);
    lv_obj_align(photo_imgbtn,LV_ALIGN_CENTER,0,-15);

    lv_obj_t*label_photo = lv_label_create(obj);
    lv_obj_set_style_text_font(label_photo,&main_imgfont,LV_PART_MAIN);
    lv_label_set_text(label_photo,"相册");
    lv_obj_set_style_text_color(label_photo,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align_to(label_photo,photo_imgbtn,LV_ALIGN_OUT_BOTTOM_MID,-10,-5);

    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 2, 1,    //  X坐标为2,1格
                              LV_GRID_ALIGN_STRETCH, 1, 1);   //  Y坐标为1，1格
//---------------------------------音乐 -------------------------------------------------------------------------
    obj = lv_obj_create(cont);
    lv_obj_set_style_border_opa(obj,100, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(obj, 25, LV_PART_MAIN);

    lv_obj_t *music_imgbtn = lv_imgbtn_create(obj);
    lv_imgbtn_set_src(music_imgbtn,LV_IMGBTN_STATE_RELEASED,NULL,&main_music, NULL);
    lv_obj_set_size(music_imgbtn,55,55);
    lv_obj_align(music_imgbtn,LV_ALIGN_CENTER,0,-15);

    lv_obj_t*label_music = lv_label_create(obj);
    lv_obj_set_style_text_font(label_music,&main_imgfont,LV_PART_MAIN);
    lv_label_set_text(label_music,"音乐");
    lv_obj_set_style_text_color(label_music,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align_to(label_music,music_imgbtn,LV_ALIGN_OUT_BOTTOM_MID,-10,-5);

    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 3, 1,    //  X坐标为3,1格
                              LV_GRID_ALIGN_STRETCH, 1, 1);   //  Y坐标为1，1格
//   lv_obj_add_event_cb(music_imgbtn,lv_event_music_cb,LV_EVENT_CLICKED,NULL);
//----------------------------------视频-------------------------------------------------------------------------

    obj = lv_obj_create(cont);
    lv_obj_set_style_border_opa(obj,100, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(obj, 25, LV_PART_MAIN);

    lv_obj_t *video_imgbtn = lv_imgbtn_create(obj);
    lv_imgbtn_set_src(video_imgbtn,LV_IMGBTN_STATE_RELEASED,NULL,&main_video, NULL);
    lv_obj_set_size(video_imgbtn,55,55);
    lv_obj_align(video_imgbtn,LV_ALIGN_CENTER,0,-15);

    lv_obj_t*label_video = lv_label_create(obj);
    lv_obj_set_style_text_font(label_video,&main_imgfont,LV_PART_MAIN);
    lv_label_set_text(label_video,"视频");
    lv_obj_set_style_text_color(label_video,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align_to(label_video,video_imgbtn,LV_ALIGN_OUT_BOTTOM_MID,-10,-5);

    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 4, 1,    //  X坐标为4,1格
                              LV_GRID_ALIGN_STRETCH, 1, 1);   //  Y坐标为1，1格





//---------------------------------控制台-------------------------------------------------------------------------

    obj = lv_obj_create(cont);
    lv_obj_set_style_border_opa(obj,100, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(obj, 25, LV_PART_MAIN);


    lv_obj_t *control_imgbtn = lv_imgbtn_create(obj);
    lv_imgbtn_set_src(control_imgbtn,LV_IMGBTN_STATE_RELEASED,NULL,&main_control, NULL);
    lv_obj_set_size(control_imgbtn,55,55);
    lv_obj_align(control_imgbtn,LV_ALIGN_CENTER,0,-15);

    lv_obj_t*label_control = lv_label_create(obj);
    lv_obj_set_style_text_font(label_control,&main_imgfont,LV_PART_MAIN);
    lv_label_set_text(label_control,"控制台");
    lv_obj_set_style_text_color(label_control,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align_to(label_control,control_imgbtn,LV_ALIGN_OUT_BOTTOM_MID,-10,-5);


    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 2, 1,    //  X坐标为2,1格
                              LV_GRID_ALIGN_STRETCH, 2, 1);   //  Y坐标为2，1格

//    lv_obj_add_event_cb(control_imgbtn,lv_event_contor_cb,LV_EVENT_CLICKED,NULL);
//---------------------------------文件管理-------------------------------------------------------------------------

    obj = lv_obj_create(cont);
    lv_obj_set_style_border_opa(obj,100, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(obj, 25, LV_PART_MAIN);

    lv_obj_t *file_imgbtn = lv_imgbtn_create(obj);
    lv_imgbtn_set_src(file_imgbtn,LV_IMGBTN_STATE_RELEASED,NULL,&main_file, NULL);
    lv_obj_set_size(file_imgbtn,55,55);
    lv_obj_align(file_imgbtn,LV_ALIGN_CENTER,0,-15);

    lv_obj_t*file_set = lv_label_create(obj);
    lv_obj_set_style_text_font(file_set,&main_imgfont,LV_PART_MAIN);
    lv_label_set_text(file_set,"文件");
    lv_obj_set_style_text_color(file_set,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align_to(file_set,file_imgbtn,LV_ALIGN_OUT_BOTTOM_MID,-10,-5);


    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 3, 1,    //  X坐标为3,1格
                              LV_GRID_ALIGN_STRETCH, 2, 1);   //  Y坐标为2，1格

//---------------------------------系统设置-------------------------------------------------------------------------

    obj = lv_obj_create(cont);
    lv_obj_set_style_border_opa(obj,100, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(obj, 25, LV_PART_MAIN);

    lv_obj_t *set_imgbtn = lv_imgbtn_create(obj);
    lv_imgbtn_set_src(set_imgbtn,LV_IMGBTN_STATE_RELEASED,NULL,&main_set, NULL);
    lv_obj_set_size(set_imgbtn,55,55);
    lv_obj_align(set_imgbtn,LV_ALIGN_CENTER,0,-15);

    lv_obj_t*label_set = lv_label_create(obj);
    lv_obj_set_style_text_font(label_set,&main_imgfont,LV_PART_MAIN);
    lv_label_set_text(label_set,"设置");
    lv_obj_set_style_text_color(label_set,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align_to(label_set,set_imgbtn,LV_ALIGN_OUT_BOTTOM_MID,-10,-5);
    lv_obj_add_event_cb(set_imgbtn,lv_event_setting_cb,LV_EVENT_CLICKED,NULL);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 4, 1,    //  X坐标为4,1格
                              LV_GRID_ALIGN_STRETCH, 2, 1);   //  Y坐标为2，1格

//---------------------------------2x2网格-------------------------------------------------------------------------

    obj = lv_obj_create(cont);
    lv_obj_set_style_border_opa(obj,100, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(obj, 25, LV_PART_MAIN);

    lv_obj_t *label = lv_label_create(obj);
        lv_label_set_long_mode(label,LV_LABEL_LONG_WRAP);
    lv_label_set_text(label,"寄存器开发Freertos+LVGL综合界面");

    lv_obj_set_style_text_font(label,&mytext18,0);
    lv_obj_set_style_text_color(label,lv_color_hex(0xffffff),0);

    lv_obj_set_width(label,180);
    lv_obj_set_style_text_letter_space(label,-2,0);
    lv_obj_set_style_text_align(label,LV_TEXT_ALIGN_CENTER,0);
    lv_obj_align(label,LV_ALIGN_CENTER,0,0);
//STM32寄存器开发+Freertos+LVGL V8.2版本 综合界面项目

    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 2,    //  X坐标为0, 2格
                              LV_GRID_ALIGN_STRETCH, 1, 2);   //  Y坐标为1，2格


//---------------------------------计算器-------------------------------------------------------------------------

    obj = lv_obj_create(cont);
    lv_obj_set_style_border_opa(obj,100, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(obj, 25, LV_PART_MAIN);
    lv_obj_t *calculator_imgbtn = lv_imgbtn_create(obj);
    lv_imgbtn_set_src(calculator_imgbtn,LV_IMGBTN_STATE_RELEASED,NULL,&main_calculator, NULL);
    lv_obj_set_size(calculator_imgbtn,55,55);
    lv_obj_align(calculator_imgbtn,LV_ALIGN_LEFT_MID,0,-15);
    lv_obj_t*label_calculator = lv_label_create(obj);
    lv_obj_set_style_text_font(label_calculator,&main_imgfont,LV_PART_MAIN);
    lv_label_set_text(label_calculator,"计算器");
    lv_obj_set_style_text_color(label_calculator,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align_to(label_calculator,calculator_imgbtn,LV_ALIGN_OUT_BOTTOM_MID,-2,-13);
    lv_obj_add_event_cb(calculator_imgbtn,lv_event_calculator_cb,LV_EVENT_CLICKED,NULL);



//--------------------------------------画板---------------------------------------------------------------------

//    obj = lv_obj_create(cont);
//    lv_obj_set_style_border_opa(obj,100, LV_PART_MAIN);
//    lv_obj_set_style_bg_opa(obj, 25, LV_PART_MAIN);


    lv_obj_t *drawing_boardbtn = lv_imgbtn_create(obj);
    lv_imgbtn_set_src(drawing_boardbtn,LV_IMGBTN_STATE_RELEASED,NULL,&main_drawing_board, NULL);
    lv_obj_set_size(drawing_boardbtn,52,52);
    lv_obj_align(drawing_boardbtn,LV_ALIGN_RIGHT_MID,-30,-13);


    lv_obj_t*calendar_label = lv_label_create(obj);
    lv_obj_set_style_text_font(calendar_label,&main_imgfont,LV_PART_MAIN);
    lv_label_set_text(calendar_label,"画板");
    lv_obj_set_style_text_color(calendar_label,lv_color_hex(0xffffff),LV_PART_MAIN);
    lv_obj_align_to(calendar_label,drawing_boardbtn,LV_ALIGN_OUT_BOTTOM_MID,102,-13);


    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 3, 2,    //  X坐标为3,2格
                              LV_GRID_ALIGN_STRETCH, 0, 1);   //  Y坐标为0，1格


}


void lv_run_main(void)
{
   main_grid();

}