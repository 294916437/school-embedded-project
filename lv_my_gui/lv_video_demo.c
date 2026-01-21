#include "lv_video_demo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>    
#include <sys/stat.h>
#include "../lvgl/lvgl.h"
#include "../lvgl/demos/lv_demos.h"
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <sys/wait.h>
#include "lv_run_main.h" 
#include "lv_mygec_font.h"



// 设置UI更新定时器（20ms间隔，约50Hz）
static lv_timer_t *game_timer;

//计时器
static lv_timer_t *one_sec_timer;

// 新增退出按钮对象和样式
static lv_obj_t *gameexitbt;                // 退出按钮
static lv_style_t style_gameexitbt;         // 退出按钮样式

lv_obj_t *video_screen;
static pid_t video_pid = -1;
static pid_t play_pid = -1;
static lv_obj_t *video_player;

static lv_obj_t *play_btn;//播放、暂停
static lv_obj_t *play_label;

static lv_obj_t *status_label;//状态

static lv_obj_t *next_btn;//下一个
static lv_obj_t *next_label;

static lv_obj_t *prev_btn;//上一个
static lv_obj_t *prev_label;

static lv_obj_t *volumup_btn;//音量滑动条
static lv_obj_t *volumup_label;

static lv_obj_t *backmainbt;

// 样式对象定义
static lv_style_t btn_style;
static lv_style_t btn_pressed_style;
static lv_style_t label_style;
static lv_style_t status_style;
static lv_style_t slider_style_bg;
static lv_style_t slider_style_indicator;
static lv_style_t slider_style_knob;
static lv_style_t video_area_style;

static int is_playing=0;//播放状态
static int fifo_fd = -1;
static int fifo_out_fd = -1;
static int volume_ = 30;//音量控制
static int total_time_flag = 0;//是否正确获取到总时长

// 在现有变量定义中添加进度条相关变量
static lv_obj_t *progress_slider;       // 进度滑动条
static lv_obj_t *progress_label;        // 进度显示标签
static int total_seconds = 0;           // 总时长(秒)
static int current_seconds = 0;         // 当前进度(秒)
static int is_slider_dragging = 0;      // 标记滑块是否正在被拖动


char *video[10] = {
    "video/1.mp4",
    "video/2.mp4",
    "video/3.mp4",
    "video/4.mp4",
    "video/5.mp4",
    "video/6.mp4"
};
char *play = NULL; //当前播放的视频
int list = 0;//视频索引
int total_time = 0;//获取总时长效应次数

// 用于接收mplayer输出的FIFO（复用现有FIFO机制）
static char current_position[32] = "00:00";  // 当前进度
static char video_length[32] = "00:00";  // 当前进度


void video_cleanup();
// 初始化样式
static void init_styles() {
    
    // 按钮基础样式
    lv_style_init(&btn_style);
    lv_style_set_radius(&btn_style, BTN_RADIUS);
    lv_style_set_bg_color(&btn_style, BTN_BG_COLOR);
    lv_style_set_border_color(&btn_style, BTN_BORDER_COLOR);
    lv_style_set_border_width(&btn_style, 2);
    lv_style_set_pad_all(&btn_style, 10);

    // 按钮按下样式
    lv_style_init(&btn_pressed_style);
    lv_style_set_bg_color(&btn_pressed_style, BTN_PRESSED_COLOR);

    // 标签样式
    lv_style_init(&label_style);
    lv_style_set_text_color(&label_style, LABEL_COLOR);
    lv_style_set_text_font(&label_style, &lv_font_montserrat_24);  // 使用较大字体

    // 状态标签样式
    lv_style_init(&status_style);
    lv_style_set_text_color(&status_style, STATUS_COLOR_STOP);
    lv_style_set_text_font(&status_style, &lv_font_montserrat_28);  // 状态字体更大
    lv_style_set_pad_all(&status_style, 5);

    // 滑块样式 - 背景
    lv_style_init(&slider_style_bg);
    lv_style_set_bg_color(&slider_style_bg, SLIDER_BG);
    lv_style_set_radius(&slider_style_bg, 8);
    lv_style_set_pad_all(&slider_style_bg, 4);

    // 滑块样式 - 已选部分
    lv_style_init(&slider_style_indicator);
    lv_style_set_bg_color(&slider_style_indicator, SLIDER_INDICATOR);
    lv_style_set_radius(&slider_style_indicator, 8);

    // 滑块样式 - 按钮
    lv_style_init(&slider_style_knob);
    lv_style_set_bg_color(&slider_style_knob, SLIDER_KNOB);
    lv_style_set_radius(&slider_style_knob, LV_RADIUS_CIRCLE);
    lv_style_set_size(&slider_style_knob, 20);  // 滑块按钮大小

    // 视频区域样式
    lv_style_init(&video_area_style);
    lv_style_set_border_color(&video_area_style, VIDEO_BORDER_COLOR);
    lv_style_set_border_width(&video_area_style, 3);
    lv_style_set_radius(&video_area_style, 8);
    lv_style_set_bg_color(&video_area_style, lv_color_hex(0x000000));  // 黑色背景
    lv_style_set_shadow_color(&video_area_style, lv_color_hex(0x00000080));  // 阴影
    lv_style_set_shadow_width(&video_area_style, 5);

    // 在init_styles()函数中，将标签字体改为自定义中文字库
    lv_style_set_text_font(&status_style, &lv_mygec_font);
  
}

//  检查 open 结果
static void ensure_fifo() {
    struct stat st;
    
    // 创建 FIFO（如果不存在）
    if (stat(FIFO_PATH, &st) == -1) {
        if (mkfifo(FIFO_PATH, 0666) == -1) {
            perror("mkfifo input failed");
            return;
        }
    }
    if (stat(FIFO_OUT_PATH, &st) == -1) {
        if (mkfifo(FIFO_OUT_PATH, 0666) == -1) {
            perror("mkfifo output failed");
            return;
        }
    }
    
    // 打开 FIFO（非阻塞模式）
    if (fifo_out_fd == -1) {
        fifo_out_fd = open(FIFO_OUT_PATH, O_RDONLY | O_NONBLOCK);
        if (fifo_out_fd == -1) {
            perror("open fifo_out failed");
        } else {
            printf("FIFO output opened (fd=%d)\n", fifo_out_fd);
        }
    }
    
    if (fifo_fd == -1) {
        fifo_fd = open(FIFO_PATH, O_WRONLY | O_NONBLOCK);
        if (fifo_fd == -1) {
            perror("open fifo_input failed");
        } else {
            printf("FIFO input opened (fd=%d)\n", fifo_fd);
        }
    }
}


// 停止视频播放
// 视频停止时终止线程
void video_stop() {
    if(video_pid != -1) {
       kill(video_pid,2);
        video_pid = -1;
    }
    lv_timer_pause(game_timer);
    is_playing = 0;
    lv_label_set_text(status_label, "停止播放");
    lv_label_set_text(play_label, "Stop");  // 播放图标
    lv_style_set_text_color(&status_style, STATUS_COLOR_STOP);
    lv_obj_add_style(status_label, &status_style, 0);
}

// 播放/暂停视频
// 优化视频启动命令（添加音频驱动配置）
static void toggle_play(lv_event_t *e) {
    ensure_fifo();
    play_pid = waitpid(video_pid, NULL, WNOHANG);
    
    if (play_pid == video_pid) {
        video_pid = fork();
        
        if (video_pid == 0) {
            // 重定向输出到 FIFO
            int out_fd = open(FIFO_OUT_PATH, O_RDWR);
            if (out_fd != -1) {
                dup2(out_fd, STDOUT_FILENO);
                dup2(out_fd, STDERR_FILENO);
                close(out_fd);
            }
            
            // ✅ 添加音频驱动配置
            execl("/usr/bin/mplayer", "mplayer",
                "-slave",                          // 从机模式
                "-quiet",                          // 安静模式
                "-input", "file=/rk3568/rk.fifo", // 命令输入
                "-vo", "fbdev2",                   // 视频输出
                "-geometry", "0:0",                // 位置
                "-zoom", "-x", "1024", "-y", "500", // 缩放
                "-ao", "alsa",                     // ✅ 音频驱动（ALSA）
                "-af", "volume=0:sc",              // ✅ 软件音量控制
                "-volume", "30",                   // ✅ 初始音量
                play, NULL);
            
            perror("execl failed");
            exit(1);
        }
        
        // 初始化播放状态
        total_time_flag = 0;
        total_seconds = 0;
        current_seconds = 0;
        is_playing = 1;
        
        // 设置初始音量
        usleep(500000);  // 等待 MPlayer 启动
        if(fifo_fd != -1) {
            char buf[50];
            sprintf(buf, VIDEO_VOLUME, volume_);
            write(fifo_fd, buf, strlen(buf));
        }
        
        lv_timer_resume(game_timer);
        lv_timer_resume(one_sec_timer);
        lv_label_set_text(play_label, LV_SYMBOL_PAUSE);
        lv_label_set_text(status_label, "正在播放");
        lv_style_set_text_color(&status_style, STATUS_COLOR_PLAY);
        lv_obj_add_style(status_label, &status_style, 0);
    } else {
        // 暂停/继续
        if (fifo_fd != -1) {
            write(fifo_fd, VIDEO_PLAY, strlen(VIDEO_PLAY));
        }
        is_playing = !is_playing;
        lv_label_set_text(play_label, is_playing ? LV_SYMBOL_PAUSE : LV_SYMBOL_PLAY);
        lv_label_set_text(status_label, is_playing ? "正在播放" : "已暂停");
        lv_style_set_text_color(&status_style, is_playing ? STATUS_COLOR_PLAY : STATUS_COLOR_PAUSE);
        lv_obj_add_style(status_label, &status_style, 0);
    }
}

// 停止视频回调
static void stop_video_cb(lv_event_t *e) {
    video_stop();
}

// 下一个视频
static void next_video(lv_event_t *e) {
    list++;
    if(video[list])
    {    
   
        kill(video_pid,2);
        play = video[list];
        usleep(100000);
        toggle_play(e);

        lv_label_set_text(status_label, "播放下一首");
    }
    else
    {
        list--;
        lv_label_set_text(status_label, "已是最后一首");
    }
}

// 上一个视频
static void prev_video(lv_event_t *e) {
    list--;
    if(list>=0)
    {
       
        kill(video_pid,2);
        play = video[list];
        usleep(100000);
        toggle_play(e);
        

        lv_label_set_text(status_label, "播放上一首");
    }
    else
    {
        list = 0;
        lv_label_set_text(status_label, "已是第一首");
    }
}

//返回主界面
static void backhome(lv_event_t *e)
{
    // 清理视频资源
    video_cleanup();
    
    // 删除视频界面
    if(video_screen != NULL) {
        lv_obj_del(video_screen);
        video_screen = NULL;
    }
    
    // 返回主界面
    main_grid();
}

//+音量
static void volumup_video(lv_event_t *e) {
    if(fifo_fd != -1) {
        volume_ = lv_slider_get_value(volumup_btn);
        char buf[50];
        
        // 发送音量命令到 MPlayer
        sprintf(buf, VIDEO_VOLUME, volume_);
        ssize_t ret = write(fifo_fd, buf, strlen(buf));
        
        if(ret > 0) {
            // 更新状态标签（仅在成功写入时更新）
            lv_label_set_text_fmt(status_label, "音量: %d%%", volume_);
            lv_style_set_text_color(&status_style, lv_color_hex(0x3498DB));  // 蓝色表示调节中
            lv_obj_add_style(status_label, &status_style, 0);
        } else {
            perror("Failed to set volume");
        }
    }
}

// 进度条事件回调函数
static void progress_video(lv_event_t *e) {
    if(total_seconds > 0) {
        // 获取滑块当前百分比值
        int value = lv_slider_get_value(progress_slider);
        
        // 计算对应的秒数（范围：0 ~ total_seconds）
        current_seconds = (total_seconds * value) / 100;
        
        // 更新显示（格式：MM:SS/MM:SS）
        int cur_mins = current_seconds / 60;
        int cur_secs = current_seconds % 60;
        int total_mins = total_seconds / 60;
        int total_secs = total_seconds % 60;
        
        lv_label_set_text_fmt(progress_label, "%02d:%02d/%02d:%02d", 
                              cur_mins, cur_secs, total_mins, total_secs);
    }
}

// 新增：滑块开始拖动事件
static void slider_start_drag(lv_event_t *e) {
   
    is_slider_dragging = 1;
    // printf("is drw:%d\n",is_slider_dragging);
   
}

// 新增：滑块结束拖动事件
static void slider_end_drag(lv_event_t *e) {
    char buf[50];
    is_slider_dragging = 0;
    
    if(fifo_fd != -1 && total_seconds > 0) {
        // 使用绝对定位模式（seek <秒数> 2 表示相对百分比）
        // 改为 seek <秒数> 1 表示绝对秒数
        sprintf(buf, "seek %d 2\n", current_seconds);
        ssize_t ret = write(fifo_fd, buf, strlen(buf));
        
        if(ret > 0) {
            printf("Seek to %d/%d seconds\n", current_seconds, total_seconds);
            // 立即查询新位置
            write(fifo_fd, VIDEO_TIME, strlen(VIDEO_TIME));
        } else {
            perror("Failed to seek");
        }
    }
    
    // 恢复定时器更新
    lv_timer_resume(one_sec_timer);
}

// 新增：时长更新的异步回调函数（在主线程执行）
static void update_length_async(void *param) {
    char *text = (char *)param;
    lv_label_set_text_fmt(progress_label, "%s/%s", current_position, video_length);
    
}

// 新增：进度更新的异步回调函数（在主线程执行）
static void update_position_async(void *param) {
    char *text = (char *)param;
    lv_label_set_text_fmt(progress_label, "%s/%s", current_position, video_length);
 
    if(total_seconds > 0)
    lv_slider_set_value(progress_slider,(current_seconds * 100) / total_seconds,LV_ANIM_OFF);

}


// 解析mplayer返回的时长信息（格式：ANS_LENGTH=xxx.xx）
static void parse_length(const char *buf) {
    const char *prefix = "ANS_LENGTH=";
    if (strstr(buf, prefix)) {
        float secs = atof(buf + strlen(prefix));
    //    printf("%.2f\n",secs);
        if(total_seconds == (int)secs)  // 保存总秒数
        total_time++;

        if(total_time >= 5)
        {
        total_time_flag = 1;//以正确获取到总时长
        total_time = 0;//计数清零
        lv_timer_pause(game_timer);//获取总时长的定时器关闭
        }
        total_seconds = (int)secs;
        int mins = (int)secs / 60;
        int sec = (int)secs % 60;

        sprintf(video_length, "%02d:%02d", mins, sec);  // 格式化为MM:SS
        lv_async_call(update_length_async, video_length);
    }
}
// 解析实际播放位置（从 ANS_TIME_POSITION 获取）
static void parse_position(const char *buf) {
    const char *prefix = "ANS_TIME_POSITION=";
    if (strstr(buf, prefix)) {
        float pos = atof(buf + strlen(prefix));
        current_seconds = (int)pos;
        
        // 更新显示
        int mins = current_seconds / 60;
        int sec = current_seconds % 60;
        sprintf(current_position, "%02d:%02d", mins, sec);
        
        // 异步更新UI
        lv_async_call(update_position_async, current_position);
    }
}

// 优化一秒定时器
static void one_sec_handle(lv_timer_t *timer) {
    // 检查进程是否存活
    play_pid = waitpid(video_pid, NULL, WNOHANG);
    
    if (play_pid != video_pid && is_playing && !is_slider_dragging) {
        // 发送查询命令
        if(fifo_fd != -1) {
            write(fifo_fd, VIDEO_TIME, strlen(VIDEO_TIME));
            
            // 读取实际位置
            char buf[128];
            ssize_t n = read(fifo_out_fd, buf, sizeof(buf)-1);
            
            if(n > 0) {
                buf[n] = '\0';
                parse_position(buf);  // ✅ 使用实际位置而非本地计数
            } else {
                // 回退到本地计数（当读取失败时）
                current_seconds++;
                int mins = current_seconds / 60;
                int sec = current_seconds % 60;
                sprintf(current_position, "%02d:%02d", mins, sec);
                lv_async_call(update_position_async, current_position);
            }
        }
    } else if(play_pid == video_pid) {
        // 视频播放结束
        video_pid = -1;
        is_playing = 0;
        lv_timer_pause(one_sec_timer);
        lv_label_set_text(status_label, "播放完成");
    }
}

// 读取函数
static void read_mplayer_output(lv_timer_t *timer) {
    if(fifo_fd == -1 || total_time_flag || fifo_out_fd == -1) {
        return;
    }
    
    // 发送查询命令
    if(write(fifo_fd, VIDEO_LENG, strlen(VIDEO_LENG)) < 0) {
        if(errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("write VIDEO_LENG failed");
        }
        return;
    }
    
    // 读取输出（非阻塞）
    char buf[256];
    ssize_t n = read(fifo_out_fd, buf, sizeof(buf)-1);
    
    if(n > 0) {
        buf[n] = '\0';
        parse_length(buf);
    } else if(n < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        perror("read fifo_out failed");
    }
}


// 创建带图标和样式的按钮
static lv_obj_t *create_icon_button(lv_obj_t *parent, const char *icon, lv_coord_t w, lv_coord_t h, lv_event_cb_t cb) {
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, w, h);
    lv_obj_add_style(btn, &btn_style, LV_STATE_DEFAULT);
    lv_obj_add_style(btn, &btn_pressed_style, LV_STATE_PRESSED);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, icon);
    lv_obj_add_style(label, &label_style, 0);
    lv_obj_center(label);  // 图标居中
   
    return btn;
}

// 初始化视频界面
void video_init() {
    // 初始化样式

    if(video_screen)
    lv_obj_clean(video_screen);
    init_styles();

    play = video[0];
    video_screen = lv_obj_create(lv_scr_act());
    lv_obj_set_scroll_dir(video_screen, false); // 禁止滚动
    lv_obj_set_scrollbar_mode(video_screen, LV_SCROLLBAR_MODE_OFF); // 隐藏滚动条
    lv_obj_set_size(video_screen, 1024, 600);
    lv_obj_set_style_bg_color(video_screen, lv_color_hex(0x1E2127), 0);  // 深色背景

    // 创建视频显示区域
    video_player = lv_obj_create(video_screen);
    lv_obj_set_size(video_player, 1024, 500);
    lv_obj_align(video_player,LV_ALIGN_CENTER,0,-100);
    lv_obj_add_style(video_player, &video_area_style, 0);

    // 状态标签
    status_label = lv_label_create(video_screen);
    lv_obj_align(status_label, LV_ALIGN_BOTTOM_MID, 400, -40);
    lv_label_set_text(status_label, "未播放");
    lv_obj_add_style(status_label, &status_style, 0);

    // 控制按钮 - 播放/暂停 (▶/⏸)
    play_btn = lv_btn_create(video_screen);
    lv_obj_set_size(play_btn,100,40);
    lv_obj_add_event_cb(play_btn,toggle_play,LV_EVENT_CLICKED,NULL);
    lv_obj_add_style(play_btn, &btn_style, LV_STATE_DEFAULT);
    lv_obj_add_style(play_btn, &btn_pressed_style, LV_STATE_PRESSED);
    lv_obj_align(play_btn, LV_ALIGN_BOTTOM_MID, 0, 10);

    play_label = lv_label_create(play_btn);
    lv_obj_set_align(play_label,LV_ALIGN_CENTER);
    lv_label_set_text(play_label,LV_SYMBOL_PLAY);
    lv_obj_add_style(play_label,&label_style,0);

    // 控制按钮 - 下一个 (▶▶)
    next_btn = create_icon_button(video_screen, LV_SYMBOL_NEXT, 100, 40, next_video);
    lv_obj_align(next_btn, LV_ALIGN_BOTTOM_MID, 220, 10);


    // 控制按钮 - 上一个 (◀◀)
    prev_btn = create_icon_button(video_screen, LV_SYMBOL_PREV, 100, 40, prev_video);
    lv_obj_align(prev_btn, LV_ALIGN_BOTTOM_MID, -220, 10);

    backmainbt = create_icon_button(video_screen,LV_SYMBOL_HOME, 100, 60, backhome);
    lv_obj_align(backmainbt, LV_ALIGN_BOTTOM_LEFT, 20, 0);


    volumup_btn = lv_slider_create(video_screen);
    lv_obj_set_size(volumup_btn, 180, 30);
    lv_obj_align(volumup_btn, LV_ALIGN_BOTTOM_MID, 400, 10);
    lv_slider_set_range(volumup_btn, 0, 100);
    lv_slider_set_value(volumup_btn, volume_, LV_ANIM_ON);  // 初始值动画
    lv_obj_add_event_cb(volumup_btn, volumup_video, LV_EVENT_VALUE_CHANGED, NULL);
    // 应用滑块样式
    lv_obj_add_style(volumup_btn, &slider_style_bg, LV_PART_MAIN);
    lv_obj_add_style(volumup_btn, &slider_style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(volumup_btn, &slider_style_knob, LV_PART_KNOB);

    // 音量控制
    volumup_label = lv_label_create(video_screen);
    lv_obj_align(volumup_label, LV_ALIGN_BOTTOM_MID, 400,-3);
    lv_label_set_text(volumup_label, "音量调节");
    lv_obj_add_style(volumup_label, &status_style, 0);

    // 创建进度显示标签
    progress_label = lv_label_create(video_screen);
    lv_obj_align(progress_label, LV_ALIGN_BOTTOM_MID, 300, -35);  // 位于控制按钮上方
    lv_label_set_text(progress_label, "00:00/00:00");
    lv_obj_add_style(progress_label, &status_style, 0);

    // 创建进度滑动条
    progress_slider = lv_slider_create(video_screen);
    lv_obj_set_size(progress_slider, 500, 20);  // 较长的滑块，方便精确控制
    lv_obj_align(progress_slider, LV_ALIGN_BOTTOM_MID, 0, -35);  // 位于进度标签下方
    lv_slider_set_range(progress_slider, 0, 100);  // 0-100百分比
    lv_slider_set_value(progress_slider, 0, LV_ANIM_OFF);
    // 修改：添加滑块拖动开始和结束的事件回调
    lv_obj_add_event_cb(progress_slider, slider_start_drag, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(progress_slider, slider_end_drag, LV_EVENT_RELEASED , NULL);
    lv_obj_add_event_cb(progress_slider, progress_video, LV_EVENT_VALUE_CHANGED, NULL);
    // 应用滑块样式（复用现有样式）
    lv_obj_add_style(progress_slider, &slider_style_bg, LV_PART_MAIN);
    lv_obj_add_style(progress_slider, &slider_style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(progress_slider, &slider_style_knob, LV_PART_KNOB);

    one_sec_timer = lv_timer_create(one_sec_handle,1000,NULL);
    game_timer = lv_timer_create(read_mplayer_output, 10, NULL);
    lv_timer_pause(game_timer);
    lv_timer_pause(one_sec_timer);
    video_pid = -1;
    fifo_fd = -1;
    fifo_out_fd = -1;
    volume_ = 30;
    total_time_flag = 0;
    is_playing = 0;
    list = 0;
    is_slider_dragging = 0;
    // 确保FIFO已创建
    ensure_fifo();
}
/**
 * 闭视频模块并返回主界面（供外部调用）
 */
void video_demo_close(void)
{
    video_cleanup();
    
    if(video_screen != NULL) {
        lv_obj_del(video_screen);
        video_screen = NULL;
    }
    
    main_grid();
}

// 清理视频资源
void video_cleanup() 
{
    // 如果正在播放，先发送暂停命令
    if(is_playing && fifo_fd != -1)
    {
        write(fifo_fd, VIDEO_PLAY, strlen(VIDEO_PLAY));
        usleep(100000);
    }
    
    // 终止视频进程
    if(video_pid > 0) {
        kill(video_pid, SIGTERM);  // 使用SIGTERM更优雅
        usleep(100000);            // 等待进程退出
        waitpid(video_pid, NULL, WNOHANG);  // 回收僵尸进程
        video_pid = -1;
    }
    
    // 关闭FIFO文件描述符
    if (fifo_fd != -1) {
        close(fifo_fd);
        fifo_fd = -1;
    }
    if (fifo_out_fd != -1) {
        close(fifo_out_fd);
        fifo_out_fd = -1;
    }
    
    // 删除定时器
    if (game_timer) {
        lv_timer_del(game_timer);
        game_timer = NULL;
    }
    if (one_sec_timer) {
        lv_timer_del(one_sec_timer);
        one_sec_timer = NULL;
    }
    
    // 重置状态变量
    is_playing = 0;
    total_time_flag = 0;
    total_seconds = 0;
    current_seconds = 0;
    list = 0;
    is_slider_dragging = 0;
}

