/**
 * @file spi.h
 * @brief SPI 硬件抽象层
 */

#ifndef HAL_SPI_H
#define HAL_SPI_H

#include "common.h"

/* SPI 模式 */
typedef enum {
    SPI_MODE0 = 0,  /* CPOL=0, CPHA=0 */
    SPI_MODE1,      /* CPOL=0, CPHA=1 */
    SPI_MODE2,      /* CPOL=1, CPHA=0 */
    SPI_MODE3,      /* CPOL=1, CPHA=1 */
} spi_mode_t;

/* SPI 配置 */
typedef struct {
    uint32_t freq;      /* 频率 Hz */
    spi_mode_t mode;
    uint8_t bits;       /* 数据位 8/16 */
    bool msb_first;     /* MSB 先发 */
} spi_cfg_t;

/* SPI 操作接口 */
typedef struct {
    int (*init)(uint8_t id, const spi_cfg_t *cfg);
    int (*deinit)(uint8_t id);
    int (*transfer)(uint8_t id, const uint8_t *tx, uint8_t *rx, size_t len);
    int (*set_cs)(uint8_t id, uint8_t cs_pin, bool active);
} spi_ops_t;

extern const spi_ops_t *spi_ops;

/* 简化接口 */
INLINE int spi_init(uint8_t id, uint32_t freq) {
    spi_cfg_t cfg = {freq, SPI_MODE0, 8, true};
    return spi_ops->init(id, &cfg);
}

INLINE int spi_write(uint8_t id, const uint8_t *data, size_t len) {
    return spi_ops->transfer(id, data, NULL, len);
}

INLINE int spi_read(uint8_t id, uint8_t *data, size_t len) {
    return spi_ops->transfer(id, NULL, data, len);
}

INLINE int spi_xfer(uint8_t id, const uint8_t *tx, uint8_t *rx, size_t len) {
    return spi_ops->transfer(id, tx, rx, len);
}

/* 发送单字节 */
INLINE int spi_send_byte(uint8_t id, uint8_t data) {
    return spi_write(id, &data, 1);
}

/* 读取单字节 */
INLINE uint8_t spi_read_byte(uint8_t id) {
    uint8_t data;
    spi_read(id, &data, 1);
    return data;
}

#endif /* HAL_SPI_H */
