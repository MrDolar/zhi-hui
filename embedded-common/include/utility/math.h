/**
 * @file math.h
 * @brief 数学工具函数
 */

#ifndef MATH_H
#define MATH_H

#include "common.h"
#include <math.h>

/* 限幅 */
INLINE int clamp(int val, int min, int max) {
    return val < min ? min : (val > max ? max : val);
}

INLINE float fclamp(float val, float min, float max) {
    return val < min ? min : (val > max ? max : val);
}

/* 线性映射 */
INLINE int map(int val, int in_min, int in_max, int out_min, int out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

INLINE float fmap(float val, float in_min, float in_max, float out_min, float out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/* 平滑滤波 */
typedef struct {
    float alpha;    /* 系数 0-1 */
    float value;
} smooth_t;

INLINE float smooth_update(smooth_t *s, float new_val) {
    s->value = s->alpha * new_val + (1.0f - s->alpha) * s->value;
    return s->value;
}

/* 滑动窗口平均 */
typedef struct {
    float *buf;
    size_t size;
    size_t index;
    float sum;
} moving_avg_t;

float moving_avg_update(moving_avg_t *ma, float val);
float moving_avg_get(const moving_avg_t *ma);

/* PID 控制器 */
typedef struct {
    float kp, ki, kd;
    float setpoint;
    float integral;
    float prev_error;
    float output_min;
    float output_max;
} pid_t;

void pid_init(pid_t *pid, float kp, float ki, float kd);
float pid_update(pid_t *pid, float measurement, float dt);
void pid_set_limits(pid_t *pid, float min, float max);
void pid_reset(pid_t *pid);

/* 角度归一化 */
INLINE float normalize_angle(float angle) {
    while (angle > 180.0f) angle -= 360.0f;
    while (angle < -180.0f) angle += 360.0f;
    return angle;
}

/* 弧度/角度转换 */
#define DEG_TO_RAD  0.017453292519943295f
#define RAD_TO_DEG  57.29577951308232f

#endif /* MATH_H */
