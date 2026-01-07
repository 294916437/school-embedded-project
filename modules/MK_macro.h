#ifndef MK_MACRO_H
#define MK_MACRO_H

#define NDEBUG
#include <assert.h>

/* application switch */
//#define ZZF_DEBUG

/* platform atomic define */
#define ATOMIC_GET(v)		__sync_fetch_and_add(&v, 0)
#define ATOMIC_SET(v)		__sync_lock_test_and_set(&v, 1)
#define ATOMIC_CLR(v)		__sync_lock_release(&v)
#define ATOMIC_INC(v)		__sync_fetch_and_add(&v, 1)
#define ATOMIC_STM(v, d)	__sync_lock_test_and_set(&v, d)

/* system define */
#define UNUSED(a)		((void)(a))
#define COUNTOF(a)           	(sizeof(a) / sizeof((a)[0]))  
#define MIN(a,b)             	((a) > (b) ? (b) : (a))
#define MAX(a,b)		((a) > (b) ? (a) : (b))
#define ASCII_TO_INT(a)		((a) - '0')

#ifndef FALSE
  #define FALSE			0
  #undef  TRUE
  #define TRUE			!FALSE
#endif

/* Serial communicate (9600,8,N,1) */
#define O2B_PORT			"/dev/ttyS2"

#define O2B_TX_MAX		272
#define O2B_TXH_SIZE		6
#define O2B_TH1			0xa5
#define O2B_TH2			0x96

#define O2B_RX_MAX		272
#define O2B_RXH_SIZE		6	/* 数据头帧大小 */
#define O2B_RH1			0x5a
#define O2B_RH2			0x69

#define O2B_CKS_SIZE		2

#define POS_HEAD0		0
#define POS_HEAD1		1
#define POS_PAGE		2
#define POS_CMD			3
#define POS_CTRLID		4
#define POS_PARAMS_LEN		5
#define POS_PARAMS		6
#define POS_CKS(n)		(6 + (n))

#define COMM_INTERVAL		100	/* 通讯间隔(ms) */
#define ERR_COUNTER_MAX		60	/* 60 * 100ms */

#define CMD_DIGITAL		0x10
#define CMD_TIME		0x12
#define CMD_LIGHT		0x15
#define CMD_BUTTON		0x20

#define CTL_DIGITAL_MAX		4
#define CTL_TIME_MAX		2
#define CTL_LIGHT_MAX		7
#define CTL_BUTTON_MAX		6

#define UNDEFINED		-1
#define ZONE_A			0
#define ZONE_B			1
#define ZONE_MAX		2
#define CH1			0
#define CH2			1
#define CH3			2
#define CH_MAX			3
#define CH1_EN			(1 << 0)
#define CH2_EN			(1 << 1)
#define CH3_EN			(1 << 2)
#define ZONE_EN			(1 << 7)
#define CH1_MASK		(1 << 0)
#define CH2_MASK		(1 << 1)
#define CH3_MASK		(1 << 2)
#define ZONE_MASK		(1 << 7)
#define PLAN_MAX		64
//#define NAME_MAX		8

#define RM_MANUAL		1
#define RM_AUTO			2

#define LANG_MAX		2
#define ALARM_MAX		3

#define ALM_NONE		0
#define ALM_OVERHEAT		0x80		/* 超温 */
#define ALM_LACK_WATER		0x40		/* 缺水 */
#define ALM_WQ			0x20		/* 水质差报警 */
#define ALM_WQ_IDX		0		/* 水质差报警内容索引 */
#define ALM_OVERHEAT_IDX	1		/* 超温报警内容索引 */
#define ALM_LACK_WATER_IDX	2		/* 缺水报警内容索引 */

#define OBJ_FLOW		0
#define OBJ_TIME		1

#define TIMER_SLICE		1000
#define TIME_DEFAULT		(30 * 60)
#define TIME_INTERVAL		(30 * 60)
#define TIME_MAX		(999 * 60)

#define FLOW_LEVEL_MAX		4
#define FLOW_INTERVAL_MAX	10

#define LOOP_ONEWAY		0
#define LOOP_CIRCLE		1

#define PWR_HONGYI		0
#define PWR_YOUJING		1

#define ID_TOP_LEFT		1
#define ID_TOP_RIGHT		2
#define ID_BOTTOM_LEFT		3
#define ID_BOTTOM_RIGHT		4

#define PG_MAIN			0
#define PG_SETTING		1

#define PARAMS_MAGIC		0x53549901
#define PARAMS_ADDR		0
	
/* similar linux define */
#if defined(__GNUC__)
  #define likely(x) 	__builtin_expect(!!(x), 1)
  #define unlikely(x) 	__builtin_expect(!!(x), 0)
  
  #define typecheck(type,x) \
  ({      type __dummy; \
        typeof(x) __dummy2; \
        (void)(&__dummy == &__dummy2); \
        1; \
  })
  
  #define time_after(a,b) \
    (typecheck(u32, a) && \
     typecheck(u32, b) && \
     ((long)(b) - (long)(a) < 0))
  #define time_before(a,b)	time_after(b,a)
#else
  #define likely(x) 	x
  #define unlikely(x) 	x
  
  #define time_after(a,b)	((long)(b) - (long)(a) < 0)
  #define time_before(a,b)	time_after(b,a)
#endif
#endif
