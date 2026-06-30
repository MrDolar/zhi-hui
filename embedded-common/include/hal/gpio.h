/**
 * @file gpio.h
 * @brief GPIO 硬件抽象层
 */

#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include "common.h"

/* GPIO 模式 */
typedef enum {
    GPIO_INPUT = 0,
    GPIO_OUTPUT,
    GPIO_AF,        /* 复用功能 */
    GPIO_ANALOG,    /* 模拟 */
    GPIO_INPUT_PU,  /* 上拉输入 */
    GPIO_INPUT_PD,  /* 下拉输入 */
} gpio_mode_t;

/* GPIO 电平 */
typedef enum {
    GPIO_LOW = 0,
    GPIO_HIGH = 1,
} gpio_level_t;

/* GPIO 操作接口 */
typedef struct {
    int (*init)(uint16_t pin, gpio_mode_t mode);
    int (*write)(uint16_t pin, gpio_level_t level);
    int (*read)(uint16_t pin);
    int (*toggle)(uint16_t pin);
    int (*deinit)(uint16_t pin);
} gpio_ops_t;

/* 全局 GPIO 操作 */
extern const gpio_ops_t *gpio_ops;

/* 简化接口 */
INLINE int gpio_init(uint16_t pin, gpio_mode_t mode) {
    return gpio_ops->init(pin, mode);
}

INLINE int gpio_write(uint16_t pin, gpio_level_t level) {
    return gpio_ops->write(pin, level);
}

INLINE int gpio_read(uint16_t pin) {
    return gpio_ops->read(pin);
}

INLINE int gpio_toggle(uint16_t pin) {
    return gpio_ops->toggle(pin);
}

/* 批量操作 */
INLINE void gpio_write_port(uint16_t port, uint16_t mask, uint16_t value) {
    for (int i = 0; i < 16; i++) {
        if (mask & BIT(i)) {
            gpio_write(port + i, (value & BIT(i)) ? GPIO_HIGH : GPIO_LOW);
        }
    }
}

#endif /* HAL_GPIO_H */
