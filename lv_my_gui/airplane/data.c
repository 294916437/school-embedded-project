#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include "data.h"


// 辅助函数：去除字符串中的换行符
static void trim_newline(char *str) {
    if (str == NULL) return;
    int len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

// 检查文件是否存在
static int file_exists(const char *path) {
    return access(path, F_OK) == 0;
}

// 创建默认数据文件
static void create_default_file() {
    FILE *fp = fopen(DATAPATH, "w");
    if (fp != NULL) {
        fprintf(fp, "guest 123456 0\n");
        fclose(fp);
        printf("创建默认数据文件: %s\n", DATAPATH);
    } else {
        printf("创建文件失败: %s\n", DATAPATH);
    }
}

//  用户密码匹配
int check_usr_passd(char *usr, char *passd) {
    // 1. 检查文件是否存在
    if (!file_exists(DATAPATH)) {
        printf("文件不存在，创建默认文件\n");
        create_default_file();
    }

    // 2. 打开文件进行验证
    FILE *fp = fopen(DATAPATH, "r");
    if (fp == NULL) {
        printf("无法打开文件: %s\n", DATAPATH);
        return 0;
    }

    // 3. 逐行读取验证
    char line[100];
    char file_usr[20], file_passd[20];
    int file_score;
    int match = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        // 解析每行: username password score
        if (sscanf(line, "%s %s %d", file_usr, file_passd, &file_score) == 3) {
            if (strcmp(usr, file_usr) == 0 && strcmp(passd, file_passd) == 0) {
                match = 1;
                printf("登录成功: %s\n", usr);
                break;
            }
        }
    }

    fclose(fp);

    if (!match) {
        printf("用户名或密码错误\n");
    }

    return match;
}

//  保存新用户数据（含初始得分）
void save_data(char *usr, char *passd, int score) {
    // 1. 检查文件是否存在
    if (!file_exists(DATAPATH)) {
        printf("文件不存在，创建默认文件\n");
        create_default_file();
    }

    // 2. 检查用户是否已存在
    FILE *fp = fopen(DATAPATH, "r");
    if (fp != NULL) {
        char line[100];
        char file_usr[20];
        int user_exists = 0;

        while (fgets(line, sizeof(line), fp) != NULL) {
            if (sscanf(line, "%s", file_usr) == 1) {
                if (strcmp(usr, file_usr) == 0) {
                    user_exists = 1;
                    break;
                }
            }
        }
        fclose(fp);

        if (user_exists) {
            printf("用户已存在: %s\n", usr);
            return;
        }
    }

    // 3. 追加新用户
    fp = fopen(DATAPATH, "a");
    if (fp != NULL) {
        fprintf(fp, "%s %s %d\n", usr, passd, score);
        fclose(fp);
        printf("保存新用户: %s\n", usr);
    } else {
        printf("保存失败\n");
    }
}

//  更新用户得分
int update_score(char *usr, int new_score) {
    // 1. 检查文件是否存在
    if (!file_exists(DATAPATH)) {
        printf("文件不存在，无法更新分数\n");
        return 0;
    }

    // 2. 读取所有用户到内存
    User users[100];
    int count = 0;

    FILE *fp = fopen(DATAPATH, "r");
    if (fp == NULL) {
        return 0;
    }

    char line[100];
    while (fgets(line, sizeof(line), fp) != NULL && count < 100) {
        if (sscanf(line, "%s %s %d", 
                   users[count].username, 
                   users[count].password, 
                   &users[count].score) == 3) {
            count++;
        }
    }
    fclose(fp);

    // 3. 查找并更新目标用户分数
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, usr) == 0) {
            users[i].score = new_score;
            found = 1;
            printf("更新用户 %s 分数为: %d\n", usr, new_score);
            break;
        }
    }

    if (!found) {
        printf("未找到用户: %s\n", usr);
        return 0;
    }

    // 4. 重写整个文件
    fp = fopen(DATAPATH, "w");
    if (fp == NULL) {
        return 0;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s %s %d\n", 
                users[i].username, 
                users[i].password, 
                users[i].score);
    }
    fclose(fp);

    return 1;
}

int list_all_users(User user[10]) {
    
    // 1. 检查文件是否存在
    if (!file_exists(DATAPATH)) {
        printf("文件不存在，创建默认文件\n");
        create_default_file();
    }

    // 2. 打开文件读取
    FILE *fp = fopen(DATAPATH, "r");
    if (fp == NULL) {
        printf("无法打开文件: %s\n", DATAPATH);
        return 0;
    }

    // 3. 读取所有用户（最多10个）
    char line[100];
    int count = 0;

    while (fgets(line, sizeof(line), fp) != NULL && count < 10) {
        if (sscanf(line, "%s %s %d", 
                   user[count].username, 
                   user[count].password, 
                   &user[count].score) == 3) {
            count++;
        }
    }

    fclose(fp);
    printf("读取了 %d 个用户\n", count);
    return count;
    
}