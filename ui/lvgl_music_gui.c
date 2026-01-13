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
            lv_label_set_text(label_play_stop, "终止");
            lv_label_set_text(label_pause_resume, "暂停");
            update_music_display(); // 更新显示
        } else {
            end_music_mode();
            is_playing = false;
            is_paused = false;
            lv_label_set_text(label_play_stop, "播放");
            lv_label_set_text(label_pause_resume, "暂停");
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
            lv_label_set_text(label_pause_resume, "继续");
        } else {
            continue_music_mode();
            is_paused = false;
            lv_label_set_text(label_pause_resume, "暂停");
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
        lv_label_set_text(label_play_stop, "终止");
        lv_label_set_text(label_pause_resume, "暂停");
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
        lv_label_set_text(label_play_stop, "终止");
        lv_label_set_text(label_pause_resume, "暂停");
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
    if(label_play_stop != NULL) {
        if(is_playing) {
            lv_label_set_text(label_play_stop, "终止");
        } else {
            lv_label_set_text(label_play_stop, "播放");
        }
    }
}

void music_gui_set_pause_state(bool paused)
{
    is_paused = paused;
    if(label_pause_resume != NULL) {
        if(is_paused) {
            lv_label_set_text(label_pause_resume,"继续");
        } else {
            lv_label_set_text(label_pause_resume, "暂停");
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
    lv_obj_set_size(info_container, LV_HOR_RES - 80, 100);
    lv_obj_align(info_container, LV_ALIGN_TOP_MID, 0, 60);
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
    lv_obj_align(label_song_name, LV_ALIGN_CENTER, 0, -20);

    /* 歌曲索引 */
    label_song_index = lv_label_create(info_container);
    lv_label_set_text(label_song_index, "-- / --");
    if(style != NULL) {
        lv_obj_add_style(label_song_index, style, 0);
    }
    lv_obj_align(label_song_index, LV_ALIGN_CENTER, 0, 15);

    /* 歌词显示区域 */
    lv_obj_t *lyrics_container = lv_obj_create(music_screen);
    lv_obj_set_size(lyrics_container, LV_HOR_RES - 80, 140);
    lv_obj_align(lyrics_container, LV_ALIGN_TOP_MID, 0, 175);
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
    lv_label_set_long_mode(label_lyrics, LV_LABEL_LONG_WRAP); // 自动换行
    lv_obj_set_width(label_lyrics, LV_HOR_RES - 120);
    lv_obj_center(label_lyrics);

    /* 控制按钮容器 */
    lv_obj_t *ctrl_container = lv_obj_create(music_screen);
    lv_obj_set_size(ctrl_container, LV_HOR_RES - 80, 200);
    lv_obj_align(ctrl_container, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_style_bg_opa(ctrl_container, LV_OPA_0, 0);
    lv_obj_set_style_border_width(ctrl_container, 0, 0);

    /* 上一首按钮 */
    btn_prev = lv_btn_create(ctrl_container);
    lv_obj_set_size(btn_prev, 100, 60);
    lv_obj_align(btn_prev, LV_ALIGN_TOP_LEFT, 20, 70);
    lv_obj_add_event_cb(btn_prev, prev_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_color(btn_prev, lv_palette_main(LV_PALETTE_ORANGE), 0);
    
    lv_obj_t *label_prev = lv_label_create(btn_prev);
    lv_label_set_text(label_prev, "上一首");
    if(style != NULL) {
        lv_obj_add_style(label_prev, style, 0);
    }
    lv_obj_center(label_prev);

    /* 播放/终止按钮 */
    btn_play_stop = lv_btn_create(ctrl_container);
    lv_obj_set_size(btn_play_stop, 140, 80);
    lv_obj_align(btn_play_stop, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_add_event_cb(btn_play_stop, play_stop_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_color(btn_play_stop, lv_palette_main(LV_PALETTE_GREEN), 0);
    
    label_play_stop = lv_label_create(btn_play_stop);
    lv_label_set_text(label_play_stop, "播放");
    if(style != NULL) {
        lv_obj_add_style(label_play_stop, style, 0);
    }
    lv_obj_center(label_play_stop);

    /* 下一首按钮 */
    btn_next = lv_btn_create(ctrl_container);
    lv_obj_set_size(btn_next, 100, 60);
    lv_obj_align(btn_next, LV_ALIGN_TOP_RIGHT, -20, 70);
    lv_obj_add_event_cb(btn_next, next_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_color(btn_next, lv_palette_main(LV_PALETTE_ORANGE), 0);
    
    lv_obj_t *label_next = lv_label_create(btn_next);
    lv_label_set_text(label_next, "下一首");
    if(style != NULL) {
        lv_obj_add_style(label_next, style, 0);
    }
    lv_obj_center(label_next);

    /* 暂停/恢复按钮 */
    btn_pause_resume = lv_btn_create(ctrl_container);
    lv_obj_set_size(btn_pause_resume, 140, 60);
    lv_obj_align(btn_pause_resume, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_event_cb(btn_pause_resume, pause_resume_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_color(btn_pause_resume, lv_palette_main(LV_PALETTE_YELLOW), 0);
    
    label_pause_resume = lv_label_create(btn_pause_resume);
    lv_label_set_text(label_pause_resume, "暂停");
    if(style != NULL) {
        lv_obj_add_style(label_pause_resume, style, 0);
    }
    lv_obj_center(label_pause_resume);
    
    // 初始化显示当前歌曲信息
    update_music_display();
}