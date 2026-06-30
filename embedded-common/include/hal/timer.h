/**
 * @file timer.h
 * @brief 定时器硬件抽象层
 */

#ifndef HAL_TIMER_H
#define HAL_TIMER_H

#include "common.h"

/* 定时器回调 */
typedef void (*timer_cb)(void *arg);

/* 定时器模式 */
typedef enum {
    TIMER_ONCE = 0,     /* 单次 */
    TIMER_PERIODIC,     /* 周期 */
} timer_mode_t;

/* 定时器操作接口 */
typedef struct {
    int (*init)(uint8_t id, uint32_t us, timer_mode_t mode, timer_cb cb, void *arg);
    int (*deinit)(uint8_t id);
    int (*start)(uint8_t id);
    int (*stop)(uint8_t id);
    int (*set_period)(uint8_t id, uint32_t us);
    uint32_t (*get_count)(uint8_t id);
} timer_ops_t;

extern const timer_ops_t *timer_ops;

/* 简化接口 */
INLINE int timer_init(uint8_t id, uint32_t us, timer_cb cb) {
    return timer_ops->init(id, us, TIMER_PERIODIC, cb, NULL);
}

INLINE int timer_once(uint8_t id, uint32_t us, timer_cb cb) {
    return timer_ops->init(id, us, TIMER_ONCE, cb, NULL);
}

INLINE int timer_start(uint8_t id) {
    return timer_ops->start(id);
}

INLINE int timer_stop(uint8_t id) {
    return timer_ops->stop(id);
}

/* 延时函数 */
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

/* 系统滴答 */
uint32_t get_tick(void);
uint32_t get_tick_ms(void);

/* 超时检测 */
INLINE bool is_timeout(uint32_t start, uint32_t timeout_ms) {
    return (get_tick_ms() - start) >= timeout_ms;
}

#endif /* HAL_TIMER_H */
