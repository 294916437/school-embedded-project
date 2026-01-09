#include "freetype_font_init.h"

void font_init(char * fontPathName, int fontSize,lv_style_t * style){
    /*FreeType uses C standard file system, so no driver letter is required.*/
    info.name = fontPathName;
    info.weight = fontSize;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;
    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }
    /*Create style with the new font*/
    lv_style_init(&style);
    lv_style_set_text_font(&style, info.font);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);
}