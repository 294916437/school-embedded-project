#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define DISP_BUF_SIZE (128 * 1024)
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000) + (tv_start.tv_usec / 1000);
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000) + (tv_now.tv_usec / 1000);

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
/*
*   MODE     :       函数工程初始化LVGL函数且设置屏幕分辨率宽高
*   package1 ：      显示屏分辨率宽度
*   package2 :       显示屏分辨率高度
*/

void init_lvgl(int LCD_WIDTH, int LCD_HEIGHT)
{
    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = LCD_WIDTH;
    disp_drv.ver_res    = LCD_HEIGHT;
    lv_disp_drv_register(&disp_drv);

    evdev_init();
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;

    /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev_drv_1.read_cb = evdev_read;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

#if 0
    /*Set a cursor for the mouse*/
    LV_IMG_DECLARE(mouse_cursor_icon)
    lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
    lv_img_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
    lv_indev_set_cursor(mouse_indev, cursor_obj);             /*Connect the image  object to the driver*/


    /*Create a Demo*/
    lv_demo_widgets();
#endif
}

int main(void)
{
    // 1.初始化LVGL功能模块
    init_lvgl(1024, 600);

    //2、创建LCD屏幕对象
    #if 0
        lv_obj_t * Screen = lv_scr_act();            //创建屏幕
        lv_obj_t * div = lv_obj_create(Screen);      //创建屏幕对象
    #elif 1
        lv_obj_t *div = lv_obj_create(lv_scr_act());    //创建屏幕对象
    #endif
    //3、设置对象大小（480*480）
    lv_obj_set_size(div, 480, 480);
    //4、设置对象样式（颜色、弧度、边框、背景.....）style ——>样式 R G B
    lv_obj_set_style_bg_color(div, lv_color_hex(0xffff00), NULL);//设置对象背景颜色
    lv_obj_set_style_border_width(div, 5, NULL);                //设置对象边框宽度
    lv_obj_set_style_border_color(div, lv_color_hex(0x0000ff), NULL);//设置对象边框颜色
    lv_obj_set_style_radius(div, 240, NULL);//设置对象弧度
    //5、设置对象位置（中心）
    lv_obj_center(div);
    //lv_obj_align(div, LV_ALIGN_CENTER, 0, 0);

    //1、创建对象按钮
    lv_obj_t * btn = lv_btn_create(div);
    //2、设置按钮大小(80*80)
    lv_obj_set_size(btn, 80, 80);
    //3、设置按钮样式 A R G B 
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x8000ff00), NULL);
    //4、设置按钮背景透明度
    lv_obj_set_style_bg_opa(btn, LV_OPA_80, NULL);
    //5、设置按钮位置
    lv_obj_center(btn);


    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}

