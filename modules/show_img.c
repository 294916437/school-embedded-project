#include "show_img.h"
#include <string.h>

/**
 * 显示 JPG 图片
 */
void JPG_Mode(const char *img_src)
{
    if(img_src == NULL) {
        printf("Error: img_src is NULL\n");
        return;
    }

    //1、创建图片
    lv_obj_t * jpg_img = lv_img_create(lv_scr_act());
    //2、设置图片
    lv_img_set_src(jpg_img, img_src);
    //3、设置图片位置
    lv_obj_center(jpg_img);
}

/**
 * 显示 PNG 图片
 */
void PNG_Mode(const char *img_src)
{
    if(img_src == NULL) {
        printf("Error: img_src is NULL\n");
        return;
    }

    //1、创建图片
    lv_obj_t * png_img = lv_img_create(lv_scr_act());
    //2、设置图片
    lv_img_set_src(png_img, img_src);
    //3、设置图片位置
    lv_obj_center(png_img);
}

/**
 * 显示 GIF 动图
 */
void GIF_Mode(const char *img_src)
{
    if(img_src == NULL) {
        printf("Error: img_src is NULL\n");
        return;
    }

    //1、创建图片
    lv_obj_t * gif_img = lv_gif_create(lv_scr_act());
    //2、设置图片
    lv_gif_set_src(gif_img, img_src);
    //3、设置图片位置
    lv_obj_center(gif_img);
}

/**
 * 自动检测图片类型并显示
 */
void show_image(const char *img_src)
{
    if(img_src == NULL) {
        printf("Error: img_src is NULL\n");
        return;
    }

    // 获取文件扩展名
    const char *ext = strrchr(img_src, '.');
    if(ext == NULL) {
        printf("Error: Cannot determine file type\n");
        return;
    }

    // 转换为小写进行比较
    char ext_lower[10] = {0};
    int i = 0;
    while(ext[i] != '\0' && i < 9) {
        ext_lower[i] = tolower(ext[i]);
        i++;
    }

    // 根据扩展名选择合适的显示函数
    if(strcmp(ext_lower, ".jpg") == 0 || strcmp(ext_lower, ".jpeg") == 0) {
        JPG_Mode(img_src);
    } else if(strcmp(ext_lower, ".png") == 0) {
        PNG_Mode(img_src);
    } else if(strcmp(ext_lower, ".gif") == 0) {
        GIF_Mode(img_src);
    } else {
        printf("Error: Unsupported image format: %s\n", ext_lower);
    }
}