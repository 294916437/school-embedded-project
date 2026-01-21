#ifndef __ENEMY_H
#define __ENEMY_H

#include "../lvgl/lvgl.h"
#include "air.h"  // 依赖obj_state_t和game_data_t

// ========== 全局状态数组声明 ==========
/**
 * @brief 敌机状态数组
 * @note 索引范围: [0, ENEMIES_MAX)
 * @values OBJ_STATE_FREE (空闲) / OBJ_STATE_ACTIVE (激活)
 */
extern obj_state_t enemy_states[ENEMIES_MAX];  // 最多20个敌机

/**
 * @brief 敌机子弹状态二维数组
 * @note 第一维：敌机编号 [0, ENEMIES_MAX)
 *       第二维：该敌机的子弹索引 [0, ENEMY_BULLET_MAX)
 */
extern obj_state_t enemy_bullet_states[ENEMIES_MAX][ENEMY_BULLET_MAX];

/**
 * @brief 敌机血量数组
 * @note 当前实现中，所有敌机初始HP=1（一击必杀）
 */
extern int enemies_hp[ENEMIES_MAX];

/**
 * @brief 敌机子弹发射计数器
 * @note 用于轮询选择下一个发射的子弹位置（循环队列）
 */
extern int enemies_bullet_num[ENEMIES_MAX];

/**
 * @brief 敌机子弹生命周期计数
 * @note 用于限制子弹存活时间（防止无限追踪）
 */
extern int enemies_bullet_cnt[ENEMIES_MAX][ENEMY_BULLET_MAX];

// ========== 工具函数 ==========
/**
 * @brief 生成指定范围的随机整数
 * @param min 最小值（包含）
 * @param max 最大值（包含）
 * @return 随机整数 [min, max]
 */
int32_t lv_random_int(int32_t min, int32_t max);

// ========== 游戏逻辑函数 ==========
/**
 * @brief 生成敌机（逻辑层）
 * @details 从3个方向随机生成敌机（上方、左侧、右侧）
 */
void enemy_act_logic(void);

/**
 * @brief 敌机发射子弹（逻辑层）
 * @details 所有激活的敌机发射一颗追踪子弹
 */
void fire_enemybullet_logic(void);

/**
 * @brief 敌机子弹移动逻辑（逻辑层）
 * @details 实现追踪玩家的智能弹道，包含碰撞检测
 */
void move_enemybullet_logic(void);

/**
 * @brief 敌机移动逻辑（逻辑层）
 * @details 随机方向移动（上下左右），边界检测
 */
void move_enemiesplane_logic(void);

#endif  // ENEMY_H