#include "lvgl_music_gui.h"

static lv_obj_t *music_screen;
static lv_obj_t *label_song_name;
static lv_obj_t *label_song_index;
static lv_obj_t *btn_play_stop;
static lv_obj_t *btn_pause_resume;
static lv_obj_t *btn_prev;
static lv_obj_t *btn_next;
static lv_obj_t *label_play_stop;
static lv_obj_t *label_pause_resume;
static lv_obj_t *label_lyrics;  // 歌词标签

static bool is_playing = false;
static bool is_paused = false;

static void update_music_display(void) {
    const char *song_name = get_current_song_name();
    int index = get_current_music_index();
    int total = get_total_music_count();
    const char *lyrics = get_current_lyrics();
    
    // 直接更新歌曲名称
    if(label_song_name != NULL) {
        lv_label_set_text(label_song_name, song_name);
    }
    
    // 直接更新索引
    if(label_song_index != NULL) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d / %d", index + 1, total);
        lv_label_set_text(label_song_index, buf);
    }
    
    // 更新歌词
    if(label_lyrics != NULL) {
        lv_label_set_text(label_lyrics, lyrics);
    }
}

/* 播放/终止按钮回调 */
static void play_stop_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if(!is_playing) {
            play_music();
            is_playing = true;
            is_paused = false;
            lv_obj_set_style_bg_img_src(btn_play_stop, "A:Stop_ON.png", 0);
            lv_obj_set_style_bg_img_src(btn_pause_resume, "A:Pause_ON.png", 0);
            update_music_display(); // 更新显示
        } else {
            end_music_mode();
            is_playing = false;
            is_paused = false;
            lv_obj_set_style_bg_img_src(btn_play_stop, "A:Play_OFF.png", 0);
            lv_obj_set_style_bg_img_src(btn_pause_resume, "A:Pause_OFF.png", 0);
        }
    }
}

/* 暂停/恢复按钮回调 */
static void pause_resume_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if(!is_playing) return;
        
        if(!is_paused) {
            stop_music();
            is_paused = true;
            lv_obj_set_style_bg_img_src(btn_pause_resume, "A:Resume_ON.png", 0);
        } else {
            continue_music_mode();
            is_paused = false;
            lv_obj_set_style_bg_img_src(btn_pause_resume, "A:Pause_ON.png", 0);
        }
    }
}

/* 上一首按钮回调 */
static void prev_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        previous_music();
        is_playing = true;
        is_paused = false;
        lv_obj_set_style_bg_img_src(btn_play_stop, "A:Stop_ON.png", 0);
        lv_obj_set_style_bg_img_src(btn_pause_resume, "A:Pause_ON.png", 0);
        update_music_display(); // 更新显示
    }
}

/* 下一首按钮回调 */
static void next_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        next_music();
        is_playing = true;
        is_paused = false;
        lv_obj_set_style_bg_img_src(btn_play_stop, "A:Stop_ON.png", 0);
        lv_obj_set_style_bg_img_src(btn_pause_resume, "A:Pause_ON.png", 0);
        update_music_display(); // 更新显示
    }
}

void music_gui_update_info(const char* song_name, int index, int total)
{
    if(label_song_name != NULL) {
        lv_label_set_text(label_song_name, song_name);
    }
    
    if(label_song_index != NULL) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d / %d", index + 1, total);
        lv_label_set_text(label_song_index, buf);
    }
}

void music_gui_set_play_state(bool playing)
{
    is_playing = playing;
    if(btn_play_stop != NULL) {
        if(is_playing) {
            lv_obj_set_style_bg_img_src(btn_play_stop, "A:Stop_ON.png", 0);
        } else {
            lv_obj_set_style_bg_img_src(btn_play_stop, "A:Play_OFF.png", 0);
        }
    }
}

void music_gui_set_pause_state(bool paused)
{
    is_paused = paused;
    if(btn_pause_resume != NULL) {
        if(is_paused) {
            lv_obj_set_style_bg_img_src(btn_pause_resume, "A:Resume_ON.png", 0);
        } else {
            lv_obj_set_style_bg_img_src(btn_pause_resume, "A:Pause_ON.png", 0);
        }
    }
}

void music_gui_init(lv_style_t* style)
{
    /* 创建音乐界面容器 */
    music_screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(music_screen, LV_HOR_RES, LV_VER_RES);
    lv_obj_center(music_screen);

    /* 标题 */
    lv_obj_t *label_title = lv_label_create(music_screen);
    lv_label_set_text(label_title, "音乐播放器");
    lv_obj_add_style(label_title, style, 0);
    lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 10);

    /* 音乐信息显示区域 */
    lv_obj_t *info_container = lv_obj_create(music_screen);
    lv_obj_set_size(info_container, 700, 80);
    lv_obj_align(info_container, LV_ALIGN_TOP_MID, 0, 50);
    lv_obj_set_style_bg_color(info_container, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_bg_opa(info_container, LV_OPA_20, 0);
    lv_obj_set_style_border_width(info_container, 2, 0);
    lv_obj_set_style_border_color(info_container, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_radius(info_container, 10, 0);

    /* 歌曲名称 - 使用中文字体 */
    label_song_name = lv_label_create(info_container);
    lv_label_set_text(label_song_name, "暂无播放");
    if(style != NULL) {
        lv_obj_add_style(label_song_name, style, 0);
    }
    lv_label_set_long_mode(label_song_name, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(label_song_name, 650);
    lv_obj_align(label_song_name, LV_ALIGN_CENTER, 0, -15);

    /* 歌曲索引 */
    label_song_index = lv_label_create(info_container);
    lv_label_set_text(label_song_index, "-- / --");
    if(style != NULL) {
        lv_obj_add_style(label_song_index, style, 0);
    }
    lv_obj_align(label_song_index, LV_ALIGN_CENTER, 0, 15);

    /* 歌词显示区域 */
    lv_obj_t *lyrics_container = lv_obj_create(music_screen);
    lv_obj_set_size(lyrics_container, 700, 120);
    lv_obj_align(lyrics_container, LV_ALIGN_TOP_MID, 0, 145);
    lv_obj_set_style_bg_color(lyrics_container, lv_palette_main(LV_PALETTE_PURPLE), 0);
    lv_obj_set_style_bg_opa(lyrics_container, LV_OPA_10, 0);
    lv_obj_set_style_border_width(lyrics_container, 2, 0);
    lv_obj_set_style_border_color(lyrics_container, lv_palette_main(LV_PALETTE_PURPLE), 0);
    lv_obj_set_style_radius(lyrics_container, 10, 0);

    /* 歌词标签 */
    label_lyrics = lv_label_create(lyrics_container);
    lv_label_set_text(label_lyrics, "♪ 歌词将在这里显示 ♪");
    if(style != NULL) {
        lv_obj_add_style(label_lyrics, style, 0);
    }
    lv_label_set_long_mode(label_lyrics, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(label_lyrics, 650);
    lv_obj_center(label_lyrics);

    /* 控制按钮容器 */
    lv_obj_t *ctrl_container = lv_obj_create(music_screen);
    lv_obj_set_size(ctrl_container, 700, 250);
    lv_obj_align(ctrl_container, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_style_bg_opa(ctrl_container, LV_OPA_0, 0);
    lv_obj_set_style_border_width(ctrl_container, 0, 0);
    lv_obj_set_style_pad_all(ctrl_container, 0, 0);

    /* 播放/终止按钮 (中心位置) */
    btn_play_stop = lv_btn_create(ctrl_container);
    lv_obj_set_size(btn_play_stop, 128, 128);
    lv_obj_align(btn_play_stop, LV_ALIGN_CENTER, 0, -40);
    lv_obj_add_event_cb(btn_play_stop, play_stop_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_img_src(btn_play_stop, "A:Play_OFF.png", 0);
    lv_obj_set_style_bg_color(btn_play_stop, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(btn_play_stop, LV_OPA_0, 0);
    lv_obj_set_style_border_width(btn_play_stop, 0, 0);
    lv_obj_set_style_shadow_width(btn_play_stop, 0, 0);
    lv_obj_set_style_radius(btn_play_stop, 0, 0);
    lv_obj_set_style_pad_all(btn_play_stop, 0, 0);

    /* 上一首按钮 (播放按钮左侧) */
    btn_prev = lv_btn_create(ctrl_container);
    lv_obj_set_size(btn_prev, 128,128);
    lv_obj_align_to(btn_prev, btn_play_stop, LV_ALIGN_OUT_LEFT_MID, -40, 0);
    lv_obj_add_event_cb(btn_prev, prev_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_img_src(btn_prev, "A:Prev_OFF.png", 0);
    lv_obj_set_style_bg_color(btn_prev, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(btn_prev, LV_OPA_0, 0);
    lv_obj_set_style_border_width(btn_prev, 0, 0);
    lv_obj_set_style_shadow_width(btn_prev, 0, 0);
    lv_obj_set_style_radius(btn_prev, 0, 0);
    lv_obj_set_style_pad_all(btn_prev, 0, 0);

    /* 下一首按钮 (播放按钮右侧) */
    btn_next = lv_btn_create(ctrl_container);
    lv_obj_set_size(btn_next, 128, 128);
    lv_obj_align_to(btn_next, btn_play_stop, LV_ALIGN_OUT_RIGHT_MID, 40, 0);
    lv_obj_add_event_cb(btn_next, next_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_img_src(btn_next, "A:Next_OFF.png", 0);
    lv_obj_set_style_bg_color(btn_next, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(btn_next, LV_OPA_0, 0);
    lv_obj_set_style_border_width(btn_next, 0, 0);
    lv_obj_set_style_shadow_width(btn_next, 0, 0);
    lv_obj_set_style_radius(btn_next, 0, 0);
    lv_obj_set_style_pad_all(btn_next, 0, 0);

    /* 暂停/恢复按钮 (播放按钮下方) */
    btn_pause_resume = lv_btn_create(ctrl_container);
    lv_obj_set_size(btn_pause_resume, 128, 128);
    lv_obj_align(btn_pause_resume, LV_ALIGN_CENTER, 0, 65);
    lv_obj_add_event_cb(btn_pause_resume, pause_resume_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_img_src(btn_pause_resume, "A:Pause_OFF.png", 0);
    lv_obj_set_style_bg_color(btn_pause_resume, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(btn_pause_resume, LV_OPA_0, 0);
    lv_obj_set_style_border_width(btn_pause_resume, 0, 0);
    lv_obj_set_style_shadow_width(btn_pause_resume, 0, 0);
    lv_obj_set_style_radius(btn_pause_resume, 0, 0);
    lv_obj_set_style_pad_all(btn_pause_resume, 0, 0);
    
    // 初始化显示当前歌曲信息
    update_music_display();
}