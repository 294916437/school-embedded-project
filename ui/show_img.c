#include "show_img.h"
void JPG_Mode(void)
{
    //1、创建图片
    lv_obj_t * jpg_img = lv_img_create(lv_scr_act());
    //2、设置图片
    lv_img_set_src(jpg_img, "A:1.jpg");
    //3、设置图片位置
    lv_obj_center(jpg_img);
}

void PNG_Mode(void)
{
    //1、创建图片
    lv_obj_t * png_img = lv_img_create(lv_scr_act());
    //2、设置图片
    lv_img_set_src(png_img, "A:left_led_off.png");
    //3、设置图片位置
    lv_obj_center(png_img);

}