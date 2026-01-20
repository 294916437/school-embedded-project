/*
*---------------------------------------------------------------
*                        Lvgl Font Tool                         
*                                                               
* 注:使用unicode编码                                              
* 注:本字体文件由Lvgl Font Tool V0.5测试版 生成                     
* 作者:阿里(qq:617622104)                                         
*---------------------------------------------------------------
*/


#include "lvgl.h"


static const uint8_t glyph_bitmap[] = {
/* 器 */
0x0f,0xff,0xff,0x0f,0xff,0xff,0x00,  //.@@@@@.@@@@@.
0x0f,0x00,0x0f,0x0f,0x00,0x0f,0x00,  //.@...@.@...@.
0x0f,0x00,0x0f,0x0f,0x00,0x0f,0x00,  //.@...@.@...@.
0x0f,0xff,0xff,0x0f,0xff,0xff,0x00,  //.@@@@@.@@@@@.
0x00,0x00,0x00,0x00,0x0f,0x00,0x00,  //.........@...
0x00,0x00,0x00,0xf0,0x00,0xf0,0x00,  //......@...@..
0xff,0xff,0xff,0xff,0xff,0xff,0xf0,  //@@@@@@@@@@@@@
0x00,0x00,0xf0,0x00,0xf0,0x00,0x00,  //....@...@....
0x00,0xff,0x00,0x00,0x0f,0xf0,0x00,  //..@@.....@@..
0xff,0x00,0x00,0x00,0x00,0x0f,0xf0,  //@@.........@@
0x0f,0xff,0xff,0x0f,0xff,0xff,0x00,  //.@@@@@.@@@@@.
0x0f,0x00,0x0f,0x0f,0x00,0x0f,0x00,  //.@...@.@...@.
0x0f,0x00,0x0f,0x0f,0x00,0x0f,0x00,  //.@...@.@...@.
0x0f,0xff,0xff,0x0f,0xff,0xff,0x00,  //.@@@@@.@@@@@.


/* 算 */
0x00,0xf0,0x00,0x00,0xf0,0x00,0x00,  //..@.....@....
0x0f,0xff,0xff,0xf0,0xff,0xff,0xf0,  //.@@@@@@.@@@@@
0xf0,0x00,0xf0,0x0f,0x00,0xf0,0x00,  //@...@..@..@..
0x00,0xff,0xff,0xff,0xff,0xf0,0x00,  //..@@@@@@@@@..
0x00,0xf0,0x00,0x00,0x00,0xf0,0x00,  //..@.......@..
0x00,0xff,0xff,0xff,0xff,0xf0,0x00,  //..@@@@@@@@@..
0x00,0xf0,0x00,0x00,0x00,0xf0,0x00,  //..@.......@..
0x00,0xff,0xff,0xff,0xff,0xf0,0x00,  //..@@@@@@@@@..
0x00,0xf0,0x00,0x00,0x00,0xf0,0x00,  //..@.......@..
0x00,0xff,0xff,0xff,0xff,0xf0,0x00,  //..@@@@@@@@@..
0x00,0x00,0xf0,0x00,0xf0,0x00,0x00,  //....@...@....
0xff,0xff,0xff,0xff,0xff,0xff,0xf0,  //@@@@@@@@@@@@@
0x00,0x0f,0x00,0x00,0xf0,0x00,0x00,  //...@....@....
0x00,0xf0,0x00,0x00,0xf0,0x00,0x00,  //..@.....@....


/* 计 */
0x0f,0x00,0x00,0x00,0xf0,0x00,0x00,  //.@......@....
0x00,0xf0,0x00,0x00,0xf0,0x00,0x00,  //..@.....@....
0x00,0xf0,0x00,0x00,0xf0,0x00,0x00,  //..@.....@....
0x00,0x00,0x00,0x00,0xf0,0x00,0x00,  //........@....
0x00,0x00,0x00,0x00,0xf0,0x00,0x00,  //........@....
0xff,0xf0,0xff,0xff,0xff,0xff,0xf0,  //@@@.@@@@@@@@@
0x00,0xf0,0x00,0x00,0xf0,0x00,0x00,  //..@.....@....
0x00,0xf0,0x00,0x00,0xf0,0x00,0x00,  //..@.....@....
0x00,0xf0,0x00,0x00,0xf0,0x00,0x00,  //..@.....@....
0x00,0xf0,0x00,0x00,0xf0,0x00,0x00,  //..@.....@....
0x00,0xf0,0xf0,0x00,0xf0,0x00,0x00,  //..@.@...@....
0x00,0xff,0x00,0x00,0xf0,0x00,0x00,  //..@@....@....
0x00,0xf0,0x00,0x00,0xf0,0x00,0x00,  //..@.....@....
0x00,0x00,0x00,0x00,0xf0,0x00,0x00,  //........@....


};


static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0,	.adv_w = 15, .box_h = 14, .box_w = 14, .ofs_x = 1, .ofs_y = 1},/*(器)*/
    {.bitmap_index = 98,	.adv_w = 15, .box_h = 14, .box_w = 14, .ofs_x = 1, .ofs_y = 1},/*(算)*/
    {.bitmap_index = 196,	.adv_w = 15, .box_h = 14, .box_w = 14, .ofs_x = 1, .ofs_y = 1},/*(计)*/
};


static const uint16_t unicode_list_1[] = {
    0x5668,	/*(器)*/
    0x7b97,	/*(算)*/
    0x8ba1,	/*(计)*/
    0x0000,    /*End indicator*/
};


static const lv_font_fmt_txt_cmap_t cmaps[] = {
    {
        .range_start = 0x5668,
        .range_length = 0x8ba1,
        .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY,
        .glyph_id_start = 0,
        .unicode_list = unicode_list_1,
        .glyph_id_ofs_list = NULL,
        .list_length = 3,
    }
};


static lv_font_fmt_txt_glyph_cache_t glyph_cache;


static lv_font_fmt_txt_dsc_t font_dsc = {
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .cmap_num = 1,
    .bpp = 4,

    .kern_scale = 0,
    .kern_dsc = NULL,
    .kern_classes = 0,

    .cache = &glyph_cache,
};


static int binsearch(const uint16_t *sortedSeq, int seqLength, uint16_t keyData) {
    int low = 0, mid, high = seqLength - 1;
    while (low <= high) {
        mid = (low + high)>>1;//右移1位等于是/2，奇数，无论奇偶，有个值就行
        if (keyData < sortedSeq[mid]) {
            high = mid - 1;//是mid-1，因为mid已经比较过了
        }
        else if (keyData > sortedSeq[mid]) {
            low = mid + 1;
        }
        else {
            return mid;
        }
    }
    return -1;
}


static const uint8_t * __user_font_get_bitmap(const lv_font_t * font, uint32_t unicode_letter) {
    lv_font_fmt_txt_dsc_t * fdsc = (lv_font_fmt_txt_dsc_t *) font->dsc;

    if( unicode_letter<fdsc->cmaps[0].range_start || unicode_letter>fdsc->cmaps[0].range_length ) return false;

    int i;
    if( unicode_letter==fdsc->cache->last_letter ){
        i = fdsc->cache->last_glyph_id;
    }
    else{
        i = binsearch(fdsc->cmaps[0].unicode_list, fdsc->cmaps[0].list_length, unicode_letter);
    }
    if( i != -1 ) {
        const lv_font_fmt_txt_glyph_dsc_t * gdsc = &fdsc->glyph_dsc[i];
        fdsc->cache->last_glyph_id = i;
        fdsc->cache->last_letter = unicode_letter;
        return &fdsc->glyph_bitmap[gdsc->bitmap_index];
    }
    return NULL;
}


static bool __user_font_get_glyph_dsc(const lv_font_t * font, lv_font_glyph_dsc_t * dsc_out, uint32_t unicode_letter, uint32_t unicode_letter_next) {
    lv_font_fmt_txt_dsc_t * fdsc = (lv_font_fmt_txt_dsc_t *) font->dsc;

    if( unicode_letter<fdsc->cmaps[0].range_start || unicode_letter>fdsc->cmaps[0].range_length ) return false;

    int i;
    if( unicode_letter==fdsc->cache->last_letter ){
        i = fdsc->cache->last_glyph_id;
    }
    else{
        i = binsearch(fdsc->cmaps[0].unicode_list, fdsc->cmaps[0].list_length, unicode_letter);
    }
    if( i != -1 ) {
        const lv_font_fmt_txt_glyph_dsc_t * gdsc = &fdsc->glyph_dsc[i];
        fdsc->cache->last_glyph_id = i;
        fdsc->cache->last_letter = unicode_letter;
        dsc_out->adv_w = gdsc->adv_w;
        dsc_out->box_h = gdsc->box_h;
        dsc_out->box_w = gdsc->box_w;
        dsc_out->ofs_x = gdsc->ofs_x;
        dsc_out->ofs_y = gdsc->ofs_y;
        dsc_out->bpp   = fdsc->bpp;
        return true;
    }
    return false;
}


//宋体,常规,9
//字模高度：15
//内部字体
//使用排序和二分查表
const lv_font_t calculator_text = {
    .dsc = &font_dsc,
    .get_glyph_bitmap = __user_font_get_bitmap,
    .get_glyph_dsc = __user_font_get_glyph_dsc,
    .line_height = 15,
    .base_line = 0,
};

