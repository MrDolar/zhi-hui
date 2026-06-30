/**
 * @file i2c.h
 * @brief I2C 硬件抽象层
 */

#ifndef HAL_I2C_H
#define HAL_I2C_H

#include "common.h"

/* I2C 操作接口 */
typedef struct {
    int (*init)(uint8_t id, uint32_t freq);
    int (*deinit)(uint8_t id);
    int (*write)(uint8_t id, uint8_t addr, const uint8_t *data, size_t len);
    int (*read)(uint8_t id, uint8_t addr, uint8_t *buf, size_t len);
    int (*mem_write)(uint8_t id, uint8_t addr, uint16_t mem_addr, const uint8_t *data, size_t len);
    int (*mem_read)(uint8_t id, uint8_t addr, uint16_t mem_addr, uint8_t *buf, size_t len);
} i2c_ops_t;

extern const i2c_ops_t *i2c_ops;

/* 简化接口 */
INLINE int i2c_init(uint8_t id, uint32_t freq) {
    return i2c_ops->init(id, freq);
}

INLINE int i2c_write_reg(uint8_t id, uint8_t addr, uint8_t reg, uint8_t val) {
    uint8_t buf[2] = {reg, val};
    return i2c_ops->write(id, addr, buf, 2);
}

INLINE int i2c_read_reg(uint8_t id, uint8_t addr, uint8_t reg, uint8_t *val) {
    i2c_ops->write(id, addr, &reg, 1);
    return i2c_ops->read(id, addr, val, 1);
}

/* 读取多寄存器 */
INLINE int i2c_read_regs(uint8_t id, uint8_t addr, uint8_t reg, uint8_t *buf, size_t len) {
    i2c_ops->write(id, addr, &reg, 1);
    return i2c_ops->read(id, addr, buf, len);
}

/* 写入多寄存器 */
INLINE int i2c_write_regs(uint8_t id, uint8_t addr, uint8_t reg, const uint8_t *data, size_t len) {
    uint8_t buf[len + 1];
    buf[0] = reg;
    memcpy(buf + 1, data, len);
    return i2c_ops->write(id, addr, buf, len + 1);
}

/* 检测设备 */
INLINE bool i2c_detect(uint8_t id, uint8_t addr) {
    uint8_t dummy;
    return i2c_ops->read(id, addr, &dummy, 0) == OK;
}

#endif /* HAL_I2C_H */
