#include "LED.h"


static const char *led_io[] = {
	"/sys/class/gpio/gpio120/value",
	"/sys/class/gpio/gpio121/value",
	"/sys/class/gpio/gpio123/value",
	"/sys/class/gpio/gpio124/value",
};

//LED初始化函数
void led_init(void)
{
	
	/* 申请GPIO */
	//以只读方式打开GPIO引脚驱动文件
	int fd = open("/sys/class/gpio/export", O_WRONLY);
	if (fd != -1) 
    {
		write(fd, "120", 3);
		write(fd, "121", 3);
		write(fd, "123", 3);
		write(fd, "124", 3);
		fsync(fd);
		close(fd);
	}	
	
	/* LED1 */
	fd = open("/sys/class/gpio/gpio120/direction", O_WRONLY);
	if (fd != -1) 
    {
		write(fd, "out", 3);
		fsync(fd);
		close(fd);
	}
	
	/* LED2 */
	fd = open("/sys/class/gpio/gpio121/direction", O_WRONLY);
	if (fd != -1) 
    {
		write(fd, "out", 3);
		fsync(fd);
		close(fd);
	}
	
	/* LED3 */
	fd = open("/sys/class/gpio/gpio123/direction", O_WRONLY);
	if (fd != -1) 
    {
		write(fd, "out", 3);
		fsync(fd);
		close(fd);
	}

	/* LED4 */
	fd = open("/sys/class/gpio/gpio124/direction", O_WRONLY);
	if (fd != -1) 
    {
		write(fd, "out", 3);
		fsync(fd);
		close(fd);
	}	
}

//控制LED灯函数
void led_ctrl(s32 id, s32 enable)
{
	s32 fd;
	u32 i;
	const char *act;
	
	act = enable ? "1" : "0";

	if (id == -1) 
    {
		for (i = 0; i < COUNTOF(led_io); ++i) 
        {
			fd = open(led_io[i], O_WRONLY);
			if (fd != -1) 
            {
				write(fd, act, 1);
				fsync(fd);
				close(fd);
			}
		}				
	}
	else 
    {
		fd = open(led_io[id], O_WRONLY);
		if (fd != -1) 
        {
			write(fd, act, 1);
			fsync(fd);
			close(fd);
		}
	}
}
