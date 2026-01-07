#include "buzzer.h"

static const char *buzz_io[] = 
{
	"/sys/class/gpio/gpio111/value",
};

void buzz_init(void)
{
	int fd;
	
	/* 申请GPIO */
	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (fd != -1) 
    {
		write(fd, "111", 3);
		fsync(fd);
		close(fd);
	}	
	
	/* BUZZ */
	fd = open("/sys/class/gpio/gpio111/direction", O_WRONLY);
	if (fd != -1) 
    {
		write(fd, "out", 3);
		fsync(fd);
		close(fd);
	}
}

//控制蜂鸣器函数
void buzz_ctrl(s32 id, s32 enable)
{
	s32 fd;
	u32 i;
	const char *act;
	
	act = enable ? "1" : "0";

	if (id == -1) 
    {
		for (i = 0; i < COUNTOF(buzz_io); ++i) 
        {
			fd = open(buzz_io[i], O_WRONLY);
			if (fd != -1) {
				write(fd, act, 1);
				fsync(fd);
				close(fd);
			}
		}				
	}
	else 
    {
		fd = open(buzz_io[id], O_WRONLY);
		if (fd != -1) 
        {
			write(fd, act, 1);
			fsync(fd);
			close(fd);
		}
	}
}
