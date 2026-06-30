/**
 * @file lowpower.h
 * @brief 低功耗管理
 */

#ifndef LOWPOWER_H
#define LOWPOWER_H

#include "common.h"

/* 低功耗模式 */
typedef enum {
    LP_SLEEP = 0,       /* 睡眠模式 */
    LP_STOP,            /* 停止模式 */
    LP_STANDBY,         /* 待机模式 */
} lp_mode_t;

/* 唤醒源 */
typedef enum {
    LP_WAKE_PIN = BIT(0),
    LP_WAKE_RTC = BIT(1),
    LP_WAKE_UART = BIT(2),
    LP_WAKE_TIMER = BIT(3),
} lp_wake_src_t;

/* 初始化 */
int lp_init(void);

/* 进入低功耗 */
int lp_enter(lp_mode_t mode);

/* 设置唤醒源 */
int lp_set_wake_src(uint32_t src);

/* 配置唤醒引脚 */
int lp_config_wake_pin(uint8_t pin, bool rising);

/* 配置 RTC 唤醒 */
int lp_config_wake_rtc(uint32_t seconds);

/* 获取睡眠时间 */
uint32_t lp_get_sleep_time(void);

/* 简化接口 */
INLINE void lp_sleep(uint32_t seconds) {
    lp_config_wake_rtc(seconds);
    lp_enter(LP_SLEEP);
}

/* 休眠直到唤醒 */
INLINE void lp_deep_sleep(void) {
    lp_enter(LP_STANDBY);
}

#endif /* LOWPOWER_H */
