#ifndef __DATA_H  // 防止头文件重复包含
#define __DATA_H

// ========== 配置常量 ==========
#define DATAPATH "/my/data/data.txt"  // 数据文件路径（嵌入式设备存储路径）
 
// ========== 枚举定义 ==========
typedef enum {
    DATA_SAVE_FREE = 0,  // 空闲状态（未使用，可能是预留的状态标志）
    DATA_SAVE_ACT        // 激活状态
} data_save_t;

// ========== 核心数据结构 ==========
/**
 * @brief 用户数据结构体
 * @note 文件存储格式: "username password score\n"
 */
typedef struct {
    char username[20];  // 用户名（最大19字符 + '\0'）
    char password[20];  // 密码（明文存储，生产环境应加密！）
    int score;          // 历史最高分
} User;

// ========== 公开API函数 ==========
/**
 * @brief 列出所有用户信息
 * @param user_list 用户数组（输出参数）
 * @return 实际读取的用户数量（最多10个）
 */
int list_all_users(User user_list[10]);

/**
 * @brief 更新指定用户的分数
 * @param usr 用户名
 * @param new_score 新分数
 * @return 1=成功, 0=失败
 */
int update_score(char *usr, int new_score);

/**
 * @brief 保存新用户数据
 * @param usr 用户名
 * @param passd 密码
 * @param score 初始分数
 */
void save_data(char *usr, char *passd, int score);

/**
 * @brief 验证用户名和密码
 * @param usr 用户名
 * @param passd 密码
 * @return 1=验证成功, 0=失败
 */
int check_usr_passd(char *usr, char *passd);

#endif