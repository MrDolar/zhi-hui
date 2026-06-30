/**
 * @file adc.h
 * @brief ADC 硬件抽象层
 */

#ifndef HAL_ADC_H
#define HAL_ADC_H

#include "common.h"

/* ADC 配置 */
typedef struct {
    uint8_t resolution;  /* 位数 8-12-16 */
    uint32_t sample_time; /* 采样时间 */
} adc_cfg_t;

/* ADC 操作接口 */
typedef struct {
    int (*init)(uint8_t id, const adc_cfg_t *cfg);
    int (*deinit)(uint8_t id);
    int (*read)(uint8_t id, uint8_t channel);
    int (*read_voltage)(uint8_t id, uint8_t channel);  /* 返回 mV */
} adc_ops_t;

extern const adc_ops_t *adc_ops;

/* 简化接口 */
INLINE int adc_init(uint8_t id, uint8_t bits) {
    adc_cfg_t cfg = {bits, 0};
    return adc_ops->init(id, &cfg);
}

INLINE int adc_read(uint8_t id, uint8_t ch) {
    return adc_ops->read(id, ch);
}

INLINE int adc_read_mv(uint8_t id, uint8_t ch) {
    return adc_ops->read_voltage(id, ch);
}

/* 多次采样取平均 */
INLINE int adc_read_avg(uint8_t id, uint8_t ch, uint8_t samples) {
    int sum = 0;
    for (int i = 0; i < samples; i++) {
        sum += adc_ops->read(id, ch);
    }
    return sum / samples;
}

/* ADC 值转电压 */
INLINE uint32_t adc_to_voltage(uint32_t adc_val, uint32_t vref_mv, uint8_t bits) {
    return adc_val * vref_mv / ((1 << bits) - 1);
}

#endif /* HAL_ADC_H */
