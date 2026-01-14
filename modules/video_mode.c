#include "video_mode.h"

//播放函数
void play_video_mode(void)
{
    char Video_Buf[SYSTEM_VIDEO_BUF_SIZE];
    snprintf(Video_Buf, SYSTEM_VIDEO_BUF_SIZE, "gst-play-1.0 %s &", video_name_buf[video_flag]);
    system(Video_Buf);
}

//终止函数
void terminate_video_mode(void)
{
    system("killall -9 gst-play-1.0");
}

void prev_video_mode(void)
{
    video_flag -= 1;
    if(video_flag < 0)
    {
        video_flag = VIDEO_NAME_SIZE - 1;
    }
    char Video_Buf[SYSTEM_VIDEO_BUF_SIZE];
    snprintf(Video_Buf, SYSTEM_VIDEO_BUF_SIZE, "gst-play-1.0 %s &", video_name_buf[video_flag]);
    system(Video_Buf);
}

void stop_video_mode(void)
{
    system("killall -STOP gst-play-1.0");
}

void continue_video_mode(void)
{
    system("killall -CONT gst-play-1.0");
}

void next_video_mode(void)
{
    video_flag += 1;
    if(video_flag > VIDEO_NAME_SIZE - 1)
    {
        video_flag = 0;
    }
    char Video_Buf[SYSTEM_VIDEO_BUF_SIZE];
    snprintf(Video_Buf, SYSTEM_VIDEO_BUF_SIZE, "gst-play-1.0 %s &", video_name_buf[video_flag]);
    system(Video_Buf);
}