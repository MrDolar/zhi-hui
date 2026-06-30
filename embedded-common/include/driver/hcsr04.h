/**
 * @file hcsr04.h
 * @brief HC-SR04 超声波测距模块驱动
 */

#ifndef HCSR04_H
#define HCSR04_H

#include "common.h"

/* HC-SR04 设备 */
typedef struct {
    uint8_t trig_pin;
    uint8_t echo_pin;
} hcsr04_t;

/* 初始化 */
int hcsr04_init(hcsr04_t *dev, uint8_t trig, uint8_t echo);

/* 读取距离（厘米） */
float hcsr04_read_cm(hcsr04_t *dev);

/* 读取距离（英寸） */
float hcsr04_read_inch(hcsr04_t *dev);

/* 多次采样取平均 */
float hcsr04_read_avg(hcsr04_t *dev, uint8_t samples);

/* 距离转时间 */
INLINE uint32_t hcsr04_distance_to_us(float cm) {
    return (uint32_t)(cm * 2.0f / 0.0343f);
}

/* 时间转距离 */
INLINE float hcsr04_us_to_distance(uint32_t us) {
    return us * 0.0343f / 2.0f;
}

#endif /* HCSR04_H */
