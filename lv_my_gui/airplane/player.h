#ifndef __PLAYER_H
#define __PLAYER_H

#include "../lvgl/lvgl.h"
#include "air.h"  

// ========== 全局状态数组声明 ==========
/**
 * @brief 子弹状态二维数组
 * @note 第一维：弹道编号 [0, BULLET_COM_MAX)
 *       第二维：该弹道中的子弹索引 [0, BULLET_MAX)
 * @values OBJ_STATE_FREE (空闲) / OBJ_STATE_ACTIVE (激活)
 */
extern obj_state_t bullet_states[BULLET_COM_MAX][BULLET_MAX];

// ========== 数学工具函数 ==========
/**
 * @brief 简化版余弦函数 (适用于 -π ~ π)
 * @param rad 弧度值
 * @return 余弦值 [-1, 1]
 * @note 使用泰勒级数展开，精度满足游戏需求
 */
float lv_cos_f32(float rad);

/**
 * @brief 简化版正弦函数 (适用于 -π ~ π)
 * @param rad 弧度值
 * @return 正弦值 [-1, 1]
 * @note 使用泰勒级数展开
 */
float lv_sin_f32(float rad);

// ========== 游戏逻辑函数 ==========
/**
 * @brief 子弹发射逻辑（逻辑层）
 * @details 根据弹道数量和角度步长计算每颗子弹的初始位置
 */
void fire_bullet_logic(void);

/**
 * @brief 子弹移动逻辑（逻辑层）
 * @details 更新子弹位置，执行边界检测和碰撞检测
 */
void move_bullet_logic(void);

/**
 * @brief 玩家飞机移动事件处理
 * @param e LVGL事件对象
 * @details 处理触摸拖拽，支持灵敏度调节
 */
void move_plane(lv_event_t *e);

#endif  // PLAYER_H