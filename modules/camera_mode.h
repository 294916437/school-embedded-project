#ifndef __CAMERA_MODE_H
#define __CAMERA_MODE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


#define CAM_DEV_PATH            "/dev/video9"   // 摄像头设备节点
#define CAM_FRAME_WIDTH         640             // 采集宽度
#define CAM_FRAME_HEIGHT        480             // 采集高度
#define CAM_MAX_RETRY_CNT       3               // 异常最大重试次数

// 视频帧数据结构
typedef struct {
    uint8_t *data;          // 帧数据缓冲区 (RGB24格式)
    uint32_t width;         // 帧宽度
    uint32_t height;        // 帧高度
    uint32_t data_size;     // 数据大小 (width*height*3)
    bool valid;             // 帧数据是否有效
} video_frame_t;

// ========== 核心接口 ==========
/**
 * @brief 初始化视频采集模块
 * @return 0:成功 其他:失败
 */
int video_capture_init(void);

/**
 * @brief 销毁视频采集模块
 */
void video_capture_deinit(void);

/**
 * @brief 启动视频采集
 * @return 0:成功 其他:失败
 */
int video_capture_start(void);

/**
 * @brief 停止视频采集
 */
void video_capture_stop(void);

/**
 * @brief 获取一帧视频数据
 * @param frame 输出参数：帧数据结构体
 * @return 0:成功 -1:失败 1:暂无数据
 */
int video_capture_get_frame(video_frame_t *frame);

/**
 * @brief 获取当前采集帧率
 * @return 当前帧率值
 */
int video_capture_get_fps(void);

/**
 * @brief 检查视频采集是否正在运行
 * @return true:运行中 false:已停止
 */
bool video_capture_is_running(void);

#ifdef __cplusplus
}
#endif

#endif