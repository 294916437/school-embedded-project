#ifndef __BUZZER_H
#define __BUZZER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "MK_macro.h"
#include "MK_types.h"
#include "MK_func.h"

void buzz_init(void);
void buzz_ctrl(s32 id, s32 enable);

#endif
