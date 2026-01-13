#ifndef _FREETYPE_FONT_INIT_H
#define _FREETYPE_FONT_INIT_H
#include "common.h"

lv_ft_info_t  info;
lv_style_t  style;

void font_init(char *font_path, int font_size, lv_style_t *font_style);

#endif