#ifndef AIR_H
#define AIR_H

#include "../lvgl/lvgl.h"

#define M_PI 3.14159265358979323846f

// 子弹配置
#define BULLET_COM_MAX 25    // 最大弹道数
#define BULLET_MAX 20       // 每弹道最大子弹数
#define ENEMY_BULLET_MAX 10 // 敌机最大子弹数

// 敌机配置
#define ENEMIES_MAX 200      // 最大敌机数量

// 拖动控制结构体（用于玩家飞机拖动）
typedef struct air_draw {
    lv_obj_t *target;       // 拖动目标组件
    int32_t offset_x;       // 触摸X偏移
    int32_t offset_y;       // 触摸Y偏移
    int32_t start_plane_x;  // 初始X位置
    int32_t start_plane_y;  // 初始Y位置
    bool is_dragging;       // 拖动状态标记
}air_draw;
// 对象状态枚举（全局使用）
typedef enum {
    OBJ_STATE_FREE = 0,    // 空闲状态
    OBJ_STATE_ACTIVE       // 激活状态
} obj_state_t;
// 游戏数据结构体（全局共享数据）
typedef struct {
    // 玩家信息
    int32_t player_x;      // 玩家X坐标
    int32_t player_y;      // 玩家Y坐标
    int player_hp;         // 当前血量
    int max_hp;            // 最大血量

    // 子弹信息（多弹道）
    int32_t bullet_x[BULLET_COM_MAX][BULLET_MAX];
    int32_t bullet_y[BULLET_COM_MAX][BULLET_MAX];

    // 敌机信息
    int32_t enemy_x[ENEMIES_MAX];
    int32_t enemy_y[ENEMIES_MAX];

    // 敌机子弹信息
    int32_t enemy_bullet_x[ENEMIES_MAX][ENEMY_BULLET_MAX];
    int32_t enemy_bullet_y[ENEMIES_MAX][ENEMY_BULLET_MAX];

    // 爆炸效果
    int32_t boom_x, boom_y;
    bool boom_visible;

    // 道具信息
    int32_t prop_x, prop_y, prop_time;
    double prop_v;
} game_data_t;

// 全局变量声明（供其他文件使用）
extern game_data_t game_data;               // 游戏核心数据
extern air_draw air;                 // 拖动控制结构体

// 全局常量与变量（跨文件使用）
extern int bullet_com;          // 弹道数量
extern int bullet_tim;          // 发射间隔
extern int enemies_cnt;         // 当前敌机数量
extern int bullet_cnt;          // 子弹计数
extern int game_score;          // 游戏分数
extern int ENEMY_BULLET;        // 敌机子弹数量
extern int ENEMIES;             // 敌机总数
extern int GAME_OVER;           // 游戏结束标记
extern int GAME_TEMP;           // 游戏暂停标记
extern float sensitivity;       // 灵敏度设置
extern double angle_step; //子弹间隔
extern int tempre;
// 函数声明（跨文件调用）
void sign_in_ui_init(void);     // 初始化登录界面
void game_init(void);           // 游戏初始化
void tempgame(lv_event_t *e);   // 游戏暂停
static void exit_game(lv_event_t *e);  // 退出游戏
static void ui_update_handle(lv_timer_t *timer);  // UI更新回调
void exitgg(int sig);
#endif  // AIR_H