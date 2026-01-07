#ifndef MK_TYPES_H
#define MK_TYPES_H

#include <stdbool.h>

typedef unsigned long long	u64;     	/* 64-bit 无符号整形 */
typedef unsigned int		u32;     	/* 32-bit 无符号整形 */
typedef unsigned short	u16;     	/* 16-bit 无符号整形 */
typedef unsigned char		u8;      	/* 8-bit 无符号整形 */

typedef signed long long	s64;     	/* 64-bit 有符号整形 */
typedef signed int		s32;     	/* 32-bit 有符号整形 */
typedef signed short		s16;     	/* 16-bit 有符号整形 */
typedef signed char      	s8;      	/* 8-bit 有符号整形 */

typedef float             	f32;     	/* 32-bit 浮点类型 */
typedef double			f64;	 	/* 64-bit 浮点类型 */

/* style id define */
enum {
	STYLE_PAGE	= 0,
	STYLE_PAGE_HEAD,
	STYLE_PRESS,
	STYLE_LIGHT_TITLE,
	STYLE_LIGHT_TIPS,
	STYLE_SLIDER_KNOB,
	STYLE_ROLLER_MAIN,
	STYLE_ROLLER_FOCUS,
	STYLE_RADIUS_30,
	STYLE_CHANNEL_HEAD,
	STYLE_CHANNEL_TITLE,
	STYLE_ROLLER_TIME,
	STYLE_ROLLER_TIME_FOCUS,
	STYLE_ROLLER_COLON,
	STYLE_ARC_KNOB,
	STYLE_LISTVIEW,
	STYLE_LIST,
	STYLE_LIST_HEAD,
	STYLE_LIST_TITLE,
	STYLE_SETTING,
	STYLE_BUTTON,
	STYLE_WARNING,
	STYLE_LABEL,
	STYLE_CIRCLE,
	STYLE_STATUSBAR,
	STYLE_NONE,
	
};

/* structure define 
 */
struct list_head {
	struct list_head *next, *prev;
};

struct temp_transform {
	s32 temp;
	s32 registor;
};

struct error_handle {
	s32 valid;
	u32 timer;
};

struct detect_timer {	
	s32 alive;
	s32 enter_detect;
	u32 stay_time;
	s32 leave_detect;
	u32 leave_time;
};

struct adjustment {
	s32 channel;
	s32 value;
	s32 enable;
};

struct rect_x1 {
	s16 x;
	s16 y;
	s16 w;
	s16 h;
};

struct coord_x1 {
	s16 x0;
	s16 y0;
};

struct coord_x3 {
	s16 x0;
	s16 y0;
	s16 x1;
	s16 y1;
	s16 x2;
	s16 y2;
};

struct plan_data {
	u8 enable;
	u8 ch[CH_MAX];
	u32 time;
};

struct channel_data {
	u8 enable;
	u8 manual[CH_MAX];
	//char name[CH_MAX][NAME_MAX];
	struct plan_data plan[PLAN_MAX];
	s32 plan_count;
};

struct flow_data {
	s32 index;
	const s32 count;
	const s32 data[FLOW_INTERVAL_MAX];
};

struct params_data {
	s32 language;
	s32 display;
	s32 power;
	s32 level;
	s32 loop_mode;
	s32 rise_time;
};

struct params_storage_data {
	u32 magic;
	struct params_data data;
	u32 crc;
};

struct channel_data_file {
	u32 magic;
	struct channel_data zone[ZONE_MAX];
	u32 crc;
};

struct runtime_data {
	s32 flow_max;
	s32 flow_cur;
	s32 power;
	s32 rise_time;	
};

struct delay_update {
	s32 changed;
	u32 changed_time;
};

struct power_type {
	s32 id;
	const char *name;
};

struct detect {
	s32 index;
	u32 key;
	u32 timer;
};

struct queue_data {
	s32 id;
	void *buffer;
	u32 var[0];
};

struct queue_node {
	struct list_head list;
	struct queue_data data;
	s32 no;
	u32 mask;
	s32 completed;	
};

struct win_params_t {
	const char *name;
	s32 (*open)(void *args);
	void (*close)();
	void (*update)();
};

#endif
