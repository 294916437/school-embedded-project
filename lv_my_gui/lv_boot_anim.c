#include "lvgl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lv_boot_anim.h"

LV_FONT_DECLARE(boot_text_28);

static lv_obj_t *boot_text_obj1;
static   lv_obj_t *boot_text_obj;
static lv_obj_t * boot_text_label;
static lv_obj_t * boot_text_label1;
static const char * full_text = "STM32F429IGT6";
static const char * full_text1 = "寄存器开发+FreeRtos+LVGL";
static size_t text_len = 0;
static size_t text_len1 = 0;

void lv_boot_anim_close(void)
{
    lv_obj_del(boot_text_obj);
    lv_obj_del(boot_text_obj1);
}


char *strndup(const char *s, size_t n)
{
    size_t len = strnlen(s, n);
    char *dup = (char *)malloc(len + 1);
    if (dup) {
        memcpy(dup, s, len);
        dup[len] = '\0';
    }
    return dup;
}


static void boot_text_anim_a_cb(void *a,int32_t v)
{
    lv_obj_set_style_text_opa(a,v,LV_PART_MAIN);
}
 static void boot_text_anim_b_cb(void *a,int32_t v)
 {
   lv_obj_set_style_text_opa(a,v,LV_PART_MAIN);
 }

  static void anim_a_ready_cb(lv_anim_t * anim1)
  {
    lv_anim_del(anim1->var, NULL); // 删除动画
  }
  static void anim_b_ready_cb(lv_anim_t * anim2)
  {
    lv_anim_del(anim2->var, NULL); // 删除动画
    lv_boot_anim_close();
  }


//打字效果函数
static void typing_effect(lv_timer_t * task)
{
    if (text_len < strlen(full_text))
    {
        text_len++;
        lv_label_set_text(boot_text_label, strndup(full_text, text_len));
    }
    else
    {
        if(text_len1 < strlen(full_text1) )
        {
            text_len1++;
            lv_label_set_text(boot_text_label1, strndup(full_text1, text_len1));
        }
        else
        {
            lv_timer_del(task); // 动画完成后删除定时器
            lv_anim_t boot_text_a;
            lv_anim_init(&boot_text_a);
            lv_anim_set_var(&boot_text_a, boot_text_label);
            lv_anim_set_values(&boot_text_a,255,0);
            lv_anim_set_exec_cb(&boot_text_a, boot_text_anim_a_cb);
            lv_anim_set_time(&boot_text_a, 5100);                             // 动画的时间
            lv_anim_set_ready_cb(&boot_text_a, anim_a_ready_cb); // 设置动画结束回调函数
            lv_anim_start(&boot_text_a);

            lv_anim_t boot_text_b;
            lv_anim_init(&boot_text_b);
            lv_anim_set_var(&boot_text_b, boot_text_label1);
            lv_anim_set_values(&boot_text_b,255,0);
            lv_anim_set_exec_cb(&boot_text_b, boot_text_anim_b_cb);
            lv_anim_set_time(&boot_text_b, 5100);                             // 动画的时间
            lv_anim_set_ready_cb(&boot_text_b, anim_b_ready_cb); // 设置动画结束回调函数
            lv_anim_start(&boot_text_b);
        }
    }
}


void create_typing_effect(lv_obj_t * parent)
{
    boot_text_obj = lv_obj_create(parent);
    lv_obj_set_size(boot_text_obj,400,100);
    lv_obj_align(boot_text_obj, LV_ALIGN_CENTER, 0, -100);
    lv_obj_set_style_bg_opa(boot_text_obj,0,LV_PART_MAIN);
    lv_obj_set_style_border_opa(boot_text_obj,0,LV_PART_MAIN);

    boot_text_obj1= lv_obj_create(parent);
    lv_obj_set_size(boot_text_obj1,700,100);
    lv_obj_align(boot_text_obj1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(boot_text_obj1,0,LV_PART_MAIN);
    lv_obj_set_style_border_opa(boot_text_obj1,0,LV_PART_MAIN);

    boot_text_label = lv_label_create(boot_text_obj);
    lv_obj_set_style_text_font(boot_text_label,&boot_text_28,LV_PART_MAIN);
    lv_label_set_text(boot_text_label," ");
    lv_obj_set_style_text_color(boot_text_label,lv_color_hex(0xFFFFFF),LV_PART_MAIN);
    lv_obj_align(boot_text_label, LV_ALIGN_CENTER, 0, 0);


    boot_text_label1 = lv_label_create(boot_text_obj1);
    lv_obj_set_style_text_font(boot_text_label1,&boot_text_28,LV_PART_MAIN);
    lv_label_set_text(boot_text_label1," ");
    lv_obj_set_style_text_color(boot_text_label1,lv_color_hex(0xFFFFFF),LV_PART_MAIN);
    lv_obj_align(boot_text_label1, LV_ALIGN_CENTER, 0, 0);

    text_len1 =0;
    text_len = 0;
   lv_timer_create(typing_effect, 200, NULL);    // 创建定时器，每隔 200 毫秒调用一次 typing_effect 函数
}

void lv_boot_text_anim(void)
{
    lv_obj_set_style_bg_color(lv_scr_act(),lv_color_hex(0x000000),LV_PART_MAIN);
    create_typing_effect(lv_scr_act());
}

