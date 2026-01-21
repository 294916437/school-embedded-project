#ifndef __AIR_H
#define __AIR_H

#include "../lvgl/lvgl.h"

#define M_PI 3.14159265358979323846f

// ========== 游戏对象池配置 ==========
#define BULLET_COM_MAX 7    // 最大弹道数（玩家子弹散射能力）
#define BULLET_MAX 10        // 每弹道最大子弹数
#define ENEMY_BULLET_MAX 5  // 敌机最大子弹数
#define ENEMIES_MAX 20      // 最大敌机数量

// ========== 拖动控制结构体（玩家飞机拖拽）==========
typedef struct air_draw {
    lv_obj_t *target;       // 拖动目标对象（玩家飞机）
    int32_t offset_x;       // 触摸起始X偏移
    int32_t offset_y;       // 触摸起始Y偏移
    int32_t start_plane_x;  // 拖动开始时飞机X位置
    int32_t start_plane_y;  // 拖动开始时飞机Y位置
    bool is_dragging;       // 是否正在拖动
} air_draw;

// ========== 对象状态枚举（通用状态机）==========
typedef enum {
    OBJ_STATE_FREE = 0,    // 空闲（可复用）
    OBJ_STATE_ACTIVE       // 激活（正在使用）
} obj_state_t;

// ========== 游戏核心数据结构（共享数据）==========
typedef struct {
    // 玩家数据
    int32_t player_x;      // 玩家X坐标
    int32_t player_y;      // 玩家Y坐标
    int player_hp;         // 当前血量
    int max_hp;            // 最大血量

    // 玩家子弹（二维数组：[弹道][子弹]）
    int32_t bullet_x[BULLET_COM_MAX][BULLET_MAX];
    int32_t bullet_y[BULLET_COM_MAX][BULLET_MAX];

    // 敌机数据
    int32_t enemy_x[ENEMIES_MAX];
    int32_t enemy_y[ENEMIES_MAX];

    // 敌机子弹（二维数组：[敌机][子弹]）
    int32_t enemy_bullet_x[ENEMIES_MAX][ENEMY_BULLET_MAX];
    int32_t enemy_bullet_y[ENEMIES_MAX][ENEMY_BULLET_MAX];

    // 视觉效果
    int32_t boom_x, boom_y;  // 爆炸位置
    bool boom_visible;       // 爆炸是否可见

    // 道具系统
    int32_t prop_x, prop_y;  // 道具位置
    int32_t prop_time;       // 道具存在时间
    double prop_v;           // 道具下落初速度
} game_data_t;

// ========== 全局变量声明 ==========
extern game_data_t game_data;  // 游戏数据（线程间共享）
extern air_draw air;           // 拖动控制器

// ========== 游戏配置变量 ==========
extern int bullet_com;      // 当前弹道数（1~25）
extern int bullet_tim;      // 子弹发射间隔（帧数）
extern int enemies_cnt;     // 当前激活敌机数
extern int bullet_cnt;      // 子弹计数器
extern int game_score;      // 游戏分数
extern int ENEMY_BULLET;    // 敌机子弹连发数（难度递增）
extern int ENEMIES;         // 当前最大敌机数（难度递增）
extern int GAME_OVER;       // 游戏结束标志
extern int GAME_TEMP;       // 游戏暂停标志
extern float sensitivity;   // 拖拽灵敏度
extern double angle_step;   // 子弹散射角度
extern int tempre;          // 外部中断标志

// ========== 函数声明 ==========
void sign_in_ui_init(void);         // 初始化登录界面
void game_init(void);               // 游戏初始化入口
void tempgame(lv_event_t *e);       // 暂停事件处理
static void exit_game(lv_event_t *e);      // 退出游戏
static void ui_update_handle(lv_timer_t *timer);  // UI更新定时器
void exitgg(int sig);               // SIGINT信号处理

#endif  // AIR_H