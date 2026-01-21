#include "camera_mode.h"

// 全局变量（供GUI模块控制）
int camera_running_flag = 0;     // 摄像头运行标志（0:停止 1:运行）
pthread_t camera_thread;         // 摄像头采集线程

// 静态全局变量（摄像头内部使用）
static int exit_flag = 0;
static int camera_fd = -1;
static int lcd_fd = -1;
static int *lcd_memory = NULL;
static int buf_count = 4;
static struct buffer_info camera_buffers[4] = {0};
static struct camera_config cam_cfg = {640, 480};
static struct display_config disp_cfg = {1024, 600, (1024-640)/2, (600-480)/2};
static int *argb_buffer = NULL;

// 原信号处理函数（保留）
void sig_handler(int signo) {
    if (signo == SIGINT) {
        printf("\n收到退出信号，准备释放资源...\n");
        exit_flag = 1;
        camera_running_flag = 0;
    }
}

// 原YUV转ARGB函数（保留）
int yuv_to_argb(int y, int u, int v)
{
    int r, g, b;
    int pixel;
    r = y + 1.4075 * (v - 128);
    g = y - 0.3455 * (u - 128) - 0.7169 * (v - 128);
    b = y + 1.779 * (u - 128);
    r = (r > 255) ? 255 : (r < 0) ? 0 : r;
    g = (g > 255) ? 255 : (g < 0) ? 0 : g;
    b = (b > 255) ? 255 : (b < 0) ? 0 : b;
    pixel = (0x00 << 24) | (r << 16) | (g << 8) | b;
    return pixel;
}

// 原YUYV转ARGB函数（保留）
int convert_yuyv_to_argb(char *yuyv_data, int *argb_data, int width, int height)
{
    int i, j;
    int total_pixels = width * height;
    for (i = 0, j = 0; i < total_pixels && j < total_pixels * 2; i += 2, j += 4)
    {
        argb_data[i] = yuv_to_argb((unsigned char)yuyv_data[j], 
                                   (unsigned char)yuyv_data[j + 1], 
                                   (unsigned char)yuyv_data[j + 3]);
        argb_data[i + 1] = yuv_to_argb((unsigned char)yuyv_data[j + 2], 
                                       (unsigned char)yuyv_data[j + 1], 
                                       (unsigned char)yuyv_data[j + 3]);
    }
    return 0;
}

// 原摄像头初始化函数（调整为内部静态函数）
static int init_camera_module(char *openVideoPathName)
{
    int ret;
    camera_fd = open(openVideoPathName, O_RDWR);
    if (camera_fd == -1)
    {
        perror("打开摄像头失败");
        return -1;
    }

    struct v4l2_capability cap;
    ret = ioctl(camera_fd, VIDIOC_QUERYCAP, &cap);
    if (ret == -1) {
        perror("查询摄像头能力失败");
        close(camera_fd);
        return -1;
    }
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        fprintf(stderr, "设备不支持视频捕获\n");
        close(camera_fd);
        return -1;
    }
    if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
        fprintf(stderr, "设备不支持流捕获\n");
        close(camera_fd);
        return -1;
    }

    struct v4l2_format format;
    memset(&format, 0, sizeof(format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = cam_cfg.width;
    format.fmt.pix.height = cam_cfg.height;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    format.fmt.pix.field = V4L2_FIELD_NONE;

    ret = ioctl(camera_fd, VIDIOC_S_FMT, &format);
    if (ret == -1)
    {
        perror("设置摄像头格式失败");
        close(camera_fd);
        return -1;
    }
    printf("实际摄像头分辨率：%dx%d\n", format.fmt.pix.width, format.fmt.pix.height);

    struct v4l2_requestbuffers req_buf;
    memset(&req_buf, 0, sizeof(req_buf));
    req_buf.count = 4;
    req_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req_buf.memory = V4L2_MEMORY_MMAP;

    ret = ioctl(camera_fd, VIDIOC_REQBUFS, &req_buf);
    if (ret == -1)
    {
        perror("申请缓冲区失败");
        close(camera_fd);
        return -1;
    }
    if (req_buf.count < 2) {
        fprintf(stderr, "缓冲区数量不足\n");
        close(camera_fd);
        return -1;
    }

    struct v4l2_buffer buffer;
    for (int i = 0; i < req_buf.count; i++)
    {
        memset(&buffer, 0, sizeof(buffer));
        buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer.memory = V4L2_MEMORY_MMAP;
        buffer.index = i;

        ret = ioctl(camera_fd, VIDIOC_QUERYBUF, &buffer);
        if (ret == -1)
        {
            perror("查询缓冲区信息失败");
            close(camera_fd);
            return -1;
        }

        camera_buffers[i].length = buffer.length;
        camera_buffers[i].start = mmap(NULL, buffer.length,
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED,
                                camera_fd,
                                buffer.m.offset);
        if (camera_buffers[i].start == MAP_FAILED)
        {
            perror("映射缓冲区失败");
            close(camera_fd);
            return -1;
        }

        ret = ioctl(camera_fd, VIDIOC_QBUF, &buffer);
        if (ret == -1)
        {
            perror("缓冲区入队失败");
            close(camera_fd);
            return -1;
        }
    }

    enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(camera_fd, VIDIOC_STREAMON, &buf_type);
    if (ret == -1)
    {
        perror("启动视频流失败");
        close(camera_fd);
        return -1;
    }

    printf("摄像头初始化成功\n");
    return 0;
}

// 原LCD初始化函数（调整为内部静态函数）
static int init_display_module(char *lcd_path_name)
{
    lcd_fd = open(lcd_path_name, O_RDWR);
    if (lcd_fd == -1)
    {
        perror("打开液晶屏失败");
        return -1;
    }

    lcd_memory = mmap(NULL,
                    disp_cfg.lcd_width * disp_cfg.lcd_height * 4,
                    PROT_READ | PROT_WRITE,
                    MAP_SHARED,
                    lcd_fd,
                    0);

    if (lcd_memory == MAP_FAILED)
    {
        perror("映射液晶屏内存失败");
        close(lcd_fd);
        return -1;
    }

    printf("LCD显示模块初始化成功\n");
    return 0;
}

// 在指定位置显示摄像头画面
void display_camera_frame(int *argb_buffer, int *lcd_mem,
                          struct camera_config *cam_cfg,
                          struct display_config disp_cfg)  // 若为普通结构体，此处无*
{
    int src_row, dst_row;
    int width = cam_cfg->width;
    int height = cam_cfg->height;

    // 边界检查：防止显示位置超出LCD范围
    // 修正：将disp_cfg-> 改为 disp_cfg.
    if (disp_cfg.display_x < 0 || disp_cfg.display_y < 0 ||
        disp_cfg.display_x + width > disp_cfg.lcd_width ||
        disp_cfg.display_y + height > disp_cfg.lcd_height) {
        fprintf(stderr, "显示位置超出LCD边界\n");
        return;
    }

    // 边界检查：防止显示位置超出LCD范围
    if (disp_cfg.display_x < 0 || disp_cfg.display_y < 0 ||
        disp_cfg.display_x + width > disp_cfg.lcd_width ||
        disp_cfg.display_y + height > disp_cfg.lcd_height) {
        fprintf(stderr, "显示位置超出LCD边界\n");
        return;
    }

    // 逐行复制ARGB数据到LCD帧缓冲
    for (src_row = 0; src_row < height; src_row++)
    {
        // 计算目标行（LCD上的行）
        dst_row = disp_cfg.display_y + src_row;  // 同步修正为.

        // 目标行起始地址 = LCD基地址 + 目标行偏移 + X坐标偏移
        int *dst_line_start = lcd_mem + dst_row * disp_cfg.lcd_width + disp_cfg.display_x;  // 同步修正为.

        // 源数据行起始地址
        int *src_line_start = argb_buffer + src_row * width;

        // 复制一行像素（每个像素4字节，直接memcpy效率最高）
        memcpy(dst_line_start, src_line_start, width * 4);
    }
}

// 原释放资源函数（调整为内部静态函数）
static void release_resources(void) {
    exit_flag = 1;
    camera_running_flag = 0;

    // 停止视频流
    enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (camera_fd > 0 && ioctl(camera_fd, VIDIOC_STREAMOFF, &buf_type) == -1) {
        perror("停止视频流失败");
    }

    // 解除摄像头缓冲区映射
    for (int i = 0; i < buf_count; i++) {
        if (camera_buffers[i].start != NULL && camera_buffers[i].start != MAP_FAILED) {
            munmap(camera_buffers[i].start, camera_buffers[i].length);
        }
    }

    // 解除LCD内存映射
    if (lcd_memory != NULL && lcd_memory != MAP_FAILED) {
        munmap(lcd_memory, disp_cfg.lcd_width * disp_cfg.lcd_height * 4);
    }

    // 关闭文件描述符
    if (camera_fd > 0) close(camera_fd);
    if (lcd_fd > 0) close(lcd_fd);

    // 释放ARGB缓冲区
    if (argb_buffer != NULL) free(argb_buffer);

    printf("摄像头资源释放完成\n");
}

// 对外接口：初始化摄像头+LCD系统
int camera_system_init(void)
{
    int ret;
    // 分配ARGB缓冲区
    argb_buffer = malloc(cam_cfg.width * cam_cfg.height * sizeof(int));
    if (argb_buffer == NULL)
    {
        perror("分配ARGB缓冲区失败");
        return -1;
    }

    // 初始化LCD
    ret = init_display_module("/dev/fb0");
    if (ret != 0)
    {
        free(argb_buffer);
        return -1;
    }

    // 初始化摄像头
    ret = init_camera_module("/dev/video9");
    if (ret != 0)
    {
        munmap(lcd_memory, disp_cfg.lcd_width * disp_cfg.lcd_height * 4);
        close(lcd_fd);
        free(argb_buffer);
        return -1;
    }

    exit_flag = 0;
    return 0;
}

// 摄像头采集循环（线程执行函数）
void *camera_collect_loop(void *arg)
{
    int ret;
    struct v4l2_buffer v4l2_buf;
    camera_running_flag = 1;
    printf("摄像头采集线程启动（按关闭按钮停止）...\n");

    while (!exit_flag && camera_running_flag)
    {
        for (int i = 0; i < buf_count && camera_running_flag; i++)
        {
            memset(&v4l2_buf, 0, sizeof(v4l2_buf));
            v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            v4l2_buf.memory = V4L2_MEMORY_MMAP;
            v4l2_buf.index = i;

            // 取出缓冲区（非阻塞，避免卡死）
            ret = ioctl(camera_fd, VIDIOC_DQBUF, &v4l2_buf);
            if (ret == -1)
            {
                if (errno == EAGAIN) continue;
                perror("从队列取出缓冲区失败");
                camera_running_flag = 0;
                break;
            }

            // 格式转换+显示
            convert_yuyv_to_argb(camera_buffers[i].start, argb_buffer, cam_cfg.width, cam_cfg.height);
            display_camera_frame(argb_buffer, lcd_memory, &cam_cfg, disp_cfg);

            // 缓冲区重新入队
            ret = ioctl(camera_fd, VIDIOC_QBUF, &v4l2_buf);
            if (ret == -1)
            {
                perror("缓冲区重新入队失败");
                camera_running_flag = 0;
                break;
            }
        }
    }

    release_resources();
    pthread_exit(NULL);
    return NULL;
}

// 对外接口：启动摄像头采集（创建线程）
void camera_system_run(void)
{
    if (camera_running_flag) return; // 已运行则直接返回
    int ret = pthread_create(&camera_thread, NULL, camera_collect_loop, NULL);
    if (ret != 0)
    {
        perror("创建摄像头线程失败");
        camera_running_flag = 0;
    }
}

// 对外接口：停止摄像头采集
void camera_system_stop(void)
{
    if (!camera_running_flag) return;
    camera_running_flag = 0;
    exit_flag = 1;
    pthread_join(camera_thread, NULL); // 等待线程退出
}

// 对外接口：释放摄像头系统资源
void camera_system_release(void)
{
    camera_system_stop();
    release_resources();
}

// 原main函数改名为独立测试入口（可选保留）
int camera_standalone_main(void)
{
    if (signal(SIGINT, sig_handler) == SIG_ERR) 
    {
        perror("注册信号处理失败");
        return -1;
    }

    if (camera_system_init() != 0)
    {
        return -1;
    }

    camera_system_run();
    pthread_join(camera_thread, NULL); // 等待线程结束
    return 0;
}