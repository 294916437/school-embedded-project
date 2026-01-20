#include "player.h"
#include "air.h"
#include "enemy.h"
#include <math.h>
#include <stdio.h>

obj_state_t bullet_states[BULLET_COM_MAX][BULLET_MAX] = {OBJ_STATE_FREE};
// 简化版余弦函数（适用于-π到π弧度范围）
/**
 * @brief 计算32位浮点数的余弦值
 * 
 * @details 使用泰勒级数展开计算余弦值，将输入角度归一化到[-π, π]范围内，
 *          使用多项式近似计算：cos(x) ≈ 1 - x²/2! + x⁴/4! - x⁶/6!
 * 
 * @param rad 输入角度（弧度制）
 * 
 * @return float 返回输入角度的余弦值
 * 
 * @note 该函数使用泰勒级数的前4项进行近似计算，对于精度要求不高的场景适用
 */
float lv_cos_f32(float rad)
{
    // ... 原有代码保持不变 ...
    while (rad > M_PI) rad -= 2 * M_PI;
    while (rad < -M_PI) rad += 2 * M_PI;

    float x2 = rad * rad;       // x²
    float x4 = x2 * x2;         // x⁴
    float x6 = x4 * x2;         // x⁶
    
    return 1.0f - x2/2.0f + x4/24.0f - x6/720.0f;
}

// 简化版正弦函数（适用于-π到π弧度范围，精度满足一般游戏需求）
/**
 * @brief 计算32位浮点数的正弦值
 * 
 * 使用泰勒级数展开计算正弦值，输入角度会被归一化到[-π, π]范围内。
 * 该实现适用于单精度浮点数计算。
 * 
 * @param rad 输入的弧度值
 * @return float 返回计算得到的正弦值，范围在[-1, 1]之间
 * 
 * @note 使用泰勒级数展开近似计算：
 *       sin(x) ≈ x - x³/6 + x⁵/120 - x⁷/5040
 *       该近似在[-π, π]范围内具有较好的精度
 */
float lv_sin_f32(float rad) {
    // ... 原有代码保持不变 ...
    while (rad > M_PI) rad -= 2 * M_PI;
    while (rad < -M_PI) rad += 2 * M_PI;

    float x2 = rad * rad;
    float x3 = x2 * rad;
    float x5 = x3 * x2;
    float x7 = x5 * x2;
    
    return rad - x3/6.0f + x5/120.0f - x7/5040.0f;
}



/**
 * @brief 处理子弹发射逻辑
 * 
 * 该函数负责管理游戏中的子弹发射系统。它会遍历所有子弹组件，寻找空闲的子弹位置，
 * 并根据子弹组件的位置（左、中、右）计算不同的发射角度和初始位置。子弹发射后，
 * 其状态会被设置为激活状态。
 * 
 * @note 函数使用全局变量：
 * - bullet_com: 子弹组件数量
 * - BULLET_MAX: 最大子弹数量
 * - bullet_states: 子弹状态数组
 * - angle_step: 角度步长
 * - game_data: 包含玩家位置和子弹位置的游戏数据结构
 * 
 * @details 子弹发射逻辑：
 * 1. 遍历所有子弹组件
 * 2. 对每个组件，寻找空闲的子弹位置
 * 3. 根据组件在数组中的位置（左、中、右）计算发射角度：
 *    - 左侧：负角度
 *    - 中间：0度
 *    - 右侧：正角度
 * 4. 设置子弹的初始位置和状态
 * 
 * @warning 该函数会修改全局变量 bullet_states 和 game_data 中的子弹位置数据
 */
void fire_bullet_logic() {
    // ... 原有代码保持不变 ...
    // 遍历所有子弹组件
    for(int j = 0;j < bullet_com ; j++)
    {
        // 遍历子弹数组，寻找空闲的子弹位置
        for(int i = 0; i < BULLET_MAX; i++)
        {
            // 检查当前子弹状态是否为空闲
            if(bullet_states[j][i] == OBJ_STATE_FREE)
            {

                // 将子弹状态设置为激活
                bullet_states[j][i] = OBJ_STATE_ACTIVE;

                
                // 计算中间值，用于确定子弹的发射方向
                double judge = (bullet_com+1.0) / 2.0f; //3
                double cnt = (double)(j+1);
                // 根据子弹位置相对于中间值的位置，计算不同的发射角度
                if( cnt < judge)
                {
                    // 计算左侧子弹的发射角度（弧度）
                    float angle_rad = (angle_step)*(int)((judge-cnt)/1) * 3.1415926f / 180.0f;  // 转为弧度
                    // 设置左侧子弹的初始位置
                    game_data.bullet_x[j][i] = game_data.player_x - (10* lv_sin_f32(angle_rad));
                    game_data.bullet_y[j][i] = game_data.player_y + (10* lv_cos_f32(angle_rad));
                    
                }  
                else if(cnt == judge)
                {
                    // 中间子弹的初始位置与玩家位置相同
                    game_data.bullet_x[j][i] = game_data.player_x;
                    game_data.bullet_y[j][i] = game_data.player_y;
                }
                else
                {
                    // 计算右侧子弹的发射角度（弧度）
                    float angle_rad = (angle_step)*(int)((cnt-judge)/1) * 3.1415926f / 180.0f;  // 转为弧度
                    
                    // 设置右侧子弹的初始位置
                    game_data.bullet_x[j][i] = game_data.player_x + 10* lv_sin_f32(angle_rad);
                    game_data.bullet_y[j][i] = game_data.player_y + 10* lv_cos_f32(angle_rad);
                    
                }  
               
                break;
            }
        }
    }
}

// 子弹移动逻辑（仅计算位置，不操作UI）
/**
 * @brief 处理子弹移动逻辑的函数
 * 该函数负责子弹的移动、边界检测和碰撞检测
 */
void move_bullet_logic() {
    // 定义常量：圆周率、子弹速度、角度转弧度系数、屏幕尺寸和碰撞距离平方
    const float PI = 3.1415926f;
    const float BULLET_SPEED = 10.0f;
    const float ANGLE_TO_RADIAN = PI / 180.0f;
    const int SCREEN_WIDTH = 1024;
    const int SCREEN_HEIGHT = 600;
    const int COLLISION_DISTANCE_SQ = 1600;
    
    // 计算中心位置，用于子弹排列
    float center_position = (bullet_com + 1.0f) * 0.5f;
    
    // 预计算角度值，用于子弹扇形排列
    float angle_cache[BULLET_MAX];
    for(int j = 0; j < bullet_com; j++) {
        float relative_position = (float)(j + 1);
        float angle_offset = 0.0f;
        
        // 根据子弹相对于中心的位置计算角度偏移
        if(relative_position < center_position) {
            angle_offset = -angle_step * (int)(center_position + 0.5f - relative_position);
        } else if(relative_position > center_position) {
            angle_offset = angle_step * (int)(relative_position - center_position + 0.5f);
        }
        
        // 将角度转换为弧度并存储
        angle_cache[j] = angle_offset * ANGLE_TO_RADIAN;
    }
    
    // 预计算三角函数值，提高性能
    float sin_cache[BULLET_MAX];
    float cos_cache[BULLET_MAX];
    for(int j = 0; j < bullet_com; j++) {
        sin_cache[j] = lv_sin_f32(angle_cache[j]);
        cos_cache[j] = lv_cos_f32(angle_cache[j]);
    }
    
    // 遍历所有子弹组
    for(int j = 0; j < bullet_com; j++) {
        // 遍历每组中的所有子弹
        for(int i = 0; i < BULLET_MAX; i++) {
            // 跳过非激活状态的子弹
            if(bullet_states[j][i] != OBJ_STATE_ACTIVE) continue;
            
            // 更新子弹位置
            if(j + 1 < center_position) {
                game_data.bullet_x[j][i] -= BULLET_SPEED * sin_cache[j];
                game_data.bullet_y[j][i] -= BULLET_SPEED * cos_cache[j];
            } else if(j + 1 == center_position) {
                game_data.bullet_y[j][i] -= BULLET_SPEED;
            } else {
                game_data.bullet_x[j][i] += BULLET_SPEED * sin_cache[j];
                game_data.bullet_y[j][i] -= BULLET_SPEED * cos_cache[j];
            }
            
            // 边界检查
            if(game_data.bullet_y[j][i] <= 0 || 
               game_data.bullet_y[j][i] >= SCREEN_HEIGHT || 
               game_data.bullet_x[j][i] <= 0 || 
               game_data.bullet_x[j][i] >= SCREEN_WIDTH) {
                bullet_states[j][i] = OBJ_STATE_FREE;
                continue;
            }
            
            // 碰撞检测
            int32_t bullet_x = game_data.bullet_x[j][i];
            int32_t bullet_y = game_data.bullet_y[j][i];
            
            for(int e = 0; e < ENEMIES; e++) {
                if(enemy_states[e] != OBJ_STATE_ACTIVE) continue;
                
                int32_t dx = bullet_x - game_data.enemy_x[e];
                int32_t dy = bullet_y - game_data.enemy_y[e];
                
                if(dx*dx + dy*dy < COLLISION_DISTANCE_SQ) {
                    if(game_score % 200 == 0 && ENEMIES < ENEMIES_MAX) {
                        ENEMIES++;
                    }
                    
                    if(game_score % 700 == 0 && ENEMY_BULLET < ENEMY_BULLET_MAX) {
                        ENEMY_BULLET++;
                    }
                    
                    if(--enemies_hp[e] == 0) {
                        game_score++;
                        bullet_states[j][i] = OBJ_STATE_FREE;
                        enemy_states[e] = OBJ_STATE_FREE;
                        
                        game_data.boom_x = game_data.enemy_x[e];
                        game_data.boom_y = game_data.enemy_y[e];
                        game_data.boom_visible = true;
                        
                        for(int k = 0; k < ENEMY_BULLET; k++) {
                            enemy_bullet_states[e][k] = OBJ_STATE_FREE;
                        }
                        
                        enemies_cnt--;
                    }
                    break;
                }
            }
        }
    }
}


// 玩家移动（添加灵敏度控制）
void move_plane(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    air_draw *state = lv_event_get_user_data(e);
    
    // 如果没有目标组件，直接返回
    if(!state->target) return;

    // 游戏暂停或结束时不处理移动
    if (GAME_TEMP || GAME_OVER) {
        return;
    }
    
    if (code == LV_EVENT_PRESSED) {
        // 按下时：记录初始触摸点和飞机位置
        lv_point_t touch_pos;
        lv_indev_get_point(lv_indev_get_act(), &touch_pos);
        
        // 记录初始触摸坐标（相对屏幕）
        state->offset_x = touch_pos.x;  // 改为存储初始触摸X
        state->offset_y = touch_pos.y;  // 改为存储初始触摸Y
        
        // 记录按下时飞机的初始位置（从共享数据获取，线程安全）
       
        state->start_plane_x = game_data.player_x;  // 需要在air_draw中新增start_plane_x/y
        state->start_plane_y = game_data.player_y;
       
        
        state->is_dragging = true;  // 标记开始拖动（需在air_draw中保留is_dragging）
    }
    else if (code == LV_EVENT_PRESSING && state->is_dragging) {
        // 拖动中：计算相对偏移并更新位置
        lv_point_t touch_pos;
        lv_indev_get_point(lv_indev_get_act(), &touch_pos);
        
        // 计算触摸点相对初始按下点的偏移量（dx=当前触摸X - 初始触摸X）
        int32_t dx = touch_pos.x - state->offset_x;
        int32_t dy = touch_pos.y - state->offset_y;
        
        // 计算新位置：初始飞机位置 + 偏移量 * 灵敏度（使用全局灵敏度变量）
        int32_t new_x = state->start_plane_x + (int32_t)(dx * sensitivity);
        int32_t new_y = state->start_plane_y + (int32_t)(dy * sensitivity);
        
        // 限制飞机在屏幕内（飞机尺寸60x60，屏幕1024x600）
        new_x = LV_CLAMP(0, new_x, 1024 - 60);  // 右边界：屏幕宽 - 飞机宽
        new_y = LV_CLAMP(0, new_y, 600 - 60);   // 下边界：屏幕高 - 飞机高
        
        lv_obj_set_pos(state->target, new_x, new_y);  // 更新UI显示
    }
    else if (code == LV_EVENT_RELEASED || code == LV_EVENT_CANCEL) {
        // 释放或取消触摸时：结束拖动
        state->is_dragging = false;
    }
}
