#include "camera_mode.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// ========== 内部全局变量 ==========
static int cam_fd = -1;
static uint8_t *cam_buf = NULL;
static bool capture_running = false;
static void *mmap_buffers[4] = {NULL};
static unsigned int mmap_lengths[4] = {0};

// 状态变量
static int frame_count = 0;
static int current_fps = 0;
static int retry_count = 0;
static time_t last_error_time = 0;
static time_t last_fps_calc_time = 0;

// ========== 内部函数声明 ==========
static int cam_hw_init(void);
static void cam_hw_deinit(void);
static int cam_hw_capture_frame(uint8_t *buf, uint32_t buf_size);

// ========== 核心接口实现 ==========
int video_capture_init(void)
{
    // 先释放已有资源
    if (cam_fd >= 0 || cam_buf != NULL) 
    {
        cam_hw_deinit();
    }

    // 初始化状态变量
    frame_count = 0;
    current_fps = 0;
    retry_count = 0;
    last_error_time = 0;
    last_fps_calc_time = time(NULL);
    memset(mmap_buffers, 0, sizeof(mmap_buffers));
    memset(mmap_lengths, 0, sizeof(mmap_lengths));

    // 分配帧缓冲区
    cam_buf = (uint8_t *)malloc(CAM_FRAME_WIDTH * CAM_FRAME_HEIGHT * 3);
    if (cam_buf == NULL) 
    {
        perror("video capture: malloc buffer failed");
        return -1;
    }

    return 0;
}

void video_capture_deinit(void)
{
    video_capture_stop();
    cam_hw_deinit();
    
    if (cam_buf) 
    {
        free(cam_buf);
        cam_buf = NULL;
    }
}

int video_capture_start(void)
{
    if (capture_running) 
    {
        return 0; // 已在运行
    }

    if (cam_buf == NULL) 
    {
        return -1; // 未初始化
    }

    // 初始化硬件
    if (cam_hw_init() != 0) 
    {
        return -1;
    }

    capture_running = true;
    last_fps_calc_time = time(NULL);
    return 0;
}

void video_capture_stop(void)
{
    capture_running = false;
    cam_hw_deinit();
}

int video_capture_get_frame(video_frame_t *frame)
{
    if (frame == NULL || !capture_running || cam_buf == NULL) 
    {
        return -1;
    }

    // 异常重试逻辑
    time_t now = time(NULL);
    if (cam_fd < 0 && difftime(now, last_error_time) > 1 && retry_count < CAM_MAX_RETRY_CNT) 
    {
        cam_hw_deinit();
        cam_hw_init();
        retry_count++;
        last_error_time = now;
        return -1;
    }

    // 采集硬件帧数据
    int ret = cam_hw_capture_frame(cam_buf, CAM_FRAME_WIDTH * CAM_FRAME_HEIGHT * 3);
    if (ret != 0) 
    {
        frame->valid = false;
        return ret;
    }

    // 填充帧数据结构体
    frame->data = cam_buf;
    frame->width = CAM_FRAME_WIDTH;
    frame->height = CAM_FRAME_HEIGHT;
    frame->data_size = CAM_FRAME_WIDTH * CAM_FRAME_HEIGHT * 3;
    frame->valid = true;

    // 帧率统计
    frame_count++;
    if (difftime(now, last_fps_calc_time) >= 1) 
    {
        current_fps = frame_count;
        frame_count = 0;
        last_fps_calc_time = now;
    }

    return 0;
}

int video_capture_get_fps(void)
{
    return current_fps;
}

bool video_capture_is_running(void)
{
    return capture_running;
}

// ========== 硬件底层操作 (私有化) ==========
static int cam_hw_init(void)
{
    struct v4l2_format fmt;
    struct v4l2_requestbuffers req;
    struct v4l2_buffer buf;
    struct v4l2_capability cap;

    // 打开设备
    cam_fd = open(CAM_DEV_PATH, O_RDWR | O_NONBLOCK);
    if (cam_fd < 0) 
    {
        perror("video capture: open device failed");
        last_error_time = time(NULL);
        return -1;
    }

    // 检查设备能力
    if (ioctl(cam_fd, VIDIOC_QUERYCAP, &cap) < 0) 
    {
        perror("video capture: query cap failed");
        close(cam_fd);
        cam_fd = -1;
        last_error_time = time(NULL);
        return -1;
    }

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) || !(cap.capabilities & V4L2_CAP_STREAMING)) 
    {
        fprintf(stderr, "video capture: device not support capture/stream\n");
        close(cam_fd);
        cam_fd = -1;
        last_error_time = time(NULL);
        return -1;
    }

    // 设置格式
    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = CAM_FRAME_WIDTH;
    fmt.fmt.pix.height = CAM_FRAME_HEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if (ioctl(cam_fd, VIDIOC_S_FMT, &fmt) < 0) 
    {
        perror("video capture: set format failed");
        close(cam_fd);
        cam_fd = -1;
        last_error_time = time(NULL);
        return -1;
    }

    // 请求缓冲区
    memset(&req, 0, sizeof(req));
    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (ioctl(cam_fd, VIDIOC_REQBUFS, &req) < 0) 
    {
        perror("video capture: request buffers failed");
        close(cam_fd);
        cam_fd = -1;
        last_error_time = time(NULL);
        return -1;
    }

    // 映射缓冲区
    for (int i = 0; i < req.count; i++) 
    {
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (ioctl(cam_fd, VIDIOC_QUERYBUF, &buf) < 0) 
        {
            perror("video capture: query buffer failed");
            cam_hw_deinit();
            return -1;
        }

        mmap_buffers[i] = mmap(NULL, buf.length, PROT_READ, MAP_SHARED, cam_fd, buf.m.offset);
        if (mmap_buffers[i] == MAP_FAILED) 
        {
            perror("video capture: mmap failed");
            cam_hw_deinit();
            return -1;
        }
        mmap_lengths[i] = buf.length;

        // 入队缓冲区
        if (ioctl(cam_fd, VIDIOC_QBUF, &buf) < 0) 
        {
            perror("video capture: qbuf failed");
            cam_hw_deinit();
            return -1;
        }
    }

    // 启动流
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(cam_fd, VIDIOC_STREAMON, &type) < 0) 
    {
        perror("video capture: stream on failed");
        cam_hw_deinit();
        return -1;
    }

    return 0;
}

static void cam_hw_deinit(void)
{
    if (cam_fd >= 0) 
    {
        // 停止流
        enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        ioctl(cam_fd, VIDIOC_STREAMOFF, &type);

        // 释放映射
        for (int i = 0; i < 4; i++) 
        {
            if (mmap_buffers[i] != NULL && mmap_buffers[i] != MAP_FAILED) 
            {
                munmap(mmap_buffers[i], mmap_lengths[i]);
                mmap_buffers[i] = NULL;
                mmap_lengths[i] = 0;
            }
        }

        close(cam_fd);
        cam_fd = -1;
    }
}

static int cam_hw_capture_frame(uint8_t *buf, uint32_t buf_size)
{
    if (cam_fd < 0) 
    {
        return -1;
    }

    struct v4l2_buffer v4l2_buf;
    memset(&v4l2_buf, 0, sizeof(v4l2_buf));
    v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_buf.memory = V4L2_MEMORY_MMAP;

    // 出队缓冲区
    int ret = ioctl(cam_fd, VIDIOC_DQBUF, &v4l2_buf);
    if (ret < 0) 
    {
        if (errno == EAGAIN) 
        {
            return 1; // 暂无数据
        }
        perror("video capture: dqbuf failed");
        last_error_time = time(NULL);
        return -1;
    }

    // 拷贝数据
    if (v4l2_buf.index >= 0 && v4l2_buf.index < 4 && mmap_buffers[v4l2_buf.index] != NULL) 
    {
        memcpy(buf, mmap_buffers[v4l2_buf.index], 
               v4l2_buf.bytesused > buf_size ? buf_size : v4l2_buf.bytesused);
    }

    // 重新入队
    ioctl(cam_fd, VIDIOC_QBUF, &v4l2_buf);

    return 0;
}