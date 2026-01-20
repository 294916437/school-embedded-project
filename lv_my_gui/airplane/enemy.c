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
    // ... 原有代码保持不变 ...
    if (min >= max) return min;  // [min, max] 范围
    return min + rand() % (max - min + 1);
}

//...
// 生成敌机（逻辑层）
void enemy_act_logic() {
    // ... 原有代码保持不变 ...
    if(enemies_cnt >= ENEMIES) return;
    
    for(int i = 0; i < ENEMIES; i++) {
        if(enemy_states[i] == OBJ_STATE_FREE) {
            enemy_states[i] = OBJ_STATE_ACTIVE;
            
            enemies_hp[i] = 1;
            int rand = lv_random_int(1, 3);
            
            if(rand == 1) {
                game_data.enemy_x[i] = lv_random_int(50, 920);
                game_data.enemy_y[i] = lv_random_int(50, 100);
            }
            else if(rand == 2) {
                game_data.enemy_x[i] = lv_random_int(50, 100);
                game_data.enemy_y[i] = lv_random_int(0, 400);    
            }
            else if(rand == 3) {
                game_data.enemy_x[i] = lv_random_int(950, 1024);
                game_data.enemy_y[i] = lv_random_int(0, 400);    
            }
            
            enemies_cnt++;
            break;
        }
    }
}
// 为每一个敌机生成子弹（逻辑层）
void fire_enemybullet_logic() {
    // ... 原有代码保持不变 ...
    for(int enemies_ = 0; enemies_ < ENEMIES; enemies_++) {
        if(enemy_states[enemies_] == OBJ_STATE_ACTIVE) {
            int bullet_idx = enemies_bullrt_num[enemies_];
            if(enemy_bullet_states[enemies_][bullet_idx] == OBJ_STATE_FREE) {
                enemy_bullet_states[enemies_][bullet_idx] = OBJ_STATE_ACTIVE;
                game_data.enemy_bullet_x[enemies_][bullet_idx] = game_data.enemy_x[enemies_];
                game_data.enemy_bullet_y[enemies_][bullet_idx] = game_data.enemy_y[enemies_];
                enemies_bullrt_num[enemies_] = (enemies_bullrt_num[enemies_] + 1) % ENEMY_BULLET;
            }
        }
    }
}

// 敌机子弹追踪玩家（逻辑层）
void move_enemybullet_logic() {
    // ... 原有代码保持不变，新增玩家受伤逻辑 ...
    for(int enemies_ = 0; enemies_ < ENEMIES; enemies_++) {
        for(int enemies_bul = 0; enemies_bul < ENEMY_BULLET; enemies_bul++) {
            if(enemy_bullet_states[enemies_][enemies_bul] != OBJ_STATE_ACTIVE)
                continue;

            // 子弹超过生命周期，销毁
            if(enemies_bullet_cnt[enemies_][enemies_bul] >= 55) {
                enemy_bullet_states[enemies_][enemies_bul] = OBJ_STATE_FREE;
                enemies_bullet_cnt[enemies_][enemies_bul] = 0;
                continue;
            }

            // 计算子弹到玩家的向量
            double dx = game_data.player_x - game_data.enemy_bullet_x[enemies_][enemies_bul];
            double dy = game_data.player_y - game_data.enemy_bullet_y[enemies_][enemies_bul];
            double distance_squared = dx*dx + dy*dy;

            // 检测碰撞（玩家被击中）
            if(distance_squared < 100) { 
            
                game_data.player_hp--;
        
                // 子弹失效处理
                enemy_bullet_states[enemies_][enemies_bul] = OBJ_STATE_FREE;
                enemies_bullet_cnt[enemies_][enemies_bul] = 0;

                // 血量为0时游戏结束
                if (game_data.player_hp <= 0) {
                    // 清空所有子弹和敌机
                    for(int j = 0;j < BULLET_COM_MAX ; j++)
                    {
                        for(int i = 0; i < BULLET_MAX; i++)
                        {
                            bullet_states[j][i] = OBJ_STATE_FREE;//玩家子弹初始化
                        }
                    }
                    for(int i = 0; i < ENEMIES; i++) {
                        enemy_states[i] = OBJ_STATE_FREE;//敌机初始化

                        // 初始化敌机子弹池
                        for(int j = 0; j < ENEMY_BULLET; j++) {
                            enemy_bullet_states[i][j] = OBJ_STATE_FREE;
                        }
                    }
                    GAME_OVER = 1;
                    break;
                }
            }

            // 计算单位向量，乘以移动速度
            lv_sqrt_res_t tt;
            lv_sqrt(distance_squared,&tt,0x8000);
            
            double distance = tt.i+tt.f;
            double speed = 15;
            double move_x = (dx / distance) * speed;
            double move_y = (dy / distance) * speed;

            // 更新子弹位置
            game_data.enemy_bullet_x[enemies_][enemies_bul] += move_x;
            game_data.enemy_bullet_y[enemies_][enemies_bul] += move_y;
            enemies_bullet_cnt[enemies_][enemies_bul]++;
        }
    }
}

// 敌机自由移动（逻辑层）
void move_enemiesplane_logic() {
    // ... 原有代码保持不变 ...
    int rand;
    for(int enemies_ = 0; enemies_ < ENEMIES; enemies_++) {
        if(enemy_states[enemies_] != OBJ_STATE_ACTIVE)
            continue;
        if(game_data.enemy_x[enemies_] < 200)
        {
            rand = lv_random_int(1, 5);
        }
        else if ( game_data.enemy_x[enemies_] > 700)
        {
            rand = lv_random_int(0, 4);
        }
        else
        {
            rand = lv_random_int(1, 4);
        }
        
        switch (rand) {
            // 左走
            case 0:
            case 1:
                game_data.enemy_x[enemies_] -= enemies_speed;
                if(game_data.enemy_x[enemies_] < 0) {
                    enemy_states[enemies_] = OBJ_STATE_FREE;
                    enemies_cnt--;
                    // 标记该敌机的所有子弹为空闲
                    for(int j = 0; j < ENEMY_BULLET; j++) {
                        enemy_bullet_states[enemies_][j] = OBJ_STATE_FREE;
                    }
                }
                break;
            // 右走
            case 5:
            case 2:
                game_data.enemy_x[enemies_] += enemies_speed;
                if(game_data.enemy_x[enemies_] > 1024) {
                    enemy_states[enemies_] = OBJ_STATE_FREE;
                    enemies_cnt--;
                    // 标记该敌机的所有子弹为空闲
                    for(int j = 0; j < ENEMY_BULLET; j++) {
                        enemy_bullet_states[enemies_][j] = OBJ_STATE_FREE;
                    }
                }
                break;
            // 下走
            case 3:
                game_data.enemy_y[enemies_] += enemies_speed;
                if(game_data.enemy_y[enemies_] > 600) {
                    enemy_states[enemies_] = OBJ_STATE_FREE;
                    enemies_cnt--;
                    // 标记该敌机的所有子弹为空闲
                    for(int j = 0; j < ENEMY_BULLET; j++) {
                        enemy_bullet_states[enemies_][j] = OBJ_STATE_FREE;
                    }
                }
                break;
            // 上走
            case 4:
                game_data.enemy_y[enemies_] -= enemies_speed;
                if(game_data.enemy_y[enemies_] < 0) {
                    enemy_states[enemies_] = OBJ_STATE_FREE;
                    enemies_cnt--;
                    // 标记该敌机的所有子弹为空闲
                    for(int j = 0; j < ENEMY_BULLET; j++) {
                        enemy_bullet_states[enemies_][j] = OBJ_STATE_FREE;
                    }
                }
                break;
        }
    }
}
