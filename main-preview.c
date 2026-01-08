#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#ifndef LVGL_LIVE_PREVIEW
    #include "lv_drivers/display/fbdev.h"
    #include "lv_drivers/indev/evdev.h"
    #include <unistd.h>
    #include <pthread.h>
    #include <time.h>
    #include <sys/time.h>
#endif

#define DISP_BUF_SIZE (128 * 1024)
lv_obj_t * screen = NULL;
lv_obj_t * btn1 = NULL;
lv_obj_t * btn2 = NULL;
lv_obj_t * btn_label = NULL;

static void div_btn1_event_key_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED)
    {
        // 1. 修改 btn1 背景颜色为红色
        lv_obj_set_style_bg_color(btn1, lv_color_hex(0xff0000), 0);

        // 2. 创建蓝色的 btn2
        lv_obj_t * btn2 = lv_btn_create(screen);
        lv_obj_set_size(btn2, 100, 80);
        lv_obj_set_style_bg_color(btn2, lv_color_hex(0x0000ff), 0);
        
        // 设置 btn2 位置 (例如放在 btn1 右侧)
        lv_obj_align_to(btn2, btn1, LV_ALIGN_OUT_RIGHT_TOP, 20, 0); 
        
        lv_obj_t * label2 = lv_label_create(btn2);
        lv_label_set_text(label2, "btn2");
        lv_obj_center(label2);
    }
}


void ui_app_start(void)
{
    screen = lv_obj_create(lv_scr_act());             //创建屏幕对象
    lv_obj_set_size(screen, 800, 600);                //设置屏幕对象大小
    lv_obj_center(screen);                            //设置屏幕对象居中
    btn1 = lv_btn_create(screen);      //创建屏幕对象按钮
    lv_obj_set_size(btn1, 100,80);                    //设置屏幕对象按钮大小
    
    // 始设置为蓝色，这样点击变蓝才有视觉效果；并将 NULL 改为 0 修复警告
    lv_obj_set_style_bg_color(btn1, lv_color_hex(0x0000ff), 0);
    
    lv_obj_align(btn1, LV_ALIGN_TOP_LEFT, 20, 20);    // 设置按钮基于屏幕对象的左上角(稍微留点边距)
    btn_label = lv_label_create(btn1);     //创建屏幕对象按钮标签
    lv_label_set_text(btn_label, "btn1");               //设置按钮标签文本
    lv_obj_set_style_text_color(btn_label, lv_color_hex(0xffffff), 0);//设置文本字体颜色
    lv_obj_center(btn_label);                             //设置文本居中

    //添加按钮事件函数（回调函数）
    lv_obj_add_event_cb(btn1, div_btn1_event_key_cb, LV_EVENT_CLICKED, 0);
}
// === 插件专用入口 ===
#ifdef LVGL_LIVE_PREVIEW
void lvgl_live_preview_init(void) {
    // 插件会自动处理屏幕初始化，这里只需要调用 UI 构建函数
    ui_app_start();
}
#endif
#ifndef LVGL_LIVE_PREVIEW
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, 0);
        start_ms = (tv_start.tv_sec * 1000) + (tv_start.tv_usec / 1000);
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, 0);
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
    lv_disp_draw_buf_init(&disp_buf, buf, 0, DISP_BUF_SIZE);

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
    lv_obj_set_style_bg_color(div, lv_color_hex(0xffff00), 0);//设置对象背景颜色
    lv_obj_set_style_border_width(div, 5, 0);                //设置对象边框宽度
    lv_obj_set_style_border_color(div, lv_color_hex(0x0000ff), 0);//设置对象边框颜色
    lv_obj_set_style_radius(div, 240, 0);//设置对象弧度
    //5、设置对象位置（中心）
    lv_obj_center(div);
    //lv_obj_align(div, LV_ALIGN_CENTER, 0, 0);

    //1、创建对象按钮
    lv_obj_t * btn = lv_btn_create(div);
    //2、设置按钮大小(80*80)
    lv_obj_set_size(btn, 80, 80);
    //3、设置按钮样式 A R G B 
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x8000ff00), 0);
    //4、设置按钮背景透明度
    lv_obj_set_style_bg_opa(btn, LV_OPA_80, 0);
    //5、设置按钮位置
    lv_obj_center(btn);


    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}
#endif
