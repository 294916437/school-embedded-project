#ifndef _MUSIC_MODE_H
#define _MUSIC_MODE_H
#include <stdio.h>
#include <unistd.h>

// 音乐信息结构体
typedef struct {
    char *file_path;
    char *song_name;
    char *lyrics;
} music_info_t;

void play_music(void);
void stop_music(void);
void next_music(void);
void previous_music(void);
void continue_music_mode(void);
void end_music_mode(void);

// 新增接口
int get_current_music_index(void);
int get_total_music_count(void);
const char* get_current_song_name(void);
const char* get_current_lyrics(void);

#endif