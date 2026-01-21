#ifndef __CAMERA_H
#define __CAMERA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <linux/videodev2.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>

// 结构体定义
struct display_config
{
    int lcd_width;
    int lcd_height;
    int display_x;
    int display_y;
};

struct camera_config
{
    int width;
    int height;
};

// 缓冲区信息结构体
struct buffer_info
{
    void *start; // 缓冲区起始地址
    int length;  // 缓冲区长度
};
// 设置自定义显示回调函数
typedef void (*camera_display_callback_t)(int *argb_buffer, int width, int height);

// 全局控制变量（供GUI调用）
extern int camera_running_flag;  // 摄像头运行标志
extern pthread_t camera_thread;  // 摄像头线程句柄

// 对外接口声明
int camera_system_init(void);    // 初始化摄像头+LCD
void camera_system_run(void);    // 摄像头采集循环（线程执行）
void camera_system_stop(void);   // 停止摄像头采集
void camera_system_release(void);// 释放摄像头+LCD资源
void camera_set_display_callback(camera_display_callback_t callback);
// 获取当前帧缓冲区（用于拍照保存）
int* camera_get_current_frame(void);

#endif