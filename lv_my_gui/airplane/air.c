#include "../lvgl/lvgl.h"
#include "air.h"
#include "player.h"
#include "enemy.h"
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include "data.h"
#include "../modules/buzzer.h"  // 添加蜂鸣器头文件
#include "../modules/LED.h"  // 添加LED头文件
#include "lv_run_main.h"

static void *game_logic_func(void *arg);
void startgame(lv_event_t * e);

// 添加LED相关函数声明（在文件开头，static void *game_logic_func 之后）
static void led_blink_timer_cb(lv_timer_t *timer);
static void update_led_by_hp(void);
static void buzz_off_timer_cb(lv_timer_t *timer);

/* 游戏元素对象 */
static lv_obj_t *screen; //屏幕
static lv_obj_t *background ; //游戏背景
static lv_style_t style_background;
    
static lv_obj_t *player; //玩家飞机
static lv_obj_t *bullets[BULLET_COM_MAX][BULLET_MAX] = {{NULL}}; //玩家子弹对象池
static lv_obj_t *killenemy;   //爆炸特效
static lv_style_t style_player,style_bullet,style_killenemy; 

static lv_obj_t *enemies[ENEMIES_MAX] = {NULL}; //敌机对象池, 最大敌机数量为200
static lv_obj_t *enemy_bullets[ENEMIES_MAX][ENEMY_BULLET_MAX] = {{NULL}}; //敌机子弹对象池, 最大子弹连发数量为5
static lv_style_t style_enemy_small,style_enemy_midle,style_enemybullet;

static lv_obj_t *gametemp,*temp; //游戏暂停, 暂停界面
static lv_style_t style_gametemp,style_temp;

static lv_obj_t *gameover,*finalscore,*gameexitbt,*gamerestartbt,*backhomebt;//结束界面, 最终得分, 退出游戏, 重新开始
static lv_style_t style_gameover,style_finalscore,style_gameexitbt,style_gamerestartbt,style_backhomebtn;

static lv_obj_t *signback,*usr,*passd,*sign_in,*signbt; //登入背景  //用户  //密码  //登入界面  //登入按钮
static lv_style_t style_signback,style_signin,style_signinbt ;//登入界面


// 在全局变量区域添加LED闪烁相关变量
static lv_timer_t *led_blink_timer = NULL; // LED闪烁定时器
static bool led_blink_state = false; // 当前LED闪烁状态
static bool led_blinking = false; // 是否正在闪烁


// 状态显示组件
static lv_obj_t *hp_bar,*firepower_label, *score_label ;//血条   //火力值标签   //分数标签   
static lv_style_t style_hp_bar,style_hp_bg,style_firepower;// 血条样式 //火力值

static lv_obj_t *superfire; //火力提升道具
static lv_style_t style_superfire; //火力提升道具

static lv_obj_t * label; 

// 设置界面组件

static lv_obj_t *setting_panel,*slider, *sensitivity_label, *close_setting_btn;//设置面板   //灵敏度滑动条  //灵敏度标签    //关闭设置按钮
static lv_style_t style_setting_panel,style_close_setting;// 设置面板样式

int bullet_com = 1,bullet_tim = 16,bullet_cnt = 0;//弹道数量 //玩家火力值 //子弹数量
double angle_step = 8.0; //子弹散射角度
float sensitivity = 1.2f;  //拖拽灵敏度
int game_score = 0; //得分
int enemies_cnt = 0,ENEMY_BULLET = 1,ENEMIES = 10;//动态敌机数量 //敌机子弹连发数量 //当前最大敌机数量
int GAME_OVER = 1,GAME_TEMP = 0;//游戏结束标志 // 游戏暂停标志
char *usr_str , *passd_str;//用户，密码匹配

int tempre = 0;         // 外部中断标志
User user_list[10]; // 用户列表（最多10个用户）
static lv_timer_t *game_timer; // 游戏UI更新定时器
static pthread_t game_logic_thread; // 游戏逻辑线程
static sem_t logic_sem; // 逻辑线程信号量
static pthread_mutex_t game_mutex; // 游戏数据互斥锁
game_data_t game_data; // 游戏数据（线程间共享）
air_draw air;          // 拖动控制器
static obj_state_t prop_state = OBJ_STATE_FREE; //道具状态

//星空主题
char *sky_theme[] = { 
    "A:img/background.png",
    "A:img/player.bmp",
    "A:img/enemy_m.bmp",
    "A:img/enemy_n.bmp",
    "A:img/bullet.bmp",
    "A:img/enmy_bullet.bmp",
    "A:img/boom.bmp",
    "A:img/superfire.bmp",
    "A:img/sign_in.png",
    "A:img/signin.bmp",
    "A:img/signinbt.bmp",
    "A:img/gametemp.bmp",
    "A:img/temp.bmp",
    "A:img/game_over.png",
    "A:img/restart.png",
    "A:img/back_to_home.png"
};
/***************************************全局变量定义*******************************************/

/**
 * @brief 设置游戏界面中各个元素的样式
 * 该函数初始化并配置了游戏中所有UI元素的样式，包括背景、玩家飞机、敌机、子弹、爆炸效果、
 * 登录界面、游戏暂停界面、游戏结束界面、血条、火力值标签等
 */
static void style_set(){
    
    // 背景样式（使用星空图片）
    lv_style_init(&style_background);
    lv_style_set_bg_img_src(&style_background,sky_theme[0]);  // 设置背景图片
    
    // 玩家飞机样式（使用飞机图片）
    lv_style_init(&style_player);
    lv_style_set_bg_img_src(&style_player, sky_theme[1]);  // 设置飞机图片
    lv_style_set_bg_opa(&style_player, LV_OPA_TRANSP);      // 设置背景透明

    // 敌人小飞机样式
    lv_style_init(&style_enemy_small);
    lv_style_set_bg_img_src(&style_enemy_small, sky_theme[2]);  // 设置敌机图片
    lv_style_set_bg_opa(&style_enemy_small, LV_OPA_TRANSP);       // 设置背景透明
    // 敌人中等飞机样式
    lv_style_init(&style_enemy_midle);
    lv_style_set_bg_img_src(&style_enemy_midle,sky_theme[3] );  // 设置敌机图片
    lv_style_set_bg_opa(&style_enemy_midle, LV_OPA_TRANSP);       // 设置背景透明

    // 子弹样式
    lv_style_init(&style_bullet);
    lv_style_set_bg_img_src(&style_bullet, sky_theme[4]);  // 设置子弹图片
    lv_style_set_bg_opa(&style_bullet, LV_OPA_TRANSP);
    
    // 敌机子弹样式
    lv_style_init(&style_enemybullet);
    lv_style_set_bg_img_src(&style_enemybullet,sky_theme[5]);  // 设置子弹图片
    lv_style_set_bg_opa(&style_enemybullet, LV_OPA_TRANSP);

    // 爆炸样式
    lv_style_init(&style_killenemy);
    lv_style_set_bg_img_src(&style_killenemy,sky_theme[6] );  // 设置爆炸图片
    lv_style_set_bg_opa(&style_killenemy, LV_OPA_TRANSP);
    
    // 提升火力道具样式
    lv_style_init(&style_superfire);
    lv_style_set_bg_img_src(&style_superfire,sky_theme[7]);  // 设置爆炸图片
    lv_style_set_bg_opa(&style_superfire, LV_OPA_TRANSP);

    
    // 登入界面背景样式
    lv_style_init(&style_signback);
    lv_style_set_bg_img_src(&style_signback,sky_theme[8]);  // 设置登入界面背景图片
    lv_style_set_bg_opa(&style_signback, LV_OPA_TRANSP);
    
    //登入界面样式
    lv_style_init(&style_signin);
    lv_style_set_bg_img_src(&style_signin,sky_theme[9]); //设置登入界面图片
    lv_style_set_bg_opa(&style_signback, LV_OPA_TRANSP);
    
    
    //登入界面按钮样式
    lv_style_init(&style_signinbt);
    lv_style_set_bg_img_src(&style_signinbt,sky_theme[10]); //设置登入界面按钮图片
    lv_style_set_bg_opa(&style_signinbt, LV_OPA_TRANSP);
    
    //游戏暂停按钮样式
    lv_style_init(&style_gametemp);
    lv_style_set_bg_img_src(&style_gametemp,sky_theme[11]); //设置游戏暂停按钮图片
    lv_style_set_bg_opa(&style_gametemp, LV_OPA_TRANSP);
   
    //游戏暂停样式
    lv_style_init(&style_temp);
    lv_style_set_bg_img_src(&style_temp,sky_theme[12]); //设置游戏暂停图片
    lv_style_set_bg_opa(&style_temp, LV_OPA_TRANSP); 

    //结束界面样式
    lv_style_init(&style_gameover);
    lv_style_set_bg_img_src(&style_gameover,sky_theme[13]); //设置登入界面按钮图片
    lv_style_set_bg_opa(&style_gameover, LV_OPA_TRANSP);
    
    //结束界面得分展示样式
    lv_style_init(&style_finalscore);
    lv_style_set_text_font(&style_finalscore, &lv_font_montserrat_32); // 大号字体
    lv_style_set_text_color(&style_finalscore, lv_color_hex(0xFFFFFF)); // 白色文字
    lv_style_set_text_opa(&style_finalscore, LV_OPA_COVER);

    //结束界面样式
    lv_style_init(&style_gamerestartbt);
    lv_style_set_bg_img_src(&style_gamerestartbt,sky_theme[14]); //设置登入界面按钮图片
    lv_style_set_bg_opa(&style_gamerestartbt, LV_OPA_TRANSP);

    // 血条样式
    lv_style_init(&style_hp_bar);
    lv_style_set_bg_color(&style_hp_bar, lv_color_hex(0xFF0000)); // 红色血量
    lv_style_set_radius(&style_hp_bar, 5);
    lv_style_set_bg_opa(&style_hp_bar, LV_OPA_COVER);

    // 血条背景样式
    lv_style_init(&style_hp_bg);
    lv_style_set_bg_color(&style_hp_bg, lv_color_hex(0x333333)); // 灰色背景
    lv_style_set_border_color(&style_hp_bg, lv_color_hex(0xFFFFFF));
    lv_style_set_border_width(&style_hp_bg, 2);
    lv_style_set_radius(&style_hp_bg, 5);
    lv_style_set_bg_opa(&style_hp_bg, LV_OPA_COVER);

    // 火力值标签样式
    lv_style_init(&style_firepower);
    lv_style_set_text_font(&style_firepower, &lv_font_montserrat_20);
    lv_style_set_text_color(&style_firepower, lv_color_hex(0x00FFFF)); // 青色文字

    // 设置面板样式
    lv_style_init(&style_setting_panel);
    lv_style_set_bg_color(&style_setting_panel, lv_color_hex(0x1A1A2E)); // 深色背景
    lv_style_set_border_color(&style_setting_panel, lv_color_hex(0xFFFFFF));
    lv_style_set_border_width(&style_setting_panel, 2);
    lv_style_set_radius(&style_setting_panel, 10);
    lv_style_set_bg_opa(&style_setting_panel, LV_OPA_90);

    // 退出按钮样式
    lv_style_init(&style_gameexitbt);
    lv_style_set_bg_opa(&style_gameexitbt, LV_OPA_0);

    // 回到主页按钮样式
    lv_style_init(&style_backhomebtn);
    lv_style_set_bg_opa(&style_backhomebtn, LV_OPA_0);

}


// 初始化子弹对象池
static void init_bullet_pool() {

    for(int j = 0; j < BULLET_COM_MAX; j++) {  // 25条弹道
        for(int i = 0; i < BULLET_MAX; i++) {  // 每条20发子弹
            // 创建子弹对象
            bullets[j][i] = lv_obj_create(background);  // 父对象为背景
            
            // 移除默认样式（避免边框、阴影等）
            lv_obj_remove_style_all(bullets[j][i]);
            
            // 禁止滚动
            lv_obj_set_scroll_dir(bullets[j][i], false);
            
            // 设置尺寸
            lv_obj_set_size(bullets[j][i], 60, 60);
            
            // 应用子弹样式
            lv_obj_add_style(bullets[j][i], &style_bullet, LV_STATE_DEFAULT);
            
            // 初始隐藏（未激活）
            lv_obj_add_flag(bullets[j][i], LV_OBJ_FLAG_HIDDEN);
            
            // 设置逻辑状态为空闲
            bullet_states[j][i] = OBJ_STATE_FREE;
        }
    }
}

// 初始化敌机对象池
static void init_enemy_pool() {
    
    for(int i = 0; i < ENEMIES_MAX; i++) {  // 200个敌机
        enemies[i] = lv_obj_create(background);
        lv_obj_remove_style_all(enemies[i]);
        lv_obj_set_scroll_dir(enemies[i], false);
        lv_obj_set_size(enemies[i], 60, 60);
        lv_obj_add_style(enemies[i], &style_enemy_small, LV_STATE_DEFAULT);
        lv_obj_add_flag(enemies[i], LV_OBJ_FLAG_HIDDEN);
        enemy_states[i] = OBJ_STATE_FREE;

        // 初始化敌机子弹池（嵌套循环）
        for(int j = 0; j < ENEMY_BULLET_MAX; j++) {  // 每个敌机10发子弹
            enemy_bullets[i][j] = lv_obj_create(background);
            lv_obj_remove_style_all(enemy_bullets[i][j]);
            lv_obj_set_size(enemy_bullets[i][j], 20, 20);  // 敌机子弹更小
            lv_obj_add_style(enemy_bullets[i][j], &style_enemybullet, LV_STATE_DEFAULT);
            lv_obj_add_flag(enemy_bullets[i][j], LV_OBJ_FLAG_HIDDEN);
            enemy_bullet_states[i][j] = OBJ_STATE_FREE;
        }
    }
}

// 添加关闭暂停面板的回调函数
static void close_pause_panel(lv_event_t *e)
{
    if(e->code == LV_EVENT_CLICKED)
    {
        // 恢复游戏
        GAME_TEMP = 0;
        lv_timer_resume(game_timer);
        
        // 恢复时根据血量更新LED
        update_led_by_hp();
        
        // 隐藏暂停面板
        lv_obj_add_flag(setting_panel, LV_OBJ_FLAG_HIDDEN);
        
        printf("游戏恢复\n");
    }
}

// 在文件顶部添加蜂鸣器定时器回调函数
static void buzz_off_timer_cb(lv_timer_t *timer) {
    buzz_ctrl(0, 0);  // 关闭蜂鸣器
    lv_timer_del(timer);  // 删除定时器
}

// 添加LED闪烁定时器回调函数（在buzz_off_timer_cb之后）
static void led_blink_timer_cb(lv_timer_t *timer) {
    led_blink_state = !led_blink_state;
    // 控制所有4个LED同步闪烁
    if(led_blink_state) {
        led_ctrl(-1, 1);  // 全部点亮
    } else {
        led_ctrl(-1, 0);  // 全部熄灭
    }
}

// 添加LED控制函数（根据血量百分比）
static void update_led_by_hp() {
    // ========== 游戏结束或暂停时关闭LED ==========
    if (GAME_OVER || GAME_TEMP) {
        // 停止闪烁定时器
        if (led_blink_timer) {
            lv_timer_del(led_blink_timer);
            led_blink_timer = NULL;
        }
        led_blinking = false;
        led_ctrl(-1, 0);  // 关闭所有LED
        return;
    }

    // ========== 计算血量百分比 ==========
    int hp_percent = (game_data.player_hp * 100) / game_data.max_hp;
    //                ^^^^^^^^^^^^^^^^^^^^^
    //                当前HP * 100 / 最大HP

    // ========== 根据血量决定LED状态 ==========
    if (hp_percent > 40) {
        // 血量 > 40%: LED长亮
        if (led_blinking) {  // 如果之前在闪烁，停止闪烁
            // 删除旧定时器
            if (led_blink_timer) {  
                lv_timer_del(led_blink_timer);
                led_blink_timer = NULL;
            }
            led_blinking = false;
        }
        led_ctrl(-1, 1);  // 所有LED点亮
    } else {
        // 血量 ≤ 40%: LED闪烁（警告）
        if (!led_blinking) {  // 如果之前未闪烁，开始闪烁
            // 确保之前定时器已清理
            if (led_blink_timer) {
                lv_timer_del(led_blink_timer);
            }
            led_blinking = true;
            // 创建500ms间隔的闪烁定时器
            led_blink_timer = lv_timer_create(led_blink_timer_cb, 500, NULL);
        }
    }
}

//CTRL+C 外部中断处理
/**
 * 处理退出信号的函数
 * @param sig 接收到的信号编号
 */
void exitgg(int sig)
{
    // 检查信号是否为2（SIGINT，通常由Ctrl+C触发）
    if(sig == 2)
    {
        
        // 输出电源关闭信息
        printf("Power Off\n");
        // 设置退出标志为1
        tempre = 1;
    }
}



//游戏暂停
void pause_game(lv_event_t *e) {
    if (e->code == LV_EVENT_CLICKED) {
        if (GAME_TEMP == 0) {  // 当前未暂停
            // ========== 暂停游戏 ==========
            GAME_TEMP = 1;  // 🔑 设置暂停标志
            
            // 保存当前分数
            update_score((char *)usr_str, game_score);
            
            // 暂停UI更新定时器
            lv_timer_pause(game_timer);
            
            // 暂停时关闭LED（在update_led_by_hp中检测GAME_TEMP）
            update_led_by_hp();
            
            // 显示暂停面板
            lv_obj_clear_flag(setting_panel, LV_OBJ_FLAG_HIDDEN);
            
            printf("游戏暂停\n");
        } else {  // 当前已暂停
            // ========== 恢复游戏 ==========
            GAME_TEMP = 0;  // 🔑 清除暂停标志
            
            // 恢复UI更新定时器
            lv_timer_resume(game_timer);
            
            // 恢复LED状态（根据当前血量）
            update_led_by_hp();
            
            // 隐藏暂停面板
            lv_obj_add_flag(setting_panel, LV_OBJ_FLAG_HIDDEN);
            
            printf("游戏恢复\n");
        }
    }
}
static void lv_game_event_handler(lv_event_t * event)
{
    lv_obj_t* obj = lv_event_get_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    if(code == LV_EVENT_CLICKED)
    {
        // ========== 第1步：清理硬件资源 ==========
        buzz_ctrl(0, 0);  // 关闭蜂鸣器
        
        if (led_blink_timer) {
            lv_timer_del(led_blink_timer);
            led_blink_timer = NULL;
        }
        led_blinking = false;
        led_ctrl(-1, 0);  // 关闭所有LED

        // ========== 第2步：保存游戏数据 ==========
        if(usr_str != NULL && passd_str != NULL) {
            update_score((char *)usr_str, game_score);
        }

        // ========== 第3步：标记游戏结束 ==========
        GAME_OVER = 1;
        usr_str = NULL;
        passd_str = NULL;

        // ========== 第4步：清理线程资源 ==========
        if (game_timer) {
            lv_timer_del(game_timer);
            game_timer = NULL;
        }

        if (game_logic_thread != 0) {
            pthread_join(game_logic_thread, NULL);
            game_logic_thread = 0;
        }

        sem_destroy(&logic_sem);
        pthread_mutex_destroy(&game_mutex);

        // ========== 第5步：删除游戏界面 ==========
        lv_obj_del(screen);  // 删除整个游戏屏幕（包含所有子对象）
        
        // ========== 第6步：重建主界面 ==========
        main_grid(&style);  // 调用主菜单创建函数
        
        printf("已返回主界面\n");
    }
}

//游戏重新开始
void restartgame(lv_event_t *e) {
    // ========== 保存分数 ==========
    update_score((char *)usr_str, game_score);
    
    // ========== 清理硬件状态 ==========
    buzz_ctrl(0, 0);  // 关闭蜂鸣器
    
    if (led_blink_timer) {
        lv_timer_del(led_blink_timer);
        led_blink_timer = NULL;
    }
    led_blinking = false;

    // ========== 重置游戏状态 ==========
    GAME_OVER = 0;  // 🔑 清除结束标志
    
    // ========== 重置游戏数据 ==========
    memset(&game_data, 0, sizeof(game_data_t));
    game_data.player_x = 100;
    game_data.player_y = 100;
    game_data.boom_visible = false;
    
    // ========== 重置游戏配置 ==========
    bullet_com = 1;
    bullet_tim = 16;
    enemies_cnt = 0;
    bullet_cnt = 0;
    game_score = 0;
    game_data.max_hp = 10;
    sensitivity = 1.2f;
    game_data.player_hp = 10;
    ENEMY_BULLET = 1;
    ENEMIES = 10;
    
    // ========== 重新创建逻辑线程 ==========
    sem_init(&logic_sem, 0, 0);
    pthread_create(&game_logic_thread, NULL, game_logic_func, NULL);

    // ========== 恢复UI ==========
    lv_timer_resume(game_timer);
    
    // 重启后LED长亮（满血）
    update_led_by_hp();
    
    // UI切换
    lv_obj_add_flag(gameover, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(background, LV_OBJ_FLAG_HIDDEN);
}

//退出游戏
static void exit_game(lv_event_t *e) {
    if (e->code == LV_EVENT_CLICKED) {

        if(usr_str != NULL && passd_str!= NULL)
        {
             // 确保蜂鸣器关闭
            buzz_ctrl(0, 0);
            
            // 清理LED闪烁定时器并关闭所有LED
            if(led_blink_timer) {
                lv_timer_del(led_blink_timer);
                led_blink_timer = NULL;
            }
            led_blinking = false;
            led_ctrl(-1, 0);  // 关闭所有LED

            // 保存分数
            update_score((char *)usr_str,game_score);

            // 标记游戏结束
            GAME_OVER = 1;
            usr_str = NULL;
            passd_str = NULL;

            // 停止并删除游戏定时器
            if (game_timer) {
                lv_timer_del(game_timer);
                game_timer = NULL;
            }

            // 等待游戏逻辑线程结束（避免资源竞争）
            if (game_logic_thread != 0) {
                pthread_join(game_logic_thread, NULL);
                game_logic_thread = 0;
            }

            // 释放线程同步资源
            sem_destroy(&logic_sem);
            pthread_mutex_destroy(&game_mutex);
            lv_obj_add_flag(gameexitbt,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clean(screen);
        }
    }
}


// //灵敏度
// static void slider_event_cb(lv_event_t * e)
// {
//     lv_obj_t * slider = lv_event_get_target(e);
//     sensitivity = (lv_slider_get_value(slider)/100.0+1.0)*0.75;
//     lv_label_set_text_fmt(sensitivity_label,"LMD:%3d",(int)(sensitivity*100));
// }

//游戏界面组件
static void creat_background_player() {
    // ========== 第1层：根容器 ==========
    screen = lv_obj_create(lv_scr_act());  // 创建在根屏幕上
    lv_obj_remove_style_all(screen);        // 移除默认样式
    lv_obj_set_size(screen, 1024, 600);     // 设置屏幕尺寸
    lv_obj_set_scroll_dir(screen, false);   // 禁止滚动
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);  // 隐藏滚动条

    // ========== 第2层：游戏背景 ==========
    background = lv_obj_create(screen);
    lv_obj_remove_style_all(background);
    lv_obj_set_scroll_dir(background, false);
    lv_obj_set_size(background, 1024, 600);
    lv_obj_add_style(background, &style_background, LV_STATE_DEFAULT);
    lv_obj_align(background, LV_ALIGN_CENTER, 0, 0);  // 居中对齐
    lv_obj_add_flag(background, LV_OBJ_FLAG_CLICKABLE);  // 可点击（拖拽需要）
    
    // ========== 第3层：游戏元素 ==========
    // 爆炸特效（初始隐藏）
    killenemy = lv_obj_create(background);
    lv_obj_remove_style_all(killenemy);
    lv_obj_set_scroll_dir(killenemy, false);
    lv_obj_set_size(killenemy, 80, 80);
    lv_obj_add_style(killenemy, &style_killenemy, LV_STATE_DEFAULT);
    lv_obj_add_flag(killenemy, LV_OBJ_FLAG_HIDDEN);  // 🔑 初始隐藏
    
    // 计分板
    score_label = lv_label_create(background);
    lv_obj_set_scroll_dir(score_label, false);
    lv_obj_align(score_label, LV_ALIGN_TOP_RIGHT, -20, 20);  // 右上角
    lv_label_set_text_fmt(score_label, "Score: %d", game_score);  // 格式化文本
    lv_obj_set_style_text_font(score_label, &lv_font_montserrat_24, 0);  // 24号字体
    
    // 道具（初始隐藏）
    superfire = lv_obj_create(background);
    lv_obj_remove_style_all(superfire);
    lv_obj_set_scroll_dir(superfire, false);
    lv_obj_set_size(superfire, 80, 80);
    lv_obj_add_style(superfire, &style_superfire, LV_STATE_DEFAULT);
    lv_obj_add_flag(superfire, LV_OBJ_FLAG_HIDDEN);  // 🔑 初始隐藏

    // 创建暂停按钮，并且绑定样式和事件
    gametemp = lv_btn_create(background);
    lv_obj_remove_style_all(gametemp); 
    lv_obj_set_scroll_dir(gametemp, false);
    lv_obj_set_size(gametemp,60,60);
    lv_obj_align(gametemp,LV_ALIGN_TOP_LEFT,0,0);  // 调整位置给设置按钮留出空间
    lv_obj_add_event_cb(gametemp,pause_game,LV_EVENT_CLICKED,NULL);
    lv_obj_add_style(gametemp,&style_gametemp,LV_STATE_DEFAULT);


    // 创建游戏结束界面，并且绑定样式
    gameover = lv_obj_create(screen);
    lv_obj_remove_style_all(gameover); 
    lv_obj_set_scroll_dir(gameover, false);
    lv_obj_set_size(gameover,1024,600);
    lv_obj_add_style(gameover,&style_gameover,LV_STATE_DEFAULT);
    lv_obj_add_flag(gameover, LV_OBJ_FLAG_HIDDEN);

    // 创建重新开始游戏按钮，并且绑定样式
    gamerestartbt = lv_btn_create(gameover);
    lv_obj_remove_style_all(gamerestartbt); 
    lv_obj_set_size(gamerestartbt,200,100);
    lv_obj_align(gamerestartbt,LV_ALIGN_BOTTOM_MID,0,0);
    lv_obj_add_style(gamerestartbt,&style_gamerestartbt,LV_STATE_DEFAULT);
    lv_obj_add_event_cb(gamerestartbt,restartgame,LV_EVENT_CLICKED,NULL);

    // ========== 返回主页按钮 ==========
    backhomebt = lv_btn_create(gameover);
    lv_obj_remove_style_all(backhomebt);
    lv_obj_set_size(backhomebt, 200, 100);
    lv_obj_align(backhomebt, LV_ALIGN_BOTTOM_MID, 120, 0);
    lv_obj_add_style(gamerestartbt,&style_backhomebtn,LV_STATE_DEFAULT);
    lv_obj_add_event_cb(backhomebt, lv_game_event_handler, LV_EVENT_CLICKED, NULL);

    // 创建结束界面分数标签（居中显示）
    finalscore = lv_label_create(gameover);
    lv_obj_align(finalscore, LV_ALIGN_CENTER, 0, 0); // 屏幕正中间
    lv_obj_add_style(finalscore, &style_finalscore, LV_STATE_DEFAULT); // 应用样式
    lv_label_set_text(finalscore, "Final Score: 0"); // 默认文本

    // 退出按钮
    gameexitbt = lv_btn_create(lv_layer_top());
    lv_obj_remove_style_all(gameexitbt); 
    lv_obj_set_size(gameexitbt, 60, 60);  // 与重新开始按钮尺寸一致
    lv_obj_align(gameexitbt, LV_ALIGN_BOTTOM_LEFT, 0, 0);  // 位置靠右，与重新开始按钮并排
    lv_obj_add_style(gameexitbt, &style_gameexitbt, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(gameexitbt, exit_game, LV_EVENT_CLICKED, NULL);
    lv_obj_add_flag(gameexitbt,LV_OBJ_FLAG_HIDDEN);

    lv_obj_t * label = lv_label_create(gameexitbt);
    lv_label_set_text(label, LV_SYMBOL_HOME); // 使用"确认"图标
    lv_style_set_text_font(&style_gameexitbt, &lv_font_montserrat_38);  // 使用较大字体
    lv_obj_add_style(label, &style_gameexitbt, LV_STATE_DEFAULT);
    lv_obj_center(label);  // 图标居中

    // 创建玩家飞机，并且绑定样式
    player = lv_obj_create(background);
    lv_obj_remove_style_all(player);
    lv_obj_set_scroll_dir(player, false);
    lv_obj_set_size(player, 60, 60);
    lv_obj_add_style(player, &style_player, LV_STATE_DEFAULT);
    // 设置初始位置为屏幕底部中央
    lv_obj_set_pos(player, (1024 - 60) / 2, 600 - 60 - 20);

    air.is_dragging = false;
    air.offset_x = 0;
    air.offset_y = 0;
    air.start_plane_x = (1024 - 60) / 2;  // 482
    air.start_plane_y = 600 - 60 - 20;     // 520
    air.target = player;
    // 绑定拖拽事件（在背景上，而不是飞机上！）
    lv_obj_add_event_cb(background,         // 事件目标：背景
                        move_plane,         // 回调函数
                        LV_EVENT_ALL,       // 监听所有事件
                        &air);              // 用户数据（传递给回调）

    // 初始化对象池
    init_bullet_pool();
    init_enemy_pool();

    /***************************************新增状态显示组件*******************************************/
    // 血条背景
    lv_obj_t *hp_bg = lv_obj_create(background);
    lv_obj_remove_style_all(hp_bg);
    lv_obj_set_size(hp_bg, 110, 20);
    lv_obj_align(hp_bg, LV_ALIGN_TOP_LEFT, 140, 20); // 设置按钮和暂停按钮右侧
    lv_obj_add_style(hp_bg, &style_hp_bg, LV_STATE_DEFAULT);

    // 血条
    hp_bar = lv_bar_create(hp_bg);
    lv_obj_remove_style_all(hp_bar);  // 清除默认样式，避免冲突
    lv_obj_set_size(hp_bar, 80, 10);
    lv_obj_align(hp_bar, LV_ALIGN_LEFT_MID, 4, 0);
    lv_bar_set_range(hp_bar, 0, game_data.max_hp);
    lv_bar_set_value(hp_bar, game_data.player_hp, LV_ANIM_OFF);
    lv_obj_add_style(hp_bar, &style_hp_bar, LV_STATE_DEFAULT);

    // 火力值标签
    firepower_label = lv_label_create(background);
    lv_obj_align(firepower_label, LV_ALIGN_TOP_LEFT, 140, 50); // 血条下方
    lv_obj_add_style(firepower_label, &style_firepower, LV_STATE_DEFAULT);
    lv_label_set_text(firepower_label, "Fire: 1 (Com: 1)");


    // 暂停面板（初始隐藏，只显示"继续游戏"按钮）
    setting_panel = lv_obj_create(background);
    lv_obj_remove_style_all(setting_panel);
    lv_obj_set_size(setting_panel, 300, 200);
    lv_obj_align(setting_panel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(setting_panel, &style_setting_panel, LV_STATE_DEFAULT);
    lv_obj_add_flag(setting_panel, LV_OBJ_FLAG_HIDDEN);

    // 暂停提示标签
    lv_obj_t *pause_title = lv_label_create(setting_panel);
    lv_obj_align(pause_title, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_add_style(pause_title, &style_firepower, LV_STATE_DEFAULT);
    lv_label_set_text(pause_title, "Game Paused");
    lv_obj_set_style_text_font(pause_title, &lv_font_montserrat_24, 0);

    // 继续游戏按钮（使用暂停图标样式）
    lv_obj_t *resume_btn = lv_btn_create(setting_panel);
    lv_obj_remove_style_all(resume_btn);
    lv_obj_set_size(resume_btn, 80, 80);
    lv_obj_align(resume_btn, LV_ALIGN_CENTER, 0, 10);
    lv_obj_add_event_cb(resume_btn, close_pause_panel, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(resume_btn, &style_temp, LV_STATE_DEFAULT);

    // 继续游戏按钮下方文字
    lv_obj_t *resume_label = lv_label_create(setting_panel);
    lv_obj_align(resume_label, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_add_style(resume_label, &style_firepower, LV_STATE_DEFAULT);
    lv_label_set_text(resume_label, "Click to Resume");
}

/***************************************道    具*******************************************/
//生成道具（逻辑层）
static void prop_logic()
{
    // ========== 第1步：检查道具是否已存在 ==========
    if(prop_state == OBJ_STATE_FREE) {  // 只有空闲时才能生成新道具
        
        // ========== 第2步：随机生成X坐标 ==========
        int rand = lv_random_int(0, 1024);  // 屏幕宽度范围
        
        // ========== 第3步：初始化道具数据（写入共享数据结构）==========
        game_data.prop_x = rand;        // X位置：随机
        game_data.prop_y = 0;            // Y位置：屏幕顶部
        
        // ========== 第4步：激活道具状态 ==========
        prop_state = OBJ_STATE_ACTIVE;  // 🔑 状态转换：FREE → ACTIVE
    }
}

//道具物理运动(自由落体)
static void move_prop()
{
    int g = 2;  // 重力加速度（像素/帧²）
    
    // ========== 只处理激活的道具 ==========
    if(prop_state == OBJ_STATE_ACTIVE) {
        
        // ========== 第1步：时间递增 ==========
        game_data.prop_time++;  // 存活时间+1帧
        
        // ========== 第2步：自由落体运动（经典物理公式）==========
        // y = v₀*t + 0.5*g*t²
        game_data.prop_y = game_data.prop_v * game_data.prop_time 
                          + 0.5 * game_data.prop_time * game_data.prop_time * g;
        //                  ^^^^^^^^^^^^^^^^^^^         ^^^^^^^^^^^^^^^^^^^^^^^^^^^
        //                  初速度项（为0）                加速度项
        
        // ========== 第3步：碰撞检测（玩家拾取）==========
        double dx = game_data.player_x - game_data.prop_x;
        double dy = game_data.player_y - game_data.prop_y;
        double distance_squared = dx*dx + dy*dy;
        
        // 拾取判定（圆形碰撞，半径50像素）
        if(distance_squared < 50*50) {
            // ========== 第4步：升级火力系统 ==========
            // 方案1：如果发射间隔 > 弹道数，先降低间隔（提高射速）
            if(bullet_tim > bullet_com)
                bullet_tim--;  // 每拾取一次，射速+1
            
            // 方案2：如果间隔已到极限，增加弹道数（散射能力）
            if(bullet_tim == bullet_com && bullet_com < BULLET_COM_MAX) {
                bullet_com += 2;         // 弹道数 +2
                bullet_tim = 16 + bullet_com;  // 重置间隔
            }
            
            // ========== 第5步：销毁道具 ==========
            prop_state = OBJ_STATE_FREE;
            game_data.prop_time = 0;
            game_data.prop_v = 0;
        }
        
        // ========== 第6步：边界检测（超出屏幕底部）==========
        if(game_data.prop_y > 600) {
            prop_state = OBJ_STATE_FREE;  // 销毁
            game_data.prop_time = 0;
            game_data.prop_v = 0;
        }
    }
}
/***************************************道    具*******************************************/

//快捷登入回调
void quik_sign(lv_event_t*e)
{
    User *user = lv_event_get_user_data(e);  // 获取传递的用户数据
    puts(user->username);
    puts(user->password);

    // 直接使用用户数据
    usr_str = user->username;
    passd_str = user->password;

    GAME_OVER = 0;

    // 初始化线程同步
    sem_init(&logic_sem, 0, 0);
    pthread_create(&game_logic_thread, NULL, game_logic_func, NULL);
    
    // 设置UI更新定时器（20ms间隔，约50Hz）
    game_timer = lv_timer_create(ui_update_handle, 20, NULL);

    // UI切换
    lv_obj_add_flag(signback,LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(sign_in,LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(player, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(gameexitbt,LV_OBJ_FLAG_HIDDEN);
}

//创建快捷登入按钮
void creat_sign_quik(int list)
{

    for(int i = 0;i < list;i++)
    {
        // 检查用户名有效性
        if (strlen(user_list[i].username) == 0) {
            continue;  // 跳过无效条目
        }

        // 应用容器（承载图标和文字）
        lv_obj_t *quiksignbt = lv_btn_create(signback);
        lv_obj_set_size(quiksignbt, 150, 60); // 图标容器大小
        lv_obj_set_pos(quiksignbt, 775, (i+1)*70);      // 位置
        lv_obj_add_event_cb(quiksignbt, quik_sign, LV_EVENT_CLICKED, &user_list[i]);

        // 创建按钮标签
        lv_obj_t * labe = lv_label_create(quiksignbt);
        lv_obj_set_align(labe,LV_ALIGN_CENTER);
        lv_label_set_text_fmt(labe,"usr:%s score:%d",user_list[i].username,user_list[i].score);
    }

}
/***************************************其    他*******************************************/
//文本输入回调
void foucus(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *kb = lv_event_get_user_data(e);  // 虚拟键盘
    lv_obj_t *txt = lv_event_get_target(e);   // 触发事件的输入框

    // ========== 聚焦事件（点击输入框）==========
    if (code == LV_EVENT_FOCUSED) {
        lv_keyboard_set_textarea(kb, txt);  // 键盘绑定到当前输入框
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);  // 显示键盘
    }

    // ========== 失焦事件（点击键盘外区域）==========
    if (code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(kb, NULL);  // 解除绑定
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);  // 隐藏键盘

        // 保存输入内容到全局变量
        if (txt == usr) {
            usr_str = (char *)lv_textarea_get_text(txt);
            printf("Username: %s\n", usr_str);
        } else if (txt == passd) {
            passd_str = (char *)lv_textarea_get_text(txt);
            printf("Password: %s\n", passd_str);
        }
    }
}

//开始游戏
void startgame(lv_event_t *e) {
    if (e->code == LV_EVENT_CLICKED) {
        // ========== 验证账号密码 ==========
        const char *input_usr = lv_textarea_get_text(usr);
        const char *input_passd = lv_textarea_get_text(passd);
        
        // 默认账号逻辑（如果未输入）
        if (input_usr == NULL || strlen(input_usr) == 0) {
            input_usr = "guest";
            input_passd = "123456";
            usr_str = "guest";
            passd_str = "123456";
        } else {
            usr_str = (char *)input_usr;
            passd_str = (char *)input_passd;
        }
        
        // 验证密码
        int match = check_usr_passd((char *)input_usr, (char *)input_passd);
        
        // ========== 处理验证结果 ==========
        if (match == 0) {  // 密码错误，使用默认账号
            printf("Password Error, using default login\n");
            usr_str = "guest";
            passd_str = "123456";
            match = 1;
        } else if (match == -1) {  // 新用户，创建账号
            save_data((char *)input_usr, (char *)input_passd, game_score);
            printf("NEW USER: %s\n", input_usr);
            match = 1;
        }
        
        // ========== 登录成功，开始游戏 ==========
        if (match == 1) {
            printf("Login Success: %s\n", usr_str);
            
            GAME_OVER = 0;  // 🔑 清除游戏结束标志
            
            // 初始化线程同步工具
            sem_init(&logic_sem, 0, 0);
            pthread_create(&game_logic_thread, NULL, game_logic_func, NULL);
            
            // 创建UI更新定时器（20ms = 50Hz）
            game_timer = lv_timer_create(ui_update_handle, 20, NULL);

            // 游戏开始时LED长亮（满血）
            update_led_by_hp();

            // UI切换：隐藏登录界面，显示游戏界面
            lv_obj_add_flag(signback, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(sign_in, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(player, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(gameexitbt, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

//登入界面
void sign_in_ui_init() {
    // ========== 登录背景 ==========
    signback = lv_obj_create(screen);
    lv_obj_remove_style_all(signback);
    lv_obj_set_size(signback, 1024, 600);
    lv_obj_add_style(signback, &style_signback, LV_STATE_DEFAULT);

    // ========== 登录面板 ==========
    sign_in = lv_obj_create(screen);
    lv_obj_set_size(sign_in, 300, 400);
    lv_obj_align(sign_in, LV_ALIGN_TOP_MID, 0, 60);  // 顶部居中
    lv_obj_set_style_bg_opa(sign_in, 35, LV_STATE_DEFAULT);  // 半透明
    lv_obj_add_style(sign_in, &style_signin, LV_STATE_DEFAULT);

    // ========== 虚拟键盘（初始隐藏）==========
    lv_obj_t *keybond = lv_keyboard_create(screen);
    lv_obj_add_flag(keybond, LV_OBJ_FLAG_HIDDEN);

    // ========== 用户名输入框 ==========
    usr = lv_textarea_create(sign_in);
    lv_obj_set_size(usr, 200, 50);
    lv_textarea_set_placeholder_text(usr, "usr");  // 占位符
    lv_obj_align(usr, LV_ALIGN_TOP_MID, 0, 60);
    lv_obj_add_event_cb(usr, foucus, LV_EVENT_ALL, keybond);  // 聚焦事件

    // ========== 密码输入框 ==========
    passd = lv_textarea_create(sign_in);
    lv_obj_set_size(passd, 200, 50);
    lv_textarea_set_placeholder_text(passd, "passd");
    lv_obj_align(passd, LV_ALIGN_TOP_MID, 0, 180);
    lv_obj_add_event_cb(passd, foucus, LV_EVENT_ALL, keybond);

    // ========== 登录按钮 ==========
    signbt = lv_btn_create(sign_in);
    lv_obj_set_size(signbt, 100, 50);
    lv_obj_align(signbt, LV_ALIGN_TOP_MID, 0, 280);
    lv_obj_add_event_cb(signbt, startgame, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(signbt, &style_signinbt, LV_STATE_DEFAULT);

    // ========== 提示标签（初始隐藏）==========
    label = lv_label_create(sign_in);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_style(label, &style_firepower, LV_STATE_DEFAULT);
    lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);

    // ========== 绑定键盘到输入框 ==========
    lv_keyboard_set_textarea(keybond, usr);
    lv_keyboard_set_textarea(keybond, passd);
}

/***************************************其    他*******************************************/

// 游戏逻辑  线程函数，返回类型是"指向任意类型的指针"，可以传递任意数据给pthread_join
static void *game_logic_func(void *arg) {
    static uint32_t fram = 0;  // 帧计数器（静态变量，保持值）,  uint32_t运行时间：4,294,967,295帧 * 10ms ≈ 497天
    
    while (!GAME_OVER) {
        // ========== 暂停检测 ==========
        if (GAME_TEMP) {
            usleep(50000);  // 暂停时休眠50ms，减少CPU占用
            continue;       // 跳过本次循环
        }
       
        pthread_mutex_lock(&game_mutex);  // 🔒 加锁

        // ========== 1. 玩家位置同步（每帧）==========
        game_data.player_x = lv_obj_get_x(player);
        game_data.player_y = lv_obj_get_y(player);

        // ========== 2. 子弹逻辑（每帧）==========
        move_bullet_logic();  // 移动+碰撞检测
        
        if (fram % bullet_tim == 0) {  // 默认16帧 = 160ms
            fire_bullet_logic();  // 发射子弹
        }

        // ========== 3. 敌机生成（每15帧 = 150ms）==========
        if (fram % 15 == 0) {  
            enemy_act_logic();  // 尝试生成1个敌机
        }
        
        // ========== 4. 敌机移动（每4帧 = 40ms）==========
        if (fram % 4 == 0) { 
            move_enemiesplane_logic();
        }

        // ========== 5. 敌机子弹系统 ==========
        if (fram % 14 == 0) {  // 140ms
            fire_enemybullet_logic();  // 所有敌机发射
            move_prop();               // 道具下落
        }
        
        if (fram % 8 == 0) {   // 80ms
            move_enemybullet_logic();  // 追踪玩家
        }

        // ========== 6. 爆炸特效控制（每20帧 = 200ms）==========
        if (fram % 20 == 0) {
            game_data.boom_visible = false;  // 隐藏爆炸
        }
        
        // ========== 7. 道具生成（每1000帧 = 10秒）==========
        if (fram % 1000 == 0) {
            prop_logic();
        }
        
        fram++;  // 帧计数器递增
        
        pthread_mutex_unlock(&game_mutex);  // 🔓 解锁
        sem_post(&logic_sem);  // 通知UI线程更新
        
        usleep(10000);  // 休眠10ms (100Hz)
    }
    
    // 线程逻辑
    return NULL;  // 线程正常退出，也可改为返回某个数据指针
}

// 主线程UI更新回调（添加状态显示更新）
static void ui_update_handle(lv_timer_t *timer) {

    // ========== 【阶段1】游戏结束处理 ==========
    static int game_over_triggered = 0;  // 静态标志：避免重复触发
    
    if (GAME_OVER) {
        if (!game_over_triggered) {
            // 🔊 触发蜂鸣器（0.5秒后自动关闭）
            buzz_ctrl(0, 1);
            lv_timer_create(buzz_off_timer_cb, 500, NULL);
            
            // 💡 关闭LED警告
            if (led_blink_timer) {
                lv_timer_del(led_blink_timer);
                led_blink_timer = NULL;
            }
            led_blinking = false;
            led_ctrl(-1, 0);
            
            game_over_triggered = 1;  // 标记已处理
        }
        
        // 🎬 UI切换：游戏界面 → 结束界面
        lv_obj_add_flag(background, LV_OBJ_FLAG_HIDDEN);  // 隐藏游戏背景
        lv_label_set_text_fmt(finalscore, "Final Score: %d", game_score);
        lv_obj_clear_flag(gameover, LV_OBJ_FLAG_HIDDEN);  // 显示结束画面
        
        lv_timer_pause(game_timer);  // 暂停UI定时器
        return;  // 🔑 退出，不再更新游戏对象
    }
    
    game_over_triggered = 0;  // 重置标志
    
    // ========== 【阶段2】等待逻辑线程完成计算 ==========
    sem_wait(&logic_sem);  // 🔒 阻塞直到逻辑线程通知（sem_post）
    pthread_mutex_lock(&game_mutex);  // 🔐 加锁保护共享数据
    
    // ========== 【阶段3】同步子弹UI ==========
    for(int j = 0; j < bullet_com; j++) {  // 遍历所有激活的弹道
        for(int i = 0; i < BULLET_MAX; i++) {
            if(bullet_states[j][i] == OBJ_STATE_ACTIVE) {
                // 🔑 核心步骤：
                // 1. 显示子弹对象
                lv_obj_clear_flag(bullets[j][i], LV_OBJ_FLAG_HIDDEN);
                
                // 2. 更新位置（从共享数据读取）
                lv_obj_set_pos(bullets[j][i], 
                              game_data.bullet_x[j][i],  // ← 逻辑线程计算的X
                              game_data.bullet_y[j][i]); // ← 逻辑线程计算的Y
            } else {
                // 隐藏未激活的子弹
                lv_obj_add_flag(bullets[j][i], LV_OBJ_FLAG_HIDDEN);
            }
        }
    }

    // ========== 【阶段4】同步敌机UI ==========
    for(int e = 0; e < ENEMIES; e++) {
        if(enemy_states[e] == OBJ_STATE_ACTIVE) {
             // 🎯 动态难度：根据分数调整敌机血量
            if(game_score >= 500 && game_score < 1000)
            {
               int rand = lv_random_int(0,3);
               if(rand ==3 ) // 30%概率强化
               {
                enemies_hp[e] ++;
                lv_obj_add_style(enemies[e], &style_enemy_midle, LV_STATE_DEFAULT); 
               } 
            }
            else if(game_score >= 1000 && game_score < 1700)
            {
                int rand = lv_random_int(0,3);
                if(rand >=2 )
                {
                    enemies_hp[e] ++;
                    lv_obj_add_style(enemies[e], &style_enemy_midle, LV_STATE_DEFAULT); 
                } 
            }
            else if(game_score >= 1700 && game_score < 4000)
            {
                int rand = lv_random_int(0,3);
                if(rand >=1 )
                {
                    enemies_hp[e] ++;
                    lv_obj_add_style(enemies[e], &style_enemy_midle, LV_STATE_DEFAULT); 
                } 
            }
            else if(game_score >= 4000 && game_score < 7000)
            {
                int rand = lv_random_int(0,5);
                if(rand >=1 )
                {
                    enemies_hp[e] ++;
                    lv_obj_add_style(enemies[e], &style_enemy_midle, LV_STATE_DEFAULT); 
                } 
            }

            lv_obj_clear_flag(enemies[e], LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_pos(enemies[e], game_data.enemy_x[e], game_data.enemy_y[e]); // ← 逻辑线程更新的位置
        }
         else {
            lv_obj_add_style(enemies[e], &style_enemy_small, LV_STATE_DEFAULT); 
            lv_obj_add_flag(enemies[e], LV_OBJ_FLAG_HIDDEN);
        }
    }

    // ========== 【阶段5】同步敌机子弹UI ==========
    for(int e = 0; e < ENEMIES; e++) {
        for(int b = 0; b < ENEMY_BULLET; b++) {
            if(enemy_bullet_states[e][b] == OBJ_STATE_ACTIVE) {
                lv_obj_clear_flag(enemy_bullets[e][b], LV_OBJ_FLAG_HIDDEN);
                lv_obj_set_pos(enemy_bullets[e][b], 
                              game_data.enemy_bullet_x[e][b], 
                              game_data.enemy_bullet_y[e][b]);
            } else {
                lv_obj_add_flag(enemy_bullets[e][b], LV_OBJ_FLAG_HIDDEN);
            }
        }
    }

    // ========== 【阶段6】同步爆炸效果UI ==========
    if(game_data.boom_visible) {
        lv_obj_clear_flag(killenemy, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_pos(killenemy, game_data.boom_x, game_data.boom_y);
    } else {
        lv_obj_add_flag(killenemy, LV_OBJ_FLAG_HIDDEN);
    }

    // ========== 【阶段7】同步道具UI ==========
    if(prop_state == OBJ_STATE_ACTIVE) {
        lv_obj_clear_flag(superfire, LV_OBJ_FLAG_HIDDEN);  // 显示道具
        lv_obj_set_pos(superfire, 
                      game_data.prop_x,  // ← move_prop 计算的位置
                      game_data.prop_y);
    } else {
        lv_obj_add_flag(superfire, LV_OBJ_FLAG_HIDDEN);  // 隐藏道具
    }
    
    // ========== 【阶段8】更新HUD（血条/分数/火力）==========
    lv_label_set_text_fmt(score_label, "Score: %d", game_score);
    
    // 血条长度 = 当前HP占比 * 100%
    lv_obj_set_size(hp_bar, game_data.player_hp * 100 / game_data.max_hp, 10);
    
    // 火力值显示
    int firepower_level = (16 + bullet_com - bullet_tim);
    lv_label_set_text_fmt(firepower_label, 
                         "Fire: %d (Com: %d) HP: %d", 
                         firepower_level, bullet_com, game_data.player_hp);
    
    // 💡 LED警告灯控制
    update_led_by_hp();  // 血量<40%时闪烁
    
    pthread_mutex_unlock(&game_mutex);  // 🔓 解锁
}

//初始化
void game_init() {
    
    // 初始化蜂鸣器
    buzz_init();

    // LED初始化
    led_init();  

    // 用当前系统时间作为种子，确保每次运行随机序列不同
    srand((unsigned int)time(NULL));
    int list = list_all_users(user_list);
    style_set();
    creat_background_player();

    // 初始化共享数据
   memset(&game_data, 0, sizeof(game_data_t));
    // 设置飞机初始位置：屏幕底部中央
    // 屏幕尺寸 1024x600，飞机尺寸 60x60
    game_data.player_x = (1024 - 60) / 2;  // 水平居中：482
    game_data.player_y = 600 - 60 - 20;     // 底部位置：520
    game_data.boom_visible = false;
    bullet_com = 1;
    bullet_tim = 16;
    enemies_cnt = 0;
    bullet_cnt = 0;
    game_score = 0;
    game_data.max_hp = 10;  // 修改最大血量为10
    game_data.player_hp = 10;  // 修改初始血量为10
    ENEMY_BULLET  = 1 ;  //敌机子弹连发数量
    ENEMIES = 10 ; //敌机数量
   
    // 显示登录界面
    sign_in_ui_init();

    // 创建快捷登录按钮
    creat_sign_quik(list);
}

