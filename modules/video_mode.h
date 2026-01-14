#ifndef _VIDEO_MODE_H
#define _VIDEO_MODE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define VIDEO_NAME_SIZE 3
#define SYSTEM_VIDEO_BUF_SIZE 1024

static char * video_name_buf[VIDEO_NAME_SIZE] = {"short.avi", "long.avi"};
static int video_flag = 0;

void play_video_mode(void);
void terminate_video_mode(void);
void prev_video_mode(void);
void stop_video_mode(void);
void continue_video_mode(void);
void next_video_mode(void);

#endif