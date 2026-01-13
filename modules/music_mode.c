#include "music_mode.h"

// 音乐列表
static music_info_t music_list[] = {
    {
        .file_path = "./jiangnan.mp3",
        .song_name = "江南",
        .lyrics = "风到这里就是粘-粘住过客的思念-雨到了这里缠成线-缠着我们流连人世间"
    },
    {
        .file_path = "./youfeng.mp3",
        .song_name = "悠风",
        .lyrics = "悠风吹过-又是一季花开落-又是一次离别时-又是一段相思愁"
    }
};

static int music_flag = 0;
static int total_music = sizeof(music_list) / sizeof(music_list[0]);

void play_music(void) {
    char buf[200];
    snprintf(buf, sizeof(buf), "play %s &", music_list[music_flag].file_path);
    system(buf);
}

void stop_music(void) {
    system("killall -STOP play");
}

void next_music(void) {
    // 先停止当前播放
    end_music_mode();
    
    // 切换到下一首
    music_flag = (music_flag + 1) % total_music;
    
    // 播放新歌曲
    play_music();
}

void previous_music(void) {
    // 先停止当前播放
    end_music_mode();
    
    // 切换到上一首
    music_flag = (music_flag - 1 + total_music) % total_music;
    
    // 播放新歌曲
    play_music();
}

void continue_music_mode(void) {
    system("killall -CONT play");
}

void end_music_mode(void) {
    system("killall -9 play");
}

// 新增接口实现
int get_current_music_index(void) {
    return music_flag;
}

int get_total_music_count(void) {
    return total_music;
}

const char* get_current_song_name(void) {
    return music_list[music_flag].song_name;
}

const char* get_current_lyrics(void) {
    return music_list[music_flag].lyrics;
}