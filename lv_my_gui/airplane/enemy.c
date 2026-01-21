#include "enemy.h"
#include "air.h"
#include "player.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

// 敌机状态数组定义
obj_state_t enemy_states[ENEMIES_MAX] = {0};
obj_state_t enemy_bullet_states[ENEMIES_MAX][ENEMY_BULLET_MAX] = {0};
int enemies_hp[ENEMIES_MAX] = {0};
int enemies_bullrt_num[ENEMIES_MAX] = {0};
int enemies_bullet_cnt[ENEMIES_MAX][ENEMY_BULLET_MAX] = {0};
int enemies_speed = 5;

// 随机数生成
int32_t lv_random_int(int32_t min, int32_t max) {
    // ========== 边界检查 ==========
    if (min >= max) return min;  // 防止除0错误
    
    // ========== 生成随机数 ==========
    return min + rand() % (max - min + 1);
}


// 生成敌机（逻辑层）
void enemy_act_logic() {
    // ========== 第1步：检查数量上限 ==========
    if(enemies_cnt >= ENEMIES) return;  // ENEMIES是动态上限（10→200）
    
    // ========== 第2步：寻找空闲位置 ==========
    for(int i = 0; i < ENEMIES; i++) {
        if(enemy_states[i] == OBJ_STATE_FREE) {
            // ========== 第3步：激活敌机 ==========
            enemy_states[i] = OBJ_STATE_ACTIVE;
            
            // ========== 第4步：设置初始属性 ==========
            enemies_hp[i] = 1;  // 初始血量为1
            
            // ========== 第5步：随机选择生成方向 ==========
            int rand = lv_random_int(1, 3);
            
            // ========== 第6步：根据方向设置坐标 ==========
            if(rand == 1) {  // 从上方生成
                game_data.enemy_x[i] = lv_random_int(50, 920);   // X: [50, 920]
                game_data.enemy_y[i] = lv_random_int(50, 100);   // Y: [50, 100]
            }
            else if(rand == 2) {  // 从左侧生成
                game_data.enemy_x[i] = lv_random_int(50, 100);   // X: [50, 100]
                game_data.enemy_y[i] = lv_random_int(0, 400);    // Y: [0, 400]
            }
            else if(rand == 3) {  // 从右侧生成
                game_data.enemy_x[i] = lv_random_int(950, 1024); // X: [950, 1024]
                game_data.enemy_y[i] = lv_random_int(0, 400);    // Y: [0, 400]
            }
            
            // ========== 第7步：更新计数 ==========
            enemies_cnt++;  // 当前敌机数+1
            break;          // 只生成1个敌机，退出循环
        }
    }
}

// 为每一个敌机生成子弹（逻辑层）
void fire_enemybullet_logic() {
    // ========== 遍历所有敌机 ==========
    for(int enemies_ = 0; enemies_ < ENEMIES; enemies_++) {
        // ========== 只处理激活的敌机 ==========
        if(enemy_states[enemies_] == OBJ_STATE_ACTIVE) {
            // ========== 获取当前子弹索引（循环队列）==========
            int bullet_idx = enemies_bullrt_num[enemies_];
            
            // ========== 检查该位置是否空闲 ==========
            if(enemy_bullet_states[enemies_][bullet_idx] == OBJ_STATE_FREE) {
                // ========== 激活子弹 ==========
                enemy_bullet_states[enemies_][bullet_idx] = OBJ_STATE_ACTIVE;

                // 重置生命周期
                 enemies_bullet_cnt[enemies_][bullet_idx] = 0;
                
                // ========== 设置初始坐标（与敌机位置相同）==========
                game_data.enemy_bullet_x[enemies_][bullet_idx] = game_data.enemy_x[enemies_];
                game_data.enemy_bullet_y[enemies_][bullet_idx] = game_data.enemy_y[enemies_];
                
                // ========== 更新索引（循环）==========
                enemies_bullrt_num[enemies_] = (enemies_bullrt_num[enemies_] + 1) % ENEMY_BULLET;
            }
        }
    }
}

// 敌机子弹追踪玩家（逻辑层）
void move_enemybullet_logic() {
    // ========== 第1步：遍历所有敌机 ==========
    for(int enemies_ = 0; enemies_ < ENEMIES; enemies_++) {
        // ========== 第2步：遍历该敌机的所有子弹 ==========
        for(int enemies_bul = 0; enemies_bul < ENEMY_BULLET; enemies_bul++) {
            // ========== 第3步：跳过非激活子弹 ==========
            if(enemy_bullet_states[enemies_][enemies_bul] != OBJ_STATE_ACTIVE)
                continue;

            // ========== 第4步：生命周期检测 ==========
            if(enemies_bullet_cnt[enemies_][enemies_bul] >= 55) {
                enemy_bullet_states[enemies_][enemies_bul] = OBJ_STATE_FREE;
                enemies_bullet_cnt[enemies_][enemies_bul] = 0;
                continue;  // 销毁子弹，跳过后续处理
            }

            // ========== 第5步：计算到玩家的向量 ==========
            double dx = game_data.player_x - game_data.enemy_bullet_x[enemies_][enemies_bul];
            double dy = game_data.player_y - game_data.enemy_bullet_y[enemies_][enemies_bul];
            double distance_squared = dx*dx + dy*dy;

            // ========== 第6步：碰撞检测（玩家被击中）==========
            if(distance_squared < 100) {  // 10像素碰撞半径
                // ========== 第7步：玩家受伤 ==========
                game_data.player_hp--;
        
                // ========== 第8步：子弹失效 ==========
                enemy_bullet_states[enemies_][enemies_bul] = OBJ_STATE_FREE;
                enemies_bullet_cnt[enemies_][enemies_bul] = 0;

                // ========== 第9步：检查游戏结束 ==========
                if (game_data.player_hp <= 0) {
                    // 清空所有玩家子弹
                    for(int j = 0; j < BULLET_COM_MAX; j++) {
                        for(int i = 0; i < BULLET_MAX; i++) {
                            bullet_states[j][i] = OBJ_STATE_FREE;
                        }
                    }
                    
                    // 清空所有敌机和敌机子弹
                    for(int i = 0; i < ENEMIES; i++) {
                        enemy_states[i] = OBJ_STATE_FREE;
                        for(int j = 0; j < ENEMY_BULLET; j++) {
                            enemy_bullet_states[i][j] = OBJ_STATE_FREE;
                        }
                    }
                    
                    GAME_OVER = 1;  // 设置游戏结束标志
                    break;  // 退出子弹循环
                }
            }

            // ========== 第10步：计算距离（使用LVGL的sqrt）==========
            lv_sqrt_res_t tt;
            lv_sqrt(distance_squared, &tt, 0x8000);
            
            double distance = tt.i + tt.f;  // 整数部分 + 小数部分
            
            // ========== 第11步：计算移动向量 ==========
            double speed = 15;  // 子弹速度（像素/帧）
            double move_x = (dx / distance) * speed;  // X方向分量
            double move_y = (dy / distance) * speed;  // Y方向分量

            // ========== 第12步：更新子弹位置 ==========
            game_data.enemy_bullet_x[enemies_][enemies_bul] += move_x;
            game_data.enemy_bullet_y[enemies_][enemies_bul] += move_y;
            
            // ========== 第13步：生命周期递增 ==========
            enemies_bullet_cnt[enemies_][enemies_bul]++;
        }
    }
}

// 敌机自由移动（逻辑层）
void move_enemiesplane_logic() {
    int rand;
    
    // ========== 第1步：遍历所有敌机 ==========
    for(int enemies_ = 0; enemies_ < ENEMIES; enemies_++) {
        // ========== 第2步：只处理激活的敌机 ==========
        if(enemy_states[enemies_] != OBJ_STATE_ACTIVE)
            continue;
        
        // ========== 第3步：根据位置调整随机范围（AI策略）==========
        if(game_data.enemy_x[enemies_] < 200) {
            // 靠近左边界，倾向向右移动
            rand = lv_random_int(1, 5);  // 1,2,3,4,5 → 少了向左(0)
        }
        else if(game_data.enemy_x[enemies_] > 700) {
            // 靠近右边界，倾向向左移动
            rand = lv_random_int(0, 4);  // 0,1,2,3,4 → 少了向右(5)
        }
        else {
            // 在中间区域，4个方向均衡
            rand = lv_random_int(1, 4);  // 1,2,3,4
        }
        
        // ========== 第4步：根据随机数执行移动 ==========
        switch (rand) {
            // ========== 向左移动 ==========
            case 0:
            case 1:
                game_data.enemy_x[enemies_] -= enemies_speed;  // 速度=5
                
                // 边界检测
                if(game_data.enemy_x[enemies_] < 0) {
                    enemy_states[enemies_] = OBJ_STATE_FREE;  // 销毁敌机
                    enemies_cnt--;
                    
                    // 销毁该敌机的所有子弹
                    for(int j = 0; j < ENEMY_BULLET; j++) {
                        enemy_bullet_states[enemies_][j] = OBJ_STATE_FREE;
                    }
                }
                break;
            
            // ========== 向右移动 ==========
            case 5:
            case 2:
                game_data.enemy_x[enemies_] += enemies_speed;
                
                if(game_data.enemy_x[enemies_] > 1024) {
                    enemy_states[enemies_] = OBJ_STATE_FREE;
                    enemies_cnt--;
                    for(int j = 0; j < ENEMY_BULLET; j++) {
                        enemy_bullet_states[enemies_][j] = OBJ_STATE_FREE;
                    }
                }
                break;
            
            // ========== 向下移动 ==========
            case 3:
                game_data.enemy_y[enemies_] += enemies_speed;
                
                if(game_data.enemy_y[enemies_] > 600) {
                    enemy_states[enemies_] = OBJ_STATE_FREE;
                    enemies_cnt--;
                    for(int j = 0; j < ENEMY_BULLET; j++) {
                        enemy_bullet_states[enemies_][j] = OBJ_STATE_FREE;
                    }
                }
                break;
            
            // ========== 向上移动 ==========
            case 4:
                game_data.enemy_y[enemies_] -= enemies_speed;
                
                if(game_data.enemy_y[enemies_] < 0) {
                    enemy_states[enemies_] = OBJ_STATE_FREE;
                    enemies_cnt--;
                    for(int j = 0; j < ENEMY_BULLET; j++) {
                        enemy_bullet_states[enemies_][j] = OBJ_STATE_FREE;
                    }
                }
                break;
        }
    }
}
