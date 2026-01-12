#include "music_mode.h"
#define MAX_MUSIC_NUM 30
static char *music_list[MAX_MUSIC_NUM] = {
    "/song1.mp3",
    "/song2.mp3",
    "/song3.mp3",
    "/song4.mp3",
    "/song5.mp3"
};
int music_flag = 0;
void play_music(void) {
    char buf[100];
    snprintf(buf, sizeof(buf), "play %s &", music_list[music_flag]);
    system(buf);
}
void stop_music(void) {
    system("killall -STOP play");
}

void next_music(void) {
    music_flag = (music_flag + 1) % MAX_MUSIC_NUM;
    char buf[100];
    snprintf(buf, sizeof(buf), "play %s &", music_list[music_flag]);
    system(buf);
}
void previous_music(void) {
    music_flag = (music_flag - 1 + MAX_MUSIC_NUM) % MAX_MUSIC_NUM;
    char buf[100];
    snprintf(buf, sizeof(buf), "play %s &", music_list[music_flag]);
    system(buf);
}

void continue_music_mode(void) {
    system("killall -CONT play &");
}
void end_music_mode(void) {
    system("killall -9 play");
}