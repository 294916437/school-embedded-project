#include "freetype_font_init.h"

void font_init(char *font_path, int font_size, lv_style_t *font_style)
{
    info.name = font_path;
    info.weight = font_size;
    info.style = font_style;
    info.mem = NULL;
    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }

    lv_style_init(font_style);
    lv_style_set_text_font(font_style, info.font);
    lv_style_set_text_align(font_style, LV_TEXT_ALIGN_CENTER);
}
