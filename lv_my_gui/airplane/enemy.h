#ifndef ENEMY_H
#define ENEMY_H

#include "../lvgl/lvgl.h"
#include "air.h"  // 依赖obj_state_t和game_data_t

// 敌机与子弹状态数组（全局）
extern obj_state_t enemy_states[ENEMIES_MAX];                  // 敌机状态
extern obj_state_t enemy_bullet_states[ENEMIES_MAX][ENEMY_BULLET_MAX];  // 敌机子弹状态
extern int enemies_hp[ENEMIES_MAX];                            // 敌机血量
extern int enemies_bullrt_num[ENEMIES_MAX];                     // 敌机子弹计数
extern int enemies_bullet_cnt[ENEMIES_MAX][ENEMY_BULLET_MAX];   // 敌机子弹生命周期计数

// 函数声明
int32_t lv_random_int(int32_t min, int32_t max);  // 随机数生成
void enemy_act_logic(void);                // 生成敌机（逻辑层）
void fire_enemybullet_logic(void);         // 敌机发射子弹（逻辑层）
void move_enemybullet_logic(void);         // 敌机子弹追踪玩家
void move_enemiesplane_logic(void);        // 敌机移动逻辑

#endif  // ENEMY_H