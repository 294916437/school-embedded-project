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

// 定时器
static lv_timer_t *game_timer;
static lv_timer_t *one_sec_timer;

// 对象定义
lv_obj_t *video_screen;
static lv_obj_t *video_player;
static lv_obj_t *play_btn;
static lv_obj_t *play_label;
static lv_obj_t *status_label;
static lv_obj_t *next_btn;
static lv_obj_t *prev_btn;
static lv_obj_t *volumup_btn;
static lv_obj_t *volumup_label;
static lv_obj_t *backmainbt;
static lv_obj_t *progress_slider;
static lv_obj_t *progress_label;

// 样式对象
static lv_style_t btn_style;
static lv_style_t btn_pressed_style;
static lv_style_t label_style;
static lv_style_t status_style;
static lv_style_t slider_style_bg;
static lv_style_t slider_style_indicator;
static lv_style_t slider_style_knob;
static lv_style_t video_area_style;

// 全局状态变量
static pid_t video_pid = -1;
static int is_playing = 0;
static int is_paused = 0;  // ✅ 新增：区分暂停状态
static int fifo_fd = -1;
static int fifo_out_fd = -1;
static int volume_ = 30;
static int total_time_flag = 0;
static int total_seconds = 0;
static int current_seconds = 0;
static int is_slider_dragging = 0;
static int list = 0;
static int total_time = 0;

// 视频列表
static char *video[10] = {
    "video/1.mp4",
    "video/2.mp4",
    "video/3.mp4",
    "video/4.mp4",
    "video/5.mp4",
    "video/6.mp4"
};
static char *play = NULL;

// 时间字符串
static char current_position[32] = "00:00";
static char video_length[32] = "00:00";

// 前置声明
void video_cleanup();

// ==================== 初始化样式 ====================
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
    lv_style_set_text_font(&label_style, &lv_font_montserrat_24);

    // 状态标签样式
    lv_style_init(&status_style);
    lv_style_set_text_color(&status_style, STATUS_COLOR_STOP);
    lv_style_set_text_font(&status_style, &lv_mygec_font);
    lv_style_set_pad_all(&status_style, 5);

    // 滑块样式
    lv_style_init(&slider_style_bg);
    lv_style_set_bg_color(&slider_style_bg, SLIDER_BG);
    lv_style_set_radius(&slider_style_bg, 8);
    lv_style_set_pad_all(&slider_style_bg, 4);

    lv_style_init(&slider_style_indicator);
    lv_style_set_bg_color(&slider_style_indicator, SLIDER_INDICATOR);
    lv_style_set_radius(&slider_style_indicator, 8);

    lv_style_init(&slider_style_knob);
    lv_style_set_bg_color(&slider_style_knob, SLIDER_KNOB);
    lv_style_set_radius(&slider_style_knob, LV_RADIUS_CIRCLE);
    lv_style_set_size(&slider_style_knob, 20);

    // 视频区域样式
    lv_style_init(&video_area_style);
    lv_style_set_border_color(&video_area_style, VIDEO_BORDER_COLOR);
    lv_style_set_border_width(&video_area_style, 3);
    lv_style_set_radius(&video_area_style, 8);
    lv_style_set_bg_color(&video_area_style, lv_color_hex(0x000000));
    lv_style_set_shadow_color(&video_area_style, lv_color_hex(0x00000080));
    lv_style_set_shadow_width(&video_area_style, 5);
}

// ==================== 确保FIFO存在 ====================
static void ensure_fifo() {
    struct stat st;
    
    // 创建输入FIFO
    if (stat(FIFO_PATH, &st) == -1) {
        if (mkfifo(FIFO_PATH, 0666) == -1) {
            perror("mkfifo input failed");
            return;
        }
    }
    
    // 创建输出FIFO
    if (stat(FIFO_OUT_PATH, &st) == -1) {
        if (mkfifo(FIFO_OUT_PATH, 0666) == -1) {
            perror("mkfifo output failed");
            return;
        }
    }
    
    // 先打开输入FIFO（写端，非阻塞）
    if (fifo_fd == -1) {
        fifo_fd = open(FIFO_PATH, O_WRONLY | O_NONBLOCK);
        if (fifo_fd == -1) {
            if (errno == ENXIO) {
                int dummy_fd = open(FIFO_PATH, O_RDWR | O_NONBLOCK);
                if (dummy_fd != -1) {
                    printf("FIFO保活文件描述符: %d\n", dummy_fd);
                }
                fifo_fd = open(FIFO_PATH, O_WRONLY | O_NONBLOCK);
            }
            
            if (fifo_fd == -1) {
                perror("open fifo_input failed");
            } else {
                printf("FIFO input opened (fd=%d)\n", fifo_fd);
            }
        }
    }
    
    // 再打开输出FIFO（读端，非阻塞）
    if (fifo_out_fd == -1) {
        fifo_out_fd = open(FIFO_OUT_PATH, O_RDONLY | O_NONBLOCK);
        if (fifo_out_fd == -1) {
            perror("open fifo_out failed");
        } else {
            printf("FIFO output opened (fd=%d)\n", fifo_out_fd);
        }
    }
}

// ==================== 停止视频播放 ====================
void video_stop() {
    if(video_pid > 0) {
        kill(video_pid, SIGTERM);
        usleep(100000);
        waitpid(video_pid, NULL, WNOHANG);
        video_pid = -1;
    }
    
    lv_timer_pause(game_timer);
    lv_timer_pause(one_sec_timer);
    is_playing = 0;
    is_paused = 0;
    
    lv_label_set_text(status_label, "停止播放");
    lv_label_set_text(play_label, LV_SYMBOL_PLAY);
    lv_style_set_text_color(&status_style, STATUS_COLOR_STOP);
    lv_obj_add_style(status_label, &status_style, 0);
}

// ==================== 播放/暂停切换 ====================
static void toggle_play(lv_event_t *e) {
    ensure_fifo();
    
    // 检查进程是否存活
    int status;
    pid_t result = waitpid(video_pid, &status, WNOHANG);
    
    if (result == video_pid || video_pid == -1) {
        // ========== 启动新视频 ==========
        video_pid = fork();
        
        if (video_pid == 0) {
            // 子进程：重定向输出到FIFO
            int out_fd = open(FIFO_OUT_PATH, O_RDWR);
            if (out_fd != -1) {
                dup2(out_fd, STDOUT_FILENO);
                dup2(out_fd, STDERR_FILENO);
                close(out_fd);
            }
            
            // ✅ 关键修复：使用 -geometry 参数将视频渲染到指定位置和大小
            // 格式：-geometry WxH+X+Y
            // 视频区域：宽1024px，高420px，起始坐标(0,10)
            execl("/usr/bin/mplayer", "mplayer",
                "-slave",
                "-quiet",
                "-input", "file=/rk3568/rk.fifo",
                "-vo", "fbdev2:/dev/fb0",
                "-vf", "scale=1024:420",        // 缩放到指定尺寸
                "-geometry", "0:10",            // 定位到屏幕坐标(0,10)
                "-zoom",                        // 启用缩放
                "-aspect", "16:9",              // 保持宽高比
                "-ao", "alsa",
                "-af", "volume=0:sc",
                "-volume", "30",
                "-framedrop",
                "-lavdopts", "fast:skiploopfilter=all",
                play, NULL);
            
            perror("execl failed");
            exit(1);
        }
        
        // 父进程：初始化播放状态
        total_time_flag = 0;
        total_seconds = 0;
        current_seconds = 0;
        is_playing = 1;
        is_paused = 0;
        
        // 等待MPlayer启动
        usleep(500000);
        
        // 设置初始音量
        if(fifo_fd != -1) {
            char buf[50];
            sprintf(buf, VIDEO_VOLUME, volume_);
            write(fifo_fd, buf, strlen(buf));
        }
        
        // 启动定时器
        lv_timer_resume(game_timer);
        lv_timer_resume(one_sec_timer);
        
        // 更新UI
        lv_label_set_text(play_label, LV_SYMBOL_PAUSE);
        lv_label_set_text(status_label, "正在播放");
        lv_style_set_text_color(&status_style, STATUS_COLOR_PLAY);
        lv_obj_add_style(status_label, &status_style, 0);
        
        printf("视频已启动: %s (PID=%d)\n", play, video_pid);
    } else {
        // ========== 暂停/继续播放 ==========
        if (fifo_fd != -1) {
            // 使用 pause 命令而不是 pause\n
            const char *cmd = "pause\n";
            ssize_t ret = write(fifo_fd, cmd, strlen(cmd));
            
            if (ret > 0) {
                // 切换暂停状态
                is_paused = !is_paused;
                
                // 更新UI
                if(is_paused) {
                    lv_label_set_text(play_label, LV_SYMBOL_PLAY);
                    lv_label_set_text(status_label, "已暂停");
                    lv_style_set_text_color(&status_style, STATUS_COLOR_PAUSE);
                    lv_timer_pause(one_sec_timer);
                    lv_timer_pause(game_timer);
                    printf("视频已暂停 (PID=%d)\n", video_pid);
                } else {
                    lv_label_set_text(play_label, LV_SYMBOL_PAUSE);
                    lv_label_set_text(status_label, "正在播放");
                    lv_style_set_text_color(&status_style, STATUS_COLOR_PLAY);
                    lv_timer_resume(one_sec_timer);
                    lv_timer_resume(game_timer);
                    printf("视频继续播放 (PID=%d)\n", video_pid);
                }
                
                lv_obj_add_style(status_label, &status_style, 0);
            } else {
                perror("发送暂停命令失败");
            }
        }
    }
}

// ==================== 下一个视频 ====================
static void next_video(lv_event_t *e) {
    list++;
    if(video[list]) {
        video_stop();
        play = video[list];
        usleep(100000);
        toggle_play(e);
        lv_label_set_text(status_label, "播放下一首");
    } else {
        list--;
        lv_label_set_text(status_label, "已是最后一首");
    }
}

// ==================== 上一个视频 ====================
static void prev_video(lv_event_t *e) {
    list--;
    if(list >= 0) {
        video_stop();
        play = video[list];
        usleep(100000);
        toggle_play(e);
        lv_label_set_text(status_label, "播放上一首");
    } else {
        list = 0;
        lv_label_set_text(status_label, "已是第一首");
    }
}

// ==================== 返回主界面 ====================
static void backhome(lv_event_t *e) {
    video_cleanup();
    
    if(video_screen != NULL) {
        lv_obj_del(video_screen);
        video_screen = NULL;
    }
    
    main_grid(&style);
}

// ==================== 音量控制 ====================
static void volumup_video(lv_event_t *e) {
    if(fifo_fd != -1) {
        volume_ = lv_slider_get_value(volumup_btn);
        char buf[50];
        sprintf(buf, VIDEO_VOLUME, volume_);
        ssize_t ret = write(fifo_fd, buf, strlen(buf));
        
        if(ret > 0) {
            lv_label_set_text_fmt(status_label, "音量: %d%%", volume_);
            lv_style_set_text_color(&status_style, lv_color_hex(0x3498DB));
            lv_obj_add_style(status_label, &status_style, 0);
        }
    }
}

// ==================== 进度条拖动开始 ====================
static void slider_start_drag(lv_event_t *e) {
    is_slider_dragging = 1;
    lv_timer_pause(one_sec_timer);
    printf("开始拖动进度条\n");
}

// ==================== 进度条拖动结束 ====================
static void slider_end_drag(lv_event_t *e) {
    is_slider_dragging = 0;
    
    if(fifo_fd != -1 && total_seconds > 0) {
        int percent = lv_slider_get_value(progress_slider);
        char buf[50];
        sprintf(buf, "seek %d 2\n", percent);
        
        ssize_t ret = write(fifo_fd, buf, strlen(buf));
        if(ret > 0) {
            printf("跳转到 %d%%\n", percent);
            usleep(200000);
            write(fifo_fd, VIDEO_TIME, strlen(VIDEO_TIME));
        } else {
            perror("跳转失败");
        }
    }
    
    if(is_playing && !is_paused) {
        lv_timer_resume(one_sec_timer);
    }
}

// ==================== 进度条值变化 ====================
static void progress_video(lv_event_t *e) {
    if(is_slider_dragging && total_seconds > 0) {
        int value = lv_slider_get_value(progress_slider);
        current_seconds = (total_seconds * value) / 100;
        
        int cur_mins = current_seconds / 60;
        int cur_secs = current_seconds % 60;
        int total_mins = total_seconds / 60;
        int total_secs = total_seconds % 60;
        
        lv_label_set_text_fmt(progress_label, "%02d:%02d/%02d:%02d", 
                              cur_mins, cur_secs, total_mins, total_secs);
    }
}

// ==================== 异步更新时长 ====================
static void update_length_async(void *param) {
    lv_label_set_text_fmt(progress_label, "%s/%s", current_position, video_length);
}

// ==================== 异步更新进度 ====================
static void update_position_async(void *param) {
    if(!is_slider_dragging) {
        lv_label_set_text_fmt(progress_label, "%s/%s", current_position, video_length);
        
        if(total_seconds > 0) {
            int percent = (current_seconds * 100) / total_seconds;
            lv_slider_set_value(progress_slider, percent, LV_ANIM_OFF);
        }
    }
}

// ==================== 解析总时长 ====================
static void parse_length(const char *buf) {
    const char *prefix = "ANS_LENGTH=";
    if (strstr(buf, prefix)) {
        float secs = atof(buf + strlen(prefix));
        
        if(total_seconds == (int)secs) {
            total_time++;
        }
        
        if(total_time >= 5) {
            total_time_flag = 1;
            total_time = 0;
            lv_timer_pause(game_timer);
            printf("总时长获取成功: %d 秒\n", total_seconds);
        }
        
        total_seconds = (int)secs;
        int mins = (int)secs / 60;
        int sec = (int)secs % 60;
        sprintf(video_length, "%02d:%02d", mins, sec);
        lv_async_call(update_length_async, video_length);
    }
}

// ==================== 解析当前位置 ====================
static void parse_position(const char *buf) {
    const char *prefix = "ANS_TIME_POSITION=";
    if (strstr(buf, prefix)) {
        float pos = atof(buf + strlen(prefix));
        current_seconds = (int)pos;
        
        int mins = current_seconds / 60;
        int sec = current_seconds % 60;
        sprintf(current_position, "%02d:%02d", mins, sec);
        
        lv_async_call(update_position_async, current_position);
    }
}

// ==================== 一秒定时器（更新进度） ====================
static void one_sec_handle(lv_timer_t *timer) {
    int status;
    pid_t result = waitpid(video_pid, &status, WNOHANG);
    
    if (result == 0 && is_playing && !is_paused && !is_slider_dragging) {
        // 进程存活且正在播放（未暂停）
        if(fifo_fd != -1) {
            write(fifo_fd, VIDEO_TIME, strlen(VIDEO_TIME));
            
            // 读取位置
            char buf[128];
            ssize_t n = read(fifo_out_fd, buf, sizeof(buf)-1);
            
            if(n > 0) {
                buf[n] = '\0';
                parse_position(buf);
            }
        }
    } else if(result == video_pid) {
        // 视频播放结束
        video_pid = -1;
        is_playing = 0;
        is_paused = 0;
        lv_timer_pause(one_sec_timer);
        lv_timer_pause(game_timer);
        lv_label_set_text(status_label, "播放完成");
        lv_label_set_text(play_label, LV_SYMBOL_PLAY);
        printf("视频播放结束\n");
    }
}

// ==================== 读取MPlayer输出（获取总时长） ====================
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
    
    // 读取输出
    char buf[256];
    ssize_t n = read(fifo_out_fd, buf, sizeof(buf)-1);
    
    if(n > 0) {
        buf[n] = '\0';
        parse_length(buf);
    }
}

// ==================== 创建图标按钮 ====================
static lv_obj_t *create_icon_button(lv_obj_t *parent, const char *icon, lv_coord_t w, lv_coord_t h, lv_event_cb_t cb) {
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, w, h);
    lv_obj_add_style(btn, &btn_style, LV_STATE_DEFAULT);
    lv_obj_add_style(btn, &btn_pressed_style, LV_STATE_PRESSED);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, icon);
    lv_obj_add_style(label, &label_style, 0);
    lv_obj_center(label);
   
    return btn;
}

// ==================== 初始化视频界面 ====================
void video_init() {
    if(video_screen) {
        lv_obj_clean(video_screen);
    }
    
    init_styles();
    play = video[0];
    
    // 创建主屏幕
    video_screen = lv_obj_create(lv_scr_act());
    lv_obj_set_scroll_dir(video_screen, LV_DIR_NONE);
    lv_obj_set_scrollbar_mode(video_screen, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(video_screen, 1024, 600);
    lv_obj_set_style_bg_color(video_screen, lv_color_hex(0x1E2127), 0);

    // ✅ 视频显示区域 - 与MPlayer渲染区域对齐
    video_player = lv_obj_create(video_screen);
    lv_obj_set_size(video_player, 1024, 420);
    lv_obj_align(video_player, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_add_style(video_player, &video_area_style, 0);

    // 进度标签
    progress_label = lv_label_create(video_screen);
    lv_obj_align(progress_label, LV_ALIGN_TOP_LEFT, 10, 440);
    lv_label_set_text(progress_label, "00:00/00:00");
    lv_obj_add_style(progress_label, &status_style, 0);

    // 状态标签
    status_label = lv_label_create(video_screen);
    lv_obj_align(status_label, LV_ALIGN_TOP_RIGHT, -10, 440);
    lv_label_set_text(status_label, "未播放");
    lv_obj_add_style(status_label, &status_style, 0);

    // 进度滑块
    progress_slider = lv_slider_create(video_screen);
    lv_obj_set_size(progress_slider, 800, 20);
    lv_obj_align(progress_slider, LV_ALIGN_TOP_MID, 0, 470);
    lv_slider_set_range(progress_slider, 0, 100);
    lv_slider_set_value(progress_slider, 0, LV_ANIM_OFF);
    lv_obj_add_event_cb(progress_slider, slider_start_drag, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(progress_slider, slider_end_drag, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(progress_slider, progress_video, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_style(progress_slider, &slider_style_bg, LV_PART_MAIN);
    lv_obj_add_style(progress_slider, &slider_style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(progress_slider, &slider_style_knob, LV_PART_KNOB);

    // 播放/暂停按钮
    play_btn = lv_btn_create(video_screen);
    lv_obj_set_size(play_btn, 100, 50);
    lv_obj_add_event_cb(play_btn, toggle_play, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(play_btn, &btn_style, LV_STATE_DEFAULT);
    lv_obj_add_style(play_btn, &btn_pressed_style, LV_STATE_PRESSED);
    lv_obj_align(play_btn, LV_ALIGN_BOTTOM_MID, 0, -10);

    play_label = lv_label_create(play_btn);
    lv_label_set_text(play_label, LV_SYMBOL_PLAY);
    lv_obj_add_style(play_label, &label_style, 0);
    lv_obj_center(play_label);

    // 上一个按钮
    prev_btn = create_icon_button(video_screen, LV_SYMBOL_PREV, 100, 50, prev_video);
    lv_obj_align(prev_btn, LV_ALIGN_BOTTOM_MID, -110, -10);

    // 下一个按钮
    next_btn = create_icon_button(video_screen, LV_SYMBOL_NEXT, 100, 50, next_video);
    lv_obj_align(next_btn, LV_ALIGN_BOTTOM_MID, 110, -10);

    // 返回主页按钮
    backmainbt = create_icon_button(video_screen, LV_SYMBOL_HOME, 80, 50, backhome);
    lv_obj_align(backmainbt, LV_ALIGN_BOTTOM_LEFT, 10, -10);

    // 音量滑块
    volumup_btn = lv_slider_create(video_screen);
    lv_obj_set_size(volumup_btn, 150, 30);
    lv_obj_align(volumup_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -20);
    lv_slider_set_range(volumup_btn, 0, 100);
    lv_slider_set_value(volumup_btn, volume_, LV_ANIM_ON);
    lv_obj_add_event_cb(volumup_btn, volumup_video, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_style(volumup_btn, &slider_style_bg, LV_PART_MAIN);
    lv_obj_add_style(volumup_btn, &slider_style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(volumup_btn, &slider_style_knob, LV_PART_KNOB);

    // 音量标签
    volumup_label = lv_label_create(video_screen);
    lv_obj_align(volumup_label, LV_ALIGN_BOTTOM_RIGHT, -40, -50);
    lv_label_set_text(volumup_label, "音量");
    lv_obj_add_style(volumup_label, &status_style, 0);

    // 创建定时器
    one_sec_timer = lv_timer_create(one_sec_handle, 1000, NULL);
    game_timer = lv_timer_create(read_mplayer_output, 200, NULL);
    lv_timer_pause(game_timer);
    lv_timer_pause(one_sec_timer);
    
    // 初始化状态
    video_pid = -1;
    fifo_fd = -1;
    fifo_out_fd = -1;
    volume_ = 30;
    total_time_flag = 0;
    is_playing = 0;
    is_paused = 0;
    list = 0;
    is_slider_dragging = 0;
    
    ensure_fifo();
    printf("视频界面初始化完成\n");
}

// ==================== 清理视频资源 ====================
void video_cleanup() {
    // 暂停播放
    if(is_playing && fifo_fd != -1 && !is_paused) {
        write(fifo_fd, "pause\n", 6);
        usleep(100000);
    }
    
    // 终止视频进程
    if(video_pid > 0) {
        kill(video_pid, SIGTERM);
        usleep(100000);
        waitpid(video_pid, NULL, WNOHANG);
        video_pid = -1;
    }
    
    // 关闭FIFO
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
    
    // 重置状态
    is_playing = 0;
    is_paused = 0;
    total_time_flag = 0;
    total_seconds = 0;
    current_seconds = 0;
    list = 0;
    is_slider_dragging = 0;
    
    printf("视频资源清理完成\n");
}

// ==================== 关闭视频模块 ====================
void video_demo_close(void) {
    video_cleanup();
    
    if(video_screen != NULL) {
        lv_obj_del(video_screen);
        video_screen = NULL;
    }
    
    main_grid(&style);
}