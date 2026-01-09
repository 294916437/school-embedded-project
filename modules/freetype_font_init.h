#ifndef _FREETYPE_INIT_H
#define _FREETYPE_INIT_H
#include "./modules/lvgl_mode_gui.h"


lv_ft_info_t info;
lv_style_t style;

void font_init(char * fontPathName, int fontSize,lv_style_t * style);

#endif  /*_FREETYPE_INIT_H*/