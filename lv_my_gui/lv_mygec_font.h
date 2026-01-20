/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --bpp 1 --size 16 --no-compress --stride 1 --align 1 --font AiDianFengYaHeiChangTi(ShangYongMianFei)-2.ttf --symbols 停止播放 正在播放 已暂停 播放下一首 已是最后一首 播放上一首 已是第一首 快进 1秒 视频已结束 音量: 未播放 音量调节 后退一秒⏩⏪◀◀▶▶▶⏸ --range 32-127 --format lvgl -o lv_mygec_font.c
 ******************************************************************************/

#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif



#ifndef LV_MYGEC_FONT
#define LV_MYGEC_FONT 1
#endif

#if LV_MYGEC_FONT

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0x18,

    /* U+0022 "\"" */
    0xb6, 0x80,

    /* U+0023 "#" */
    0x12, 0x12, 0x32, 0x7f, 0x24, 0x24, 0x24, 0x24,
    0xfe, 0x48, 0x48, 0x48, 0x48,

    /* U+0024 "$" */
    0x27, 0xe9, 0x4a, 0x52, 0x9f, 0x29, 0x4a, 0x5f,
    0x90,

    /* U+0025 "%" */
    0x0, 0x66, 0x96, 0x94, 0x6c, 0x8, 0x8, 0x18,
    0x10, 0x10, 0x36, 0x29, 0x69, 0x46, 0x0,

    /* U+0026 "&" */
    0x38, 0x64, 0x44, 0x44, 0x6c, 0x38, 0x31, 0x4b,
    0x4e, 0x46, 0x46, 0x3b, 0x0,

    /* U+0027 "'" */
    0x3a,

    /* U+0028 "(" */
    0x13, 0x22, 0x22, 0x22, 0x22, 0x22, 0x23, 0x10,

    /* U+0029 ")" */
    0x92, 0x64, 0x92, 0x49, 0x29, 0x20,

    /* U+002A "*" */
    0x23, 0x88, 0xa0, 0x0,

    /* U+002B "+" */
    0x21, 0x9, 0xf2, 0x10, 0x80,

    /* U+002C "," */
    0x68,

    /* U+002D "-" */
    0xf0,

    /* U+002E "." */
    0xc0,

    /* U+002F "/" */
    0x8, 0x46, 0x21, 0x18, 0x84, 0x42, 0x31, 0x8,
    0x0,

    /* U+0030 "0" */
    0xfe, 0x18, 0x61, 0x86, 0x18, 0x61, 0x86, 0x18,
    0x61, 0xfc,

    /* U+0031 "1" */
    0x3, 0xc1, 0x4, 0x10, 0x41, 0x4, 0x10, 0x41,
    0x4, 0xfc,

    /* U+0032 "2" */
    0xf8, 0x42, 0x10, 0x87, 0xf0, 0x84, 0x21, 0xf,
    0x80,

    /* U+0033 "3" */
    0xf8, 0x42, 0x10, 0x87, 0xe1, 0x8, 0x42, 0x1f,
    0x80,

    /* U+0034 "4" */
    0xc, 0xc, 0x1c, 0x14, 0x34, 0x24, 0x64, 0x44,
    0xc4, 0x7f, 0x4, 0x4, 0x4,

    /* U+0035 "5" */
    0xfc, 0x21, 0x8, 0x43, 0xe1, 0x8, 0x42, 0x1f,
    0x80,

    /* U+0036 "6" */
    0xfe, 0x8, 0x20, 0x82, 0xf, 0xe1, 0x86, 0x18,
    0x61, 0xfc,

    /* U+0037 "7" */
    0xfc, 0x10, 0x41, 0x4, 0x10, 0x41, 0x4, 0x10,
    0x41, 0x4,

    /* U+0038 "8" */
    0xfe, 0x18, 0x61, 0x86, 0x1f, 0xe1, 0x86, 0x18,
    0x61, 0xfc,

    /* U+0039 "9" */
    0xfc, 0x63, 0x18, 0xc7, 0xe1, 0x8, 0x42, 0x1f,
    0x80,

    /* U+003A ":" */
    0xc6,

    /* U+003B ";" */
    0x51, 0x70,

    /* U+003C "<" */
    0x0, 0x63, 0x18, 0xc1, 0x83, 0x2, 0x0,

    /* U+003D "=" */
    0xf8, 0x0, 0xf, 0x80,

    /* U+003E ">" */
    0x83, 0x6, 0xc, 0xc, 0xc6, 0x30, 0x0,

    /* U+003F "?" */
    0xf8, 0x42, 0x11, 0x98, 0x84, 0x21, 0x0, 0x42,
    0x0,

    /* U+0040 "@" */
    0x3c, 0x3e, 0x62, 0x4a, 0x5a, 0x5e, 0x5e, 0x5e,
    0x40, 0x60, 0x3c, 0x1c,

    /* U+0041 "A" */
    0x30, 0x60, 0xc1, 0xc6, 0x8d, 0x12, 0x26, 0xfd,
    0xa, 0x14, 0x30,

    /* U+0042 "B" */
    0xf4, 0x63, 0x18, 0xc7, 0xd1, 0x8c, 0x63, 0x1f,
    0x0,

    /* U+0043 "C" */
    0x7e, 0x8, 0x20, 0x82, 0x8, 0x20, 0x82, 0x8,
    0x20, 0x7c,

    /* U+0044 "D" */
    0xf4, 0x63, 0x18, 0xc6, 0x31, 0x8c, 0x63, 0x1f,
    0x0,

    /* U+0045 "E" */
    0xfc, 0x21, 0x8, 0x43, 0xf0, 0x84, 0x21, 0xf,
    0x80,

    /* U+0046 "F" */
    0xfc, 0x21, 0x8, 0x43, 0xf0, 0x84, 0x21, 0x8,
    0x0,

    /* U+0047 "G" */
    0xfc, 0x21, 0x8, 0x42, 0xf1, 0x8c, 0x63, 0x1f,
    0x80,

    /* U+0048 "H" */
    0x8c, 0x63, 0x18, 0xc7, 0xf1, 0x8c, 0x63, 0x18,
    0x80,

    /* U+0049 "I" */
    0xff, 0xf8,

    /* U+004A "J" */
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0xe0,

    /* U+004B "K" */
    0x8c, 0xe5, 0x6a, 0x73, 0x94, 0xb4, 0xa7, 0x18,
    0x80,

    /* U+004C "L" */
    0x84, 0x21, 0x8, 0x42, 0x10, 0x84, 0x21, 0xf,
    0x80,

    /* U+004D "M" */
    0x83, 0x8f, 0x1f, 0x7e, 0xf5, 0x6e, 0xc9, 0x83,
    0x6, 0xc, 0x18, 0x20,

    /* U+004E "N" */
    0x87, 0x1c, 0x71, 0xe6, 0x9b, 0x65, 0x9e, 0x38,
    0xe3, 0x84,

    /* U+004F "O" */
    0x74, 0x63, 0x18, 0xc6, 0x31, 0x8c, 0x63, 0x17,
    0x0,

    /* U+0050 "P" */
    0xf4, 0xe3, 0x18, 0xc6, 0x33, 0xf4, 0x21, 0x8,
    0x0,

    /* U+0051 "Q" */
    0x72, 0x28, 0xa2, 0x8a, 0x28, 0xa2, 0x8a, 0x28,
    0xa2, 0x74,

    /* U+0052 "R" */
    0xf4, 0x63, 0x18, 0xc7, 0xd2, 0x94, 0xa7, 0x18,
    0x80,

    /* U+0053 "S" */
    0x7e, 0x8, 0x20, 0x82, 0x7, 0x81, 0x4, 0x10,
    0x41, 0xf8,

    /* U+0054 "T" */
    0xfc, 0x82, 0x8, 0x20, 0x82, 0x8, 0x20, 0x82,
    0x8, 0x20,

    /* U+0055 "U" */
    0x86, 0x18, 0x61, 0x86, 0x18, 0x61, 0x86, 0x18,
    0x61, 0x78,

    /* U+0056 "V" */
    0x85, 0xa, 0x16, 0x24, 0xc9, 0x92, 0x34, 0x78,
    0x70, 0xc1, 0x83, 0x0,

    /* U+0057 "W" */
    0x81, 0x81, 0x81, 0x81, 0x91, 0xdb, 0xdb, 0xda,
    0x7e, 0x66, 0x66, 0x66, 0x42,

    /* U+0058 "X" */
    0x86, 0x3c, 0x92, 0x78, 0xc3, 0xc, 0x79, 0x2c,
    0xa1, 0x84,

    /* U+0059 "Y" */
    0xc6, 0x89, 0xb1, 0x42, 0x82, 0x4, 0x8, 0x10,
    0x20, 0x40, 0x80,

    /* U+005A "Z" */
    0xfc, 0x30, 0x86, 0x18, 0x43, 0x8, 0x61, 0x84,
    0x30, 0xfc,

    /* U+005B "[" */
    0xf2, 0x49, 0x24, 0x92, 0x49, 0xc0,

    /* U+005C "\\" */
    0x84, 0x10, 0x84, 0x10, 0x86, 0x10, 0x86, 0x10,
    0x80,

    /* U+005D "]" */
    0xe4, 0x92, 0x49, 0x24, 0x93, 0xc0,

    /* U+005E "^" */
    0x3, 0x35, 0x0,

    /* U+005F "_" */
    0xf0,

    /* U+0060 "`" */
    0x29,

    /* U+0061 "a" */
    0xf8, 0x10, 0x41, 0x7e, 0x18, 0x61, 0x7c,

    /* U+0062 "b" */
    0x84, 0x21, 0x8, 0x7a, 0x71, 0x8c, 0x63, 0x3f,
    0x0,

    /* U+0063 "c" */
    0x3d, 0x8, 0x20, 0x82, 0x8, 0x10, 0x7c,

    /* U+0064 "d" */
    0x8, 0x42, 0x10, 0xbf, 0x31, 0x8c, 0x63, 0x97,
    0x80,

    /* U+0065 "e" */
    0x76, 0xe3, 0x1f, 0xc2, 0x18, 0x78,

    /* U+0066 "f" */
    0x8, 0x88, 0x4f, 0x90, 0x84, 0x21, 0x8, 0x42,
    0x0,

    /* U+0067 "g" */
    0x7c, 0x63, 0x18, 0xc5, 0xe1, 0x8, 0x7c,

    /* U+0068 "h" */
    0x84, 0x21, 0xf, 0x46, 0x31, 0x8c, 0x63, 0x18,
    0x80,

    /* U+0069 "i" */
    0xef, 0xf8,

    /* U+006A "j" */
    0x24, 0x82, 0x49, 0x24, 0x92, 0xf0,

    /* U+006B "k" */
    0x84, 0x21, 0x8, 0xce, 0xdc, 0xe5, 0xa5, 0x38,
    0x80,

    /* U+006C "l" */
    0xff, 0xf8,

    /* U+006D "m" */
    0xfd, 0x26, 0x4c, 0x99, 0x32, 0x64, 0xc9, 0x92,

    /* U+006E "n" */
    0xfa, 0x18, 0x61, 0x86, 0x18, 0x61, 0x84,

    /* U+006F "o" */
    0x74, 0x63, 0x18, 0xc6, 0x31, 0x70,

    /* U+0070 "p" */
    0xfa, 0x38, 0x61, 0x86, 0x3f, 0xa0, 0x82, 0x8,
    0x0,

    /* U+0071 "q" */
    0x7f, 0x18, 0x61, 0x87, 0x17, 0xc1, 0x4, 0x10,
    0x40,

    /* U+0072 "r" */
    0x8d, 0xa9, 0x8c, 0x42, 0x10, 0x80,

    /* U+0073 "s" */
    0x7c, 0x21, 0x7, 0x4, 0x21, 0xf0,

    /* U+0074 "t" */
    0x20, 0x8f, 0xc8, 0x20, 0x82, 0x8, 0x20, 0x40,
    0xc0,

    /* U+0075 "u" */
    0x86, 0x18, 0x61, 0x86, 0x18, 0x61, 0x78,

    /* U+0076 "v" */
    0x85, 0xb, 0x12, 0x64, 0x8d, 0xe, 0x18, 0x30,

    /* U+0077 "w" */
    0x81, 0x81, 0x81, 0x9b, 0xdb, 0x7e, 0x66, 0x66,
    0x42,

    /* U+0078 "x" */
    0x87, 0x25, 0x9c, 0x31, 0xc5, 0xa3, 0x80,

    /* U+0079 "y" */
    0x86, 0x3c, 0x92, 0x78, 0xc3, 0xc, 0x21, 0x84,
    0x0,

    /* U+007A "z" */
    0xfc, 0x31, 0x84, 0x30, 0x86, 0x30, 0xfc,

    /* U+007B "{" */
    0x34, 0x44, 0x44, 0x48, 0x44, 0x44, 0x43,

    /* U+007C "|" */
    0xff, 0xfc,

    /* U+007D "}" */
    0xc1, 0x8, 0x42, 0x10, 0x83, 0x21, 0x8, 0x42,
    0x60,

    /* U+4E00 "一" */
    0xff, 0xc0,

    /* U+4E0A "上" */
    0x18, 0x6, 0x1, 0x80, 0x7e, 0x18, 0x6, 0x1,
    0x80, 0x60, 0x18, 0x6, 0x1, 0x80, 0x60, 0x18,
    0x3f, 0xf0,

    /* U+4E0B "下" */
    0xff, 0xc2, 0x0, 0x80, 0x20, 0x8, 0x2, 0x0,
    0xa0, 0x26, 0x8, 0x82, 0x0, 0x80, 0x20, 0x8,
    0x2, 0x0,

    /* U+505C "停" */
    0x2, 0x37, 0xfc, 0x1, 0x7f, 0x58, 0x57, 0xf4,
    0x1, 0x7f, 0x50, 0x50, 0x4, 0xf9, 0x8, 0x42,
    0x10, 0x80,

    /* U+540E "后" */
    0x0, 0x1f, 0xf4, 0x1, 0x0, 0x40, 0x1f, 0xf4,
    0x1, 0x0, 0x40, 0x17, 0xf5, 0x5, 0x41, 0x50,
    0x54, 0x15, 0xfc,

    /* U+5728 "在" */
    0x10, 0x3f, 0xf3, 0x0, 0x80, 0x22, 0x18, 0x8c,
    0x23, 0x7f, 0x42, 0x10, 0x84, 0x21, 0x8, 0x42,
    0x17, 0xf0,

    /* U+5DF2 "已" */
    0xff, 0x80, 0x40, 0x20, 0x10, 0xc, 0x7, 0xff,
    0x0, 0x80, 0x40, 0x20, 0x10, 0x8, 0x7, 0xfc,

    /* U+5FEB "快" */
    0x46, 0x37, 0xec, 0x6b, 0x1a, 0xe6, 0xb1, 0xac,
    0x6b, 0x7f, 0xc4, 0x31, 0x4c, 0xdb, 0x22, 0x58,
    0xd4, 0x10,

    /* U+64AD "播" */
    0x5f, 0xa0, 0x3e, 0xe8, 0x45, 0xfa, 0x59, 0x4a,
    0xc0, 0xdf, 0xa9, 0x54, 0xaa, 0xf5, 0x2e, 0xfc,

    /* U+653E "放" */
    0x22, 0x3e, 0xf4, 0x5, 0x1, 0x42, 0x5e, 0x94,
    0xa5, 0x29, 0x4b, 0xd2, 0x64, 0x99, 0x26, 0xcb,
    0xe6, 0x90,

    /* U+662F "是" */
    0xff, 0xa0, 0x2b, 0xfa, 0x2, 0x80, 0xbf, 0xe0,
    0x3, 0xff, 0x8, 0x2, 0x8, 0xfa, 0x20, 0xe8,
    0x2f, 0xf0,

    /* U+6682 "暂" */
    0xc0, 0x63, 0xfd, 0x10, 0x8a, 0x47, 0xbc, 0xb3,
    0xf9, 0x2c, 0x80, 0x3f, 0xf0, 0x1b, 0xfc, 0x7,
    0xfe,

    /* U+6700 "最" */
    0x7f, 0x90, 0x27, 0xf9, 0x2, 0x7f, 0x80, 0xf,
    0xfc, 0x60, 0xff, 0xe6, 0x3f, 0xea, 0x6e, 0xfb,
    0x87, 0xb0,

    /* U+672A "未" */
    0xc, 0x1f, 0xe0, 0xc0, 0x30, 0xc, 0x3f, 0xf0,
    0xc1, 0x32, 0x4c, 0x93, 0x24, 0xc9, 0x32, 0x8c,
    0xe3, 0x10,

    /* U+675F "束" */
    0x8, 0x7f, 0xc2, 0x1, 0xf, 0xfc, 0x46, 0x23,
    0x11, 0xff, 0x84, 0x12, 0x49, 0x28, 0x8c, 0x44,

    /* U+6B62 "止" */
    0x4, 0x1, 0x4, 0x41, 0x1e, 0x44, 0x11, 0x4,
    0x41, 0x10, 0x44, 0x11, 0x4, 0x41, 0x10, 0x44,
    0x3f, 0xf0,

    /* U+6B63 "正" */
    0xff, 0x82, 0x0, 0x80, 0x20, 0x8, 0x12, 0x4,
    0xf9, 0x20, 0x48, 0x12, 0x4, 0x81, 0x20, 0x48,
    0x3f, 0xf0,

    /* U+79D2 "秒" */
    0xf2, 0x1a, 0xa6, 0xa9, 0xaa, 0xfa, 0x9a, 0xaf,
    0xab, 0xea, 0xf2, 0x3c, 0xaf, 0xb, 0xc4, 0xf6,
    0x3f, 0x0,

    /* U+7B2C "第" */
    0x84, 0x3d, 0xf4, 0x20, 0xc2, 0xff, 0x82, 0x2f,
    0xfb, 0xfe, 0x88, 0x3f, 0xf6, 0x8d, 0x23, 0x48,
    0xe2, 0x70,

    /* U+7ED3 "结" */
    0x62, 0x13, 0xf4, 0x22, 0x8, 0xe2, 0x8, 0x84,
    0xfd, 0x0, 0x80, 0x3b, 0xf0, 0x84, 0x21, 0x8,
    0x7b, 0xf0,

    /* U+8282 "节" */
    0x41, 0x3f, 0xf4, 0x10, 0x0, 0x0, 0x0, 0xf,
    0xfc, 0x43, 0x10, 0xc4, 0x31, 0xc, 0x43, 0x11,
    0xc4, 0x0,

    /* U+89C6 "视" */
    0x4f, 0xcd, 0x8, 0xa5, 0x14, 0xa2, 0x94, 0x92,
    0xb2, 0x53, 0x4a, 0x6a, 0x4c, 0x51, 0x9a, 0x3a,
    0x44, 0xc8, 0xb1, 0xc0,

    /* U+8C03 "调" */
    0xdf, 0xd4, 0x15, 0x24, 0x5d, 0x17, 0x74, 0x95,
    0x75, 0x41, 0x57, 0x55, 0x55, 0x55, 0x5d, 0x50,
    0x5c, 0x30,

    /* U+8FDB "进" */
    0x88, 0x92, 0x25, 0xfc, 0x22, 0xc8, 0x92, 0x24,
    0x89, 0x7f, 0x48, 0x92, 0x24, 0x89, 0x42, 0x40,
    0x1f, 0xf0,

    /* U+9000 "退" */
    0x9f, 0xd4, 0x15, 0x4, 0x5f, 0xd0, 0x57, 0xf5,
    0x1, 0x4b, 0x53, 0x94, 0x65, 0x8d, 0x0, 0x40,
    0x1f, 0xf0,

    /* U+91CF "量" */
    0xff, 0xa0, 0x2f, 0xfb, 0xfe, 0x0, 0x3f, 0xff,
    0xfa, 0x22, 0xbf, 0xa2, 0x27, 0xf3, 0xff, 0x8,
    0x3f, 0xf0,

    /* U+97F3 "音" */
    0x8, 0x3f, 0xf0, 0x1, 0x2, 0x41, 0xbf, 0xf0,
    0x0, 0x0, 0x7f, 0x90, 0x27, 0xf9, 0x2, 0x40,
    0x9f, 0xe0,

    /* U+9891 "频" */
    0x27, 0xfe, 0x4e, 0x13, 0x84, 0xe3, 0xfe, 0x92,
    0x37, 0xad, 0xeb, 0x7a, 0xda, 0xb4, 0x6a, 0x32,
    0xf9, 0x10,

    /* U+9996 "首" */
    0x40, 0x80, 0x2f, 0xfc, 0x20, 0x8, 0x3f, 0xe8,
    0xa, 0x2, 0xbf, 0xa0, 0x2b, 0xfa, 0x2, 0x80,
    0xbf, 0xe0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 130, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 39, .box_w = 1, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 75, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 10},
    {.bitmap_index = 5, .adv_w = 146, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 18, .adv_w = 113, .box_w = 5, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 27, .adv_w = 145, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 42, .adv_w = 136, .box_w = 8, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 55, .adv_w = 56, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = 10},
    {.bitmap_index = 56, .adv_w = 59, .box_w = 4, .box_h = 15, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 64, .adv_w = 59, .box_w = 3, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 70, .adv_w = 90, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 8},
    {.bitmap_index = 74, .adv_w = 106, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 79, .adv_w = 56, .box_w = 2, .box_h = 3, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 80, .adv_w = 89, .box_w = 4, .box_h = 1, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 81, .adv_w = 39, .box_w = 1, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 82, .adv_w = 114, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 91, .adv_w = 113, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 101, .adv_w = 114, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 111, .adv_w = 113, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 120, .adv_w = 109, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 129, .adv_w = 132, .box_w = 8, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 142, .adv_w = 112, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 151, .adv_w = 111, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 161, .adv_w = 112, .box_w = 6, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 171, .adv_w = 110, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 181, .adv_w = 109, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 190, .adv_w = 39, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 191, .adv_w = 53, .box_w = 2, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 193, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 200, .adv_w = 101, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 204, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 211, .adv_w = 109, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 220, .adv_w = 129, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 232, .adv_w = 133, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 243, .adv_w = 111, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 252, .adv_w = 112, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 262, .adv_w = 109, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 271, .adv_w = 110, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 280, .adv_w = 108, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 289, .adv_w = 110, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 298, .adv_w = 110, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 307, .adv_w = 40, .box_w = 1, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 309, .adv_w = 92, .box_w = 4, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 316, .adv_w = 111, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 325, .adv_w = 108, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 334, .adv_w = 131, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 346, .adv_w = 112, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 356, .adv_w = 110, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 365, .adv_w = 110, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 374, .adv_w = 124, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 384, .adv_w = 109, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 393, .adv_w = 112, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 403, .adv_w = 113, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 413, .adv_w = 111, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 423, .adv_w = 133, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 435, .adv_w = 158, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 448, .adv_w = 125, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 458, .adv_w = 132, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 469, .adv_w = 114, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 479, .adv_w = 78, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 485, .adv_w = 114, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 494, .adv_w = 78, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 500, .adv_w = 102, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 10},
    {.bitmap_index = 503, .adv_w = 83, .box_w = 4, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 504, .adv_w = 56, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = 10},
    {.bitmap_index = 505, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 512, .adv_w = 110, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 521, .adv_w = 113, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 528, .adv_w = 110, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 537, .adv_w = 110, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 543, .adv_w = 106, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 552, .adv_w = 111, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 559, .adv_w = 110, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 568, .adv_w = 40, .box_w = 1, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 570, .adv_w = 69, .box_w = 3, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 576, .adv_w = 112, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 585, .adv_w = 40, .box_w = 1, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 587, .adv_w = 135, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 595, .adv_w = 113, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 602, .adv_w = 110, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 608, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 617, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 626, .adv_w = 104, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 632, .adv_w = 110, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 638, .adv_w = 109, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 647, .adv_w = 111, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 654, .adv_w = 132, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 662, .adv_w = 158, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 671, .adv_w = 121, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 678, .adv_w = 127, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 687, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 694, .adv_w = 89, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 701, .adv_w = 39, .box_w = 1, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 703, .adv_w = 89, .box_w = 5, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 712, .adv_w = 179, .box_w = 10, .box_h = 1, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 714, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 732, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 750, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 768, .adv_w = 179, .box_w = 10, .box_h = 15, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 787, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 805, .adv_w = 179, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 821, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 839, .adv_w = 179, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 855, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 873, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 891, .adv_w = 179, .box_w = 9, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 908, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 926, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 944, .adv_w = 179, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 960, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 978, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 996, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1014, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1032, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1050, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1068, .adv_w = 179, .box_w = 11, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1088, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1106, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1124, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1142, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1160, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1178, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1196, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0xa, 0xb, 0x25c, 0x60e, 0x928, 0xff2, 0x11eb,
    0x16ad, 0x173e, 0x182f, 0x1882, 0x1900, 0x192a, 0x195f, 0x1d62,
    0x1d63, 0x2bd2, 0x2d2c, 0x30d3, 0x3482, 0x3bc6, 0x3e03, 0x41db,
    0x4200, 0x43cf, 0x49f3, 0x4a91, 0x4b96
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 94, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 19968, .range_length = 19351, .glyph_id_start = 95,
        .unicode_list = unicode_list_1, .glyph_id_ofs_list = NULL, .list_length = 29, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif

};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t lv_mygec_font = {
#else
lv_font_t lv_mygec_font = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 16,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if LV_MYGEC_FONT*/
