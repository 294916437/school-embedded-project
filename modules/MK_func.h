#ifndef MK_FUNC_H
#define MK_FUNC_H

#include <stdio.h>

/* debug function define */
#if defined(ZZF_DEBUG)
  #define dprint		printf
#else
  #define dprint(...)
#endif
#define eprint			printf

/* Fonts declaration */
LV_FONT_DECLARE(micross_42);
LV_FONT_DECLARE(sans_m_30);
LV_FONT_DECLARE(msyh_29);
LV_FONT_DECLARE(msyh_26);
LV_FONT_DECLARE(sans_r_24);
LV_FONT_DECLARE(sans_r_20);

LV_FONT_DECLARE(lv_font_montserrat_18);
LV_FONT_DECLARE(lv_font_montserrat_20);

/* Implement by MK_sys.c */
extern void 		system_delay(s32 ms);
extern u32 		get_current_time(void);

/* Implement by MK_style.c */
extern void		project_style_init(void);

/* Implement by MK_main.c */
extern s32 		main_init(void);
extern void		main_deinit(void);
extern void 		main_update(void);
extern void 		update_control(u8 *params);


/* Implement by MK_data.c */

/* Implement by MK_e2prom.c */
//extern s32 		e2prom_init(void);
//extern void 		e2prom_deinit(void);
//extern s32 		e2prom_write(u8 *buf, u16 addr, s32 len, u32 timeout);
//extern s32 		e2prom_read(u8 *buf, u16 addr, s32 len, u32 timeout);

/* Implement by MK_pool.c */
extern s32 		pool_init(u32 bs, s32 depth, s32 size);
extern void 		pool_deinit(void);
extern void 		pool_reset(void);
extern void 		pool_push_idle(void *node);
extern void 		pool_push_work(void *node);
extern void*		pool_pop_idle(void);
extern void*		pool_pop_work(void);

/* Implement by MK_rtc_test.c */
extern s32 		rtc_test_open(void *args);
extern void		rtc_test_close(void);
extern void 		rtc_test_update(void);

/* Implement by MK_led_test.c */
extern s32 		led_test_open(void *args);
extern void		led_test_close(void);

/* Implement by MK_buzz_test.c */
extern s32 		buzzer_test_open(void *args);
extern void		buzzer_test_close(void);

/* Implement by MK_bl_test.c */
extern s32 		bl_test_open(void *args);
extern void		bl_test_close(void);

/* Implement by MK_key_test.c */
extern s32 		key_test_open(void *args);
extern void		key_test_close(void);
extern void 		key_test_update(void);

/* Implement by MK_ls_test.c */
extern s32 		ls_test_open(void *args);
extern void		ls_test_close(void);
extern void 		ls_test_update(void);

/* Implement by MK_adc_test.c */
extern s32 		adc_test_open(void *args);
extern void		adc_test_close(void);
extern void 		adc_test_update(void);

/* Implement by MK_gs_test.c */
extern s32 		gs_test_open(void *args);
extern void		gs_test_close(void);
extern void 		gs_test_update(void);

/* Implement by MK_audio_test.c */
extern s32 		audio_test_open(void *args);
extern void		audio_test_close(void);

/* Implement by MK_ts_test.c */
extern s32 		ts_test_open(void *args);
extern void		ts_test_close(void);
extern void 		ts_test_update(void);

/* Implement by MK_udisk_test.c */
extern s32 		udisk_test_open(void *args);
extern void		udisk_test_close(void);

/* Implement by MK_tf_test.c */
extern s32 		tf_test_open(void *args);
extern void		tf_test_close(void);

/* Implement by MK_uart_test.c */
extern s32 		uart_test_open(void *args);
extern void		uart_test_close(void);

/* Implement by MK_can_test.c */
extern s32 		can_test_open(void *args);
extern void		can_test_close(void);
extern void		can_test_update(void);

/* Implement by MK_lan_test.c */
extern s32 		lan_test_open(void *args);
extern void		lan_test_close(void);
extern void		lan_test_update(void);

/* Implement by MK_m4g_test.c */
extern s32 		m4g_test_open(void *args);
extern void		m4g_test_close(void);
extern void		m4g_test_update(void);

/* Implement by MK_wifi_test.c */
extern s32 		wifi_test_open(void *args);
extern void		wifi_test_close(void);
extern void		wifi_test_update(void);

/* Implement by MK_bt_test.c */
extern s32 		bt_test_open(void *args);
extern void		bt_test_close(void);

/* Implement by MK_e2prom_test.c */
extern s32 		e2prom_test_open(void *args);
extern void		e2prom_test_close(void);

#endif
