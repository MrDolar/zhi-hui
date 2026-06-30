/**
 * @file pwm.h
 * @brief PWM 硬件抽象层
 */

#ifndef HAL_PWM_H
#define HAL_PWM_H

#include "common.h"

/* PWM 配置 */
typedef struct {
    uint32_t freq;      /* 频率 Hz */
    uint8_t duty;       /* 占空比 0-100 */
    bool polarity;      /* 极性 */
} pwm_cfg_t;

/* PWM 操作接口 */
typedef struct {
    int (*init)(uint8_t id, uint8_t ch, const pwm_cfg_t *cfg);
    int (*deinit)(uint8_t id, uint8_t ch);
    int (*set_duty)(uint8_t id, uint8_t ch, uint8_t duty);
    int (*set_freq)(uint8_t id, uint8_t ch, uint32_t freq);
    int (*start)(uint8_t id, uint8_t ch);
    int (*stop)(uint8_t id, uint8_t ch);
} pwm_ops_t;

extern const pwm_ops_t *pwm_ops;

/* 简化接口 */
INLINE int pwm_init(uint8_t id, uint8_t ch, uint32_t freq, uint8_t duty) {
    pwm_cfg_t cfg = {freq, duty, false};
    return pwm_ops->init(id, ch, &cfg);
}

INLINE int pwm_set_duty(uint8_t id, uint8_t ch, uint8_t duty) {
    return pwm_ops->set_duty(id, ch, duty);
}

INLINE int pwm_start(uint8_t id, uint8_t ch) {
    return pwm_ops->start(id, ch);
}

INLINE int pwm_stop(uint8_t id, uint8_t ch) {
    return pwm_ops->stop(id, ch);
}

/* 线性映射 */
INLINE uint8_t pwm_map(int val, int in_min, int in_max) {
    return (uint8_t)((val - in_min) * 100 / (in_max - in_min));
}

#endif /* HAL_PWM_H */
