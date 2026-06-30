/**
 * @file uart.h
 * @brief UART 硬件抽象层
 */

#ifndef HAL_UART_H
#define HAL_UART_H

#include "common.h"

/* UART 配置 */
typedef struct {
    uint32_t baudrate;
    uint8_t databits;   /* 5-8 */
    uint8_t stopbits;   /* 1-2 */
    uint8_t parity;     /* 0=none, 1=odd, 2=even */
} uart_cfg_t;

/* UART 回调 */
typedef void (*uart_rx_cb)(uint8_t data);

/* UART 操作接口 */
typedef struct {
    int (*init)(uint8_t id, const uart_cfg_t *cfg);
    int (*deinit)(uint8_t id);
    int (*write)(uint8_t id, const uint8_t *data, size_t len);
    int (*read)(uint8_t id, uint8_t *buf, size_t len, uint32_t timeout);
    int (*set_rx_cb)(uint8_t id, uart_rx_cb cb);
} uart_ops_t;

extern const uart_ops_t *uart_ops;

/* 简化接口 */
INLINE int uart_init(uint8_t id, uint32_t baud) {
    uart_cfg_t cfg = {baud, 8, 1, 0};
    return uart_ops->init(id, &cfg);
}

INLINE int uart_send(uint8_t id, const void *data, size_t len) {
    return uart_ops->write(id, (const uint8_t *)data, len);
}

INLINE int uart_recv(uint8_t id, void *buf, size_t len, uint32_t timeout) {
    return uart_ops->read(id, (uint8_t *)buf, len, timeout);
}

/* 发送字符串 */
INLINE int uart_puts(uint8_t id, const char *str) {
    return uart_ops->write(id, (const uint8_t *)str, strlen(str));
}

/* 格式化输出 */
int uart_printf(uint8_t id, const char *fmt, ...);

#endif /* HAL_UART_H */
