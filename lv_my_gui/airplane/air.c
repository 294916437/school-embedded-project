#include "../lvgl/lvgl.h"
#include "air.h"
#include "player.h"
#include "enemy.h"
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdio.h>
#include "main_screen.h"
#include <unistd.h>
#include "data.h"
#include "../modules/buzzer.h"  // 添加蜂鸣器头文件
#include "../modules/LED.h"  // 添加LED头文件

static void *game_logic_func(void *arg);
void startgame(lv_event_t * e);

// 添加LED相关函数声明（在文件开头，static void *game_logic_func 之后）
static void led_blink_timer_cb(lv_timer_t *timer);
static void update_led_by_hp(void);
static void buzz_off_timer_cb(lv_timer_t *timer);

/* 游戏元素对象 */
static lv_obj_t *screen,*background ;//屏幕 //游戏背景
static lv_style_t style_background;
    
static lv_obj_t *player;
static lv_obj_t *bullets[BULLET_COM_MAX][BULLET_MAX] = {{NULL}};
static lv_obj_t *killenemy;//玩家子弹数   //爆炸特效
static lv_style_t style_player,style_bullet,style_killenemy;

static lv_obj_t *enemies[ENEMIES_MAX] = {NULL},*enemy_bullets[ENEMIES_MAX][ENEMY_BULLET_MAX] = {{NULL}}; //最大敌机数量为200 //最大子弹连发数量为5
static lv_style_t style_enemy_small,style_enemy_midle,style_enemybullet;

static lv_obj_t *gametemp,*temp; //游戏暂停 //暂停界面
static lv_style_t style_gametemp,style_temp;

static lv_obj_t *gameover,*finalscore,*gameexitbt,*gamerestartbt;//结束界面 //最终得分  //退出游戏  //重新开始
static lv_style_t style_gameover,style_finalscore,style_gameexitbt,style_gamerestartbt;

static lv_obj_t *signback,*usr,*passd,*sign_in,*signbt; //登入背景  //用户  //密码  //登入界面  //登入按钮
static lv_style_t style_signback,style_signin,style_signinbt ;//登入界面


// 在全局变量区域添加LED闪烁相关变量
static lv_timer_t *led_blink_timer = NULL;
static bool led_blink_state = false;
static bool led_blinking = false;


//..
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
double angle_step = 8.0; //灵敏度 //子弹间隔
float sensitivity = 1.2f;
int game_score = 0;//得分
int enemies_cnt = 0,ENEMY_BULLET = 1,ENEMIES = 10;//动态敌机数量 //敌机子弹连发数量 //当前最大敌机数量
int GAME_OVER = 1,GAME_TEMP = 0;//游戏结束标志 // 游戏暂停标志
char *usr_str , *passd_str;//用户，密码匹配

int tempre = 0;
User user_list[10];
static lv_timer_t *game_timer;
static pthread_t game_logic_thread;
static sem_t logic_sem;
static pthread_mutex_t game_mutex;
game_data_t game_data;
air_draw air;
static obj_state_t prop_state = OBJ_STATE_FREE;
//星空主题
char *sky_theme[] = { 
    "A:img/background.bmp",
    "A:img/player.bmp",
    "A:img/enemy_m.bmp",
    "A:img/enemy_n.bmp",
    "A:img/bullet.bmp",
    "A:img/enmy_bullet.bmp",
    "A:img/boom.bmp",
    "A:img/superfire.bmp",
    "A:img/signin_back.bmp",
    "A:img/signin.bmp",
    "A:img/signinbt.bmp",
    "A:img/gametemp.bmp",
    "A:img/temp.bmp",
    "A:img/gameover.bmp",
    "A:img/restartgame.bmp"
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

    // 新增：退出按钮样式
    lv_style_init(&style_gameexitbt);
    lv_style_set_bg_opa(&style_gameexitbt, LV_OPA_0);

    
}


// 初始化子弹对象池
static void init_bullet_pool() {
    // ... 原有代码保持不变 ...
    for(int j = 0;j < BULLET_COM_MAX ; j++)
    {
        for(int i = 0; i < BULLET_MAX; i++)
        {
            bullets[j][i] = lv_obj_create(background);
            lv_obj_remove_style_all(bullets[j][i]);
            lv_obj_set_scroll_dir(bullets[j][i], false);
            lv_obj_set_size(bullets[j][i], 60, 60);
            lv_obj_add_style(bullets[j][i], &style_bullet, LV_STATE_DEFAULT);
            lv_obj_add_flag(bullets[j][i], LV_OBJ_FLAG_HIDDEN);  // 初始隐藏
            bullet_states[j][i] = OBJ_STATE_FREE;
        }
    }
}

// 初始化敌机对象池
static void init_enemy_pool() {
    // ... 原有代码保持不变 ...
    for(int i = 0; i < ENEMIES_MAX; i++) {
        enemies[i] = lv_obj_create(background);
        lv_obj_remove_style_all(enemies[i]);
        lv_obj_set_scroll_dir(enemies[i], false);
        lv_obj_set_size(enemies[i], 60, 60);
        lv_obj_add_style(enemies[i], &style_enemy_small, LV_STATE_DEFAULT);
        lv_obj_add_flag(enemies[i], LV_OBJ_FLAG_HIDDEN);
        enemy_states[i] = OBJ_STATE_FREE;

        // 初始化敌机子弹池
        for(int j = 0; j < ENEMY_BULLET_MAX; j++) {
            enemy_bullets[i][j] = lv_obj_create(background);
            lv_obj_remove_style_all(enemy_bullets[i][j]);
            lv_obj_set_size(enemy_bullets[i][j], 20, 20);
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
    if(GAME_OVER || GAME_TEMP) {
        // 游戏结束或暂停时，关闭所有LED和闪烁
        if(led_blink_timer) {
            lv_timer_del(led_blink_timer);
            led_blink_timer = NULL;
        }
        led_blinking = false;
        led_ctrl(-1, 0);  // 关闭所有LED
        return;
    }

    // 计算血量百分比
    int hp_percent = (game_data.player_hp * 100) / game_data.max_hp;

    if(hp_percent > 40) {
        // 血量>40%: LED长亮，停止闪烁
        if(led_blinking) {
            if(led_blink_timer) {
                lv_timer_del(led_blink_timer);
                led_blink_timer = NULL;
            }
            led_blinking = false;
        }
        led_ctrl(-1, 1);  // 所有LED长亮
    } else {
        // 血量<=40%: LED闪烁
        if(!led_blinking) {
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
void pause_game(lv_event_t *e)
{
    if(e->code == LV_EVENT_CLICKED)
    {
        if(GAME_TEMP == 0) {
            // 暂停游戏
            GAME_TEMP = 1;
            update_score((char*)usr_str, game_score);
            lv_timer_pause(game_timer);
            
            // 暂停时关闭LED
            update_led_by_hp();
            
            lv_obj_clear_flag(setting_panel, LV_OBJ_FLAG_HIDDEN);
            printf("游戏暂停\n");
        } else {
            GAME_TEMP = 0;
            lv_timer_resume(game_timer);
            
            // 恢复时根据血量更新LED
            update_led_by_hp();
            
            lv_obj_add_flag(setting_panel, LV_OBJ_FLAG_HIDDEN);
            printf("游戏恢复\n");
        }
    }
}

//游戏重新开始
void restartgame(lv_event_t *e)
{
    update_score((char *)usr_str,game_score);
    
    // 确保蜂鸣器关闭
    buzz_ctrl(0, 0);
    
    // 清理LED闪烁定时器
    if(led_blink_timer) {
        lv_timer_del(led_blink_timer);
        led_blink_timer = NULL;
    }
    led_blinking = false;

    GAME_OVER = 0;
    // 初始化共享数据
    memset(&game_data, 0, sizeof(game_data_t));
    game_data.player_x = 100;
    game_data.player_y = 100;
    game_data.boom_visible = false;
    bullet_com = 1;
    bullet_tim = 16;
    enemies_cnt = 0;
    bullet_cnt = 0;
    game_score = 0;
    game_data.max_hp = 10;  // 修改最大血量为10
    sensitivity = 1.2f;
    game_data.player_hp = 10;  // 修改初始血量为10
    ENEMY_BULLET  = 1 ;
    ENEMIES = 10 ;
    
    // 初始化线程同步
    sem_init(&logic_sem, 0, 0);
    pthread_create(&game_logic_thread, NULL, game_logic_func, NULL);

    lv_timer_resume(game_timer);
    
    // 重启后LED长亮（满血状态）
    update_led_by_hp();
    
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
            main_screen_init();  
        }
    }
}



//游戏界面组件
static void creat_background_player() {
    screen = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(screen);
    lv_obj_set_size(screen, 1024, 600);
    lv_obj_set_scroll_dir(screen, false); // 禁止滚动
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF); // 隐藏滚动条

    // 创建背景，并且绑定样式
    background = lv_obj_create(screen);
    lv_obj_remove_style_all(background);
    lv_obj_set_scroll_dir(background, false);
    lv_obj_set_size(background, 1024, 600);
    lv_obj_add_style(background, &style_background, LV_STATE_DEFAULT);
    lv_obj_align(background, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(background, LV_OBJ_FLAG_CLICKABLE);
    
    // 创建爆炸效果，但是隐藏
    killenemy = lv_obj_create(background);
    lv_obj_remove_style_all(killenemy);
    lv_obj_set_scroll_dir(killenemy, false);
    lv_obj_set_size(killenemy, 80, 80);
    lv_obj_add_style(killenemy, &style_killenemy, LV_STATE_DEFAULT);
    lv_obj_add_flag(killenemy, LV_OBJ_FLAG_HIDDEN);

    //创建计分板
    score_label = lv_label_create(background);
    lv_obj_set_scroll_dir(score_label, false);
    lv_obj_align(score_label, LV_ALIGN_TOP_RIGHT, -20, 20);
    lv_label_set_text_fmt(score_label, "Score: %d", game_score);
    lv_obj_set_style_text_font(score_label, &lv_font_montserrat_24, 0);

    //创建道具，隐藏
    superfire = lv_obj_create(background);
    lv_obj_remove_style_all(superfire);
    lv_obj_set_scroll_dir(superfire, false);
    lv_obj_set_size(superfire, 80, 80);
    lv_obj_add_style(superfire, &style_superfire, LV_STATE_DEFAULT);
    lv_obj_add_flag(superfire, LV_OBJ_FLAG_HIDDEN);

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
    lv_obj_add_event_cb(background, move_plane, 
                        LV_EVENT_ALL,
                        &air);

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
    // ... 原有代码保持不变 ...
    if(prop_state == OBJ_STATE_FREE)
    {
    int rand = lv_random_int(0,1024);
    game_data.prop_x = rand;
    game_data.prop_y = 0;
    prop_state = OBJ_STATE_ACTIVE;
    }
}

//道具自由落体
static void move_prop()
{
    // ... 原有代码保持不变 ...
    int g = 2;
    if(prop_state == OBJ_STATE_ACTIVE)
    {
        game_data.prop_time++;
        game_data.prop_y = game_data.prop_v*game_data.prop_time + 0.5 * game_data.prop_time*game_data.prop_time*g;
       
        double dx = game_data.player_x - game_data.prop_x;
        double dy = game_data.player_y - game_data.prop_y;
        double distance_squared = dx*dx + dy*dy;
        //玩家拾取到道具 碰撞
        if(distance_squared < 50*50)
        {
            prop_state = OBJ_STATE_FREE;
            game_data.prop_time = 0;
            game_data.prop_v = 0;
            
            if(bullet_tim > bullet_com)
                bullet_tim--;

            if(bullet_tim == bullet_com && bullet_com < BULLET_COM_MAX)
            {
                bullet_com +=2 ;
                bullet_tim = 16+bullet_com;
            }
        }
        if(game_data.prop_y > 600)
        {
            prop_state = OBJ_STATE_FREE;
            game_data.prop_time = 0;
            game_data.prop_v = 0;
           
        }
     
    }

}
/***************************************道    具*******************************************/

//快捷登入
void quik_sign(lv_event_t*e)
{
    User *user = lv_event_get_user_data(e); 
    puts(user->username);
    puts(user->password);
    usr_str = user->username;
    passd_str = user->password;
    GAME_OVER = 0;
    // 初始化线程同步
    sem_init(&logic_sem, 0, 0);
    pthread_create(&game_logic_thread, NULL, game_logic_func, NULL);
    
    // 设置UI更新定时器（20ms间隔，约50Hz）
    game_timer = lv_timer_create(ui_update_handle, 20, NULL);

    lv_obj_add_flag(signback,LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(sign_in,LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(player, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(gameexitbt,LV_OBJ_FLAG_HIDDEN);
}

//快捷登入按钮
void creat_sign_quik(int list)
{

    for(int i = 0;i < list;i++)
    {
        // 应用容器（承载图标和文字）
        lv_obj_t *quiksignbt = lv_btn_create(signback);
        lv_obj_set_size(quiksignbt, 150, 60); // 图标容器大小
        lv_obj_set_pos(quiksignbt, 775, (i+1)*70);      // 位置
        lv_obj_add_event_cb(quiksignbt, quik_sign, LV_EVENT_CLICKED, &user_list[i]);

        lv_obj_t * labe = lv_label_create(quiksignbt);
        lv_obj_set_align(labe,LV_ALIGN_CENTER);
        lv_label_set_text_fmt(labe,"usr:%s score:%d",user_list[i].username,user_list[i].score);
    }

}
/***************************************其    他*******************************************/
//文本输入回调
void foucus(lv_event_t * e)
{
    // ... 原有代码保持不变 ...
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * kb = lv_event_get_user_data(e);
    lv_obj_t * txt = lv_event_get_target(e);

    if(code == LV_EVENT_FOCUSED) {      //聚焦
        lv_keyboard_set_textarea(kb, txt);
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN); //显示
    }

    if(code == LV_EVENT_DEFOCUSED) {  //不聚焦
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);  //隐藏
      
        if( txt == usr ) {
            usr_str = (char *)lv_textarea_get_text(txt);
            printf("%s\n",usr_str);
        }
        else if( txt == passd ) {
            passd_str = (char *)lv_textarea_get_text(txt);
            printf("%s\n",passd_str);
        }
    }
}

//开始游戏
void startgame(lv_event_t * e)
{
    if(e->code == LV_EVENT_CLICKED)
    {
        // 从文本框直接获取文本（而不是依赖全局变量）
        const char *input_usr = lv_textarea_get_text(usr);
        const char *input_passd = lv_textarea_get_text(passd);
        
        // 默认账号：如果没有输入，使用默认账号
        if(input_usr == NULL || input_passd == NULL || 
           strlen(input_usr) == 0 || strlen(input_passd) == 0)
        {
            printf("Using default account: guest/123456\n");
            input_usr = "guest";
            input_passd = "123456";
            usr_str = "guest";
            passd_str = "123456";
        }
        else
        {
            usr_str = (char *)input_usr;
            passd_str = (char *)input_passd;
        }
        
        int match = check_usr_passd((char *)input_usr,(char *)input_passd);
        
        // 如果验证失败（文件不存在或账号错误），直接使用默认账号登录
        if(match == 0 )
        {
            printf("Password Error, using default login\n");
            lv_label_set_text_fmt(label,"Using Default Account");
            lv_obj_clear_flag(label,LV_OBJ_FLAG_HIDDEN);
            match = 1; // 强制登录成功
            usr_str = "guest";
            passd_str = "123456";
        }
        else if(match == -1)
        {
            // 新用户或文件不存在，创建账号
            save_data((char *)input_usr,(char *)input_passd,game_score);
            printf("NEW USER: %s\n", input_usr);
            lv_label_set_text_fmt(label,"New User Created");
            lv_obj_clear_flag(label,LV_OBJ_FLAG_HIDDEN);
            match = 1;
        }
        
        if(match == 1) 
        {
            printf("Login Success: %s\n", usr_str);
            lv_label_set_text_fmt(label,"Login Success");
            lv_obj_clear_flag(label,LV_OBJ_FLAG_HIDDEN);
            
            // 登录成功后清空输入框并移除焦点
        
            GAME_OVER = 0;
            // 初始化线程同步
            sem_init(&logic_sem, 0, 0);
            pthread_create(&game_logic_thread, NULL, game_logic_func, NULL);
            
            // 设置UI更新定时器（20ms间隔，约50Hz）
            game_timer = lv_timer_create(ui_update_handle, 20, NULL);

            // 游戏开始时LED长亮（满血状态）
            update_led_by_hp();

            lv_obj_add_flag(signback,LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(sign_in,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(player, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(gameexitbt,LV_OBJ_FLAG_HIDDEN);
        }
    }
}

//登入界面
void sign_in_ui_init()
{
    
    // ... 原有代码保持不变 ...
    //创建登入界面背景
    signback = lv_obj_create(screen);
    lv_obj_remove_style_all(signback);
    lv_obj_set_size(signback,1024,600);
    lv_obj_add_style(signback,&style_signback,LV_STATE_DEFAULT);

    //创建登入界面
    sign_in = lv_obj_create(screen);
    lv_obj_set_size(sign_in,300,400);
    lv_obj_align(sign_in,LV_ALIGN_TOP_MID,0,60);
    lv_obj_set_style_bg_opa(sign_in, 35,LV_STATE_DEFAULT);
    lv_obj_add_style(sign_in,&style_signin,LV_STATE_DEFAULT);

    lv_obj_t *keybond = lv_keyboard_create(screen);
    lv_obj_add_flag(keybond,LV_OBJ_FLAG_HIDDEN);

    usr = lv_textarea_create(sign_in);
    lv_obj_set_size(usr,200,50);
    lv_textarea_set_placeholder_text(usr,"usr");
    lv_obj_align(usr,LV_ALIGN_TOP_MID,0,60);
    lv_obj_add_event_cb(usr,foucus,LV_EVENT_ALL,keybond);

    passd = lv_textarea_create(sign_in);
    lv_obj_set_size(passd,200,50);
    lv_textarea_set_placeholder_text(passd,"passd");
    lv_obj_align(passd,LV_ALIGN_TOP_MID,0,180);
    lv_obj_add_event_cb(passd,foucus,LV_EVENT_ALL,keybond);
    
    signbt = lv_btn_create(sign_in); 
    lv_obj_set_size(signbt,100,50);
    lv_obj_align(signbt,LV_ALIGN_TOP_MID,0,280);
    lv_obj_add_event_cb(signbt,startgame,LV_EVENT_CLICKED,NULL);
    lv_obj_add_style(signbt,&style_signinbt,LV_STATE_DEFAULT);

    label = lv_label_create(sign_in);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0); // 屏幕正中间
    lv_obj_add_style(label, &style_firepower, LV_STATE_DEFAULT); // 应用样式
    lv_obj_add_flag(label,LV_OBJ_FLAG_HIDDEN);

    lv_keyboard_set_textarea(keybond,usr);
    lv_keyboard_set_textarea(keybond,passd);
    
}


// 游戏逻辑线程函数
static void *game_logic_func(void *arg) {
    static uint32_t fram = 0;
    while((!GAME_OVER))
    {
        // 暂停时休眠并跳过所有逻辑
        if(GAME_TEMP)
        {
            usleep(50000);  // 暂停时休眠50ms，避免CPU占用
            continue;
        }
       
        pthread_mutex_lock(&game_mutex);

        // 1. 更新玩家位置
        game_data.player_x = lv_obj_get_x(player);
        game_data.player_y = lv_obj_get_y(player);

        // 2. 子弹逻辑
        move_bullet_logic();
        if(fram % bullet_tim == 0) {
            fire_bullet_logic();
        }

        // 3. 敌机逻辑
        if(fram % 15 == 0) {  
            enemy_act_logic();
        }
        if(fram % 4 == 0) { 
            move_enemiesplane_logic();
        }

        // 4. 敌机子弹逻辑
        if(fram % 14 == 0) {
            fire_enemybullet_logic();
            move_prop();
        }
        if(fram % 8 == 0) {
            move_enemybullet_logic();
        }

        // 5. 爆炸效果控制
        if(fram % 20 == 0) {
            game_data.boom_visible = false;
        }
        
        // 6. 道具生成
        if(fram % 1000 == 0) {
            prop_logic();
        }
        
        fram++;
        pthread_mutex_unlock(&game_mutex);
        sem_post(&logic_sem);

        usleep(10000);  // 休眠10ms
    }
    return NULL;
}

// 主线程UI更新回调（添加状态显示更新）
// 在游戏结束时触发蜂鸣器
static void ui_update_handle(lv_timer_t *timer) {

    static int game_over_triggered = 0;  // 添加标志避免重复触发

    if(GAME_OVER) 
    {
        // 只在第一次游戏结束时触发蜂鸣器
       if(!game_over_triggered) {
            buzz_ctrl(0, 1);  // 开启蜂鸣器
            lv_timer_create(buzz_off_timer_cb, 500, NULL);  // 0.5秒后关闭

            // 游戏结束时关闭LED
            if(led_blink_timer) {
                lv_timer_del(led_blink_timer);
                led_blink_timer = NULL;
            }
            led_blinking = false;
            led_ctrl(-1, 0);


            game_over_triggered = 1;
        }
        
        lv_obj_add_flag(background, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text_fmt(finalscore, "Final Score: %d", game_score);
        lv_obj_clear_flag(gameover, LV_OBJ_FLAG_HIDDEN);
        lv_timer_pause(game_timer);
        return;  // 游戏结束后直接返回，不再更新
    }
    
    // 游戏进行中时重置标志
    game_over_triggered = 0;
    
    sem_wait(&logic_sem);  // 等待逻辑线程通知
    pthread_mutex_lock(&game_mutex);

    // 1. 更新子弹UI
    for(int j = 0; j < bullet_com ; j++)
    {
        for(int i = 0; i < BULLET_MAX; i++) {
            if(bullet_states[j][i] == OBJ_STATE_ACTIVE) {
                lv_obj_clear_flag(bullets[j][i], LV_OBJ_FLAG_HIDDEN);
                lv_obj_set_pos(bullets[j][i], game_data.bullet_x[j][i], game_data.bullet_y[j][i]);
            } else {
                lv_obj_add_flag(bullets[j][i], LV_OBJ_FLAG_HIDDEN);
            }
            
        }
    }
    // 2. 更新敌机UI
    for(int e = 0; e < ENEMIES; e++) {
        if(enemy_states[e] == OBJ_STATE_ACTIVE) {
             
            if(game_score >= 500 && game_score < 1000)
            {
               int rand = lv_random_int(0,3);
               if(rand ==3 )
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
            lv_obj_set_pos(enemies[e], game_data.enemy_x[e], game_data.enemy_y[e]);
        }
         else {
            lv_obj_add_style(enemies[e], &style_enemy_small, LV_STATE_DEFAULT); 
            lv_obj_add_flag(enemies[e], LV_OBJ_FLAG_HIDDEN);
        }
    }

    // 3. 更新敌机子弹UI
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

    // 4. 更新爆炸效果
    if(game_data.boom_visible) {
        lv_obj_clear_flag(killenemy, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_pos(killenemy, game_data.boom_x, game_data.boom_y);
    } else {
        lv_obj_add_flag(killenemy, LV_OBJ_FLAG_HIDDEN);
    }

    //5.更新道具
    if(prop_state == OBJ_STATE_ACTIVE)
    {
        lv_obj_clear_flag(superfire, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_pos(superfire,game_data.prop_x,game_data.prop_y);
    }
    else{
        lv_obj_add_flag(superfire,LV_OBJ_FLAG_HIDDEN);
    }
    lv_label_set_text_fmt(score_label, "Score: %d", game_score);
  
    /***************************************更新状态显示*******************************************/
    // 更新血条
 
    lv_obj_set_size(hp_bar,game_data.player_hp*100/game_data.max_hp,10);
    // 更新火力值显示（结合火力值和弹道数）
    int firepower_level = (16 + bullet_com - bullet_tim); // 综合计算
    lv_label_set_text_fmt(firepower_label,"Fire: %d (Com: %d) HP: %d",firepower_level,bullet_com,game_data.player_hp);


    // 根据血量更新LED状态（新增）
    update_led_by_hp();

    pthread_mutex_unlock(&game_mutex);
}

//初始化
void game_init() {
    
    // 初始化蜂鸣器
    buzz_init();
    led_init();  // 添加LED初始化

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
    creat_sign_quik(list);
}

