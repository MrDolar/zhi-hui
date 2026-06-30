/**
 * @file ringbuf.h
 * @brief 环形缓冲区
 */

#ifndef RINGBUF_H
#define RINGBUF_H

#include "common.h"

/* 环形缓冲区结构 */
typedef struct {
    uint8_t *buf;
    size_t size;
    size_t head;
    size_t tail;
    size_t count;
} ringbuf_t;

/* 初始化 */
INLINE int ringbuf_init(ringbuf_t *rb, uint8_t *buf, size_t size) {
    rb->buf = buf;
    rb->size = size;
    rb->head = rb->tail = rb->count = 0;
    return OK;
}

/* 静态创建宏 */
#define RINGBUF_DEF(name, size) \
    static uint8_t name##_buf[size]; \
    static ringbuf_t name = {name##_buf, size, 0, 0, 0}

/* 写入一字节 */
INLINE int ringbuf_put(ringbuf_t *rb, uint8_t data) {
    if (rb->count >= rb->size) return ERR;
    rb->buf[rb->head] = data;
    rb->head = (rb->head + 1) % rb->size;
    rb->count++;
    return OK;
}

/* 读取一字节 */
INLINE int ringbuf_get(ringbuf_t *rb, uint8_t *data) {
    if (rb->count == 0) return ERR;
    *data = rb->buf[rb->tail];
    rb->tail = (rb->tail + 1) % rb->size;
    rb->count--;
    return OK;
}

/* 写入多字节 */
INLINE size_t ringbuf_write(ringbuf_t *rb, const uint8_t *data, size_t len) {
    size_t i;
    for (i = 0; i < len && ringbuf_put(rb, data[i]) == OK; i++);
    return i;
}

/* 读取多字节 */
INLINE size_t ringbuf_read(ringbuf_t *rb, uint8_t *buf, size_t len) {
    size_t i;
    for (i = 0; i < len && ringbuf_get(rb, &buf[i]) == OK; i++);
    return i;
}

/* 查看但不移除 */
INLINE int ringbuf_peek(const ringbuf_t *rb, uint8_t *data) {
    if (rb->count == 0) return ERR;
    *data = rb->buf[rb->tail];
    return OK;
}

/* 状态查询 */
INLINE size_t ringbuf_count(const ringbuf_t *rb) { return rb->count; }
INLINE size_t ringbuf_space(const ringbuf_t *rb) { return rb->size - rb->count; }
INLINE bool ringbuf_is_full(const ringbuf_t *rb) { return rb->count >= rb->size; }
INLINE bool ringbuf_is_empty(const ringbuf_t *rb) { return rb->count == 0; }

/* 清空 */
INLINE void ringbuf_flush(ringbuf_t *rb) {
    rb->head = rb->tail = rb->count = 0;
}

#endif /* RINGBUF_H */
