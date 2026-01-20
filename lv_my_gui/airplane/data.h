#ifndef _DATA_H_
#define _DATA_H_

#define DATAPATH "/my/data/data.txt"
 
typedef enum {
    DATA_SAVE_FREE = 0,
    DATA_SAVE_ACT
}data_save_t;

// 用户数据结构体
typedef struct {
    char username[20];
    char password[20];
    int score;
} User;

int list_all_users(User user_list[10]) ;
int update_score(char *usr, int new_score);
void save_data(char *usr, char *passd, int score);
int check_usr_passd(char *usr, char *passd);
#endif