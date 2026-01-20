#ifndef PLAYER_H
#define PLAYER_H

#include "../lvgl/lvgl.h"
#include "air.h"  
extern obj_state_t bullet_states[BULLET_COM_MAX][BULLET_MAX];
// 函数声明
float lv_cos_f32(float rad);    // 简化版余弦函数
float lv_sin_f32(float rad);    // 简化版正弦函数
void fire_bullet_logic(void);   // 生成子弹（逻辑层）
void move_bullet_logic(void);   // 子弹移动逻辑
void move_plane(lv_event_t *e); // 玩家移动（添加灵敏度控制）
#endif  // PLAYER_H