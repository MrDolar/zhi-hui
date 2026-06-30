/**
 * @file queue.h
 * @brief 队列（线程安全）
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"
#include "ringbuf.h"

/* 队列结构 */
typedef struct {
    ringbuf_t rb;
    void (*lock)(void);
    void (*unlock)(void);
} queue_t;

/* 初始化 */
INLINE int queue_init(queue_t *q, uint8_t *buf, size_t size) {
    ringbuf_init(&q->rb, buf, size);
    q->lock = NULL;
    q->unlock = NULL;
    return OK;
}

/* 设置锁（用于多线程） */
INLINE void queue_set_lock(queue_t *q, void (*lock)(void), void (*unlock)(void)) {
    q->lock = lock;
    q->unlock = unlock;
}

/* 入队 */
INLINE int queue_push(queue_t *q, uint8_t data) {
    int ret;
    if (q->lock) q->lock();
    ret = ringbuf_put(&q->rb, data);
    if (q->unlock) q->unlock();
    return ret;
}

/* 出队 */
INLINE int queue_pop(queue_t *q, uint8_t *data) {
    int ret;
    if (q->lock) q->lock();
    ret = ringbuf_get(&q->rb, data);
    if (q->unlock) q->unlock();
    return ret;
}

/* 批量入队 */
INLINE size_t queue_push_batch(queue_t *q, const uint8_t *data, size_t len) {
    size_t ret;
    if (q->lock) q->lock();
    ret = ringbuf_write(&q->rb, data, len);
    if (q->unlock) q->unlock();
    return ret;
}

/* 批量出队 */
INLINE size_t queue_pop_batch(queue_t *q, uint8_t *buf, size_t len) {
    size_t ret;
    if (q->lock) q->lock();
    ret = ringbuf_read(&q->rb, buf, len);
    if (q->unlock) q->unlock();
    return ret;
}

/* 状态查询 */
INLINE size_t queue_count(const queue_t *q) { return ringbuf_count(&q->rb); }
INLINE bool queue_is_empty(const queue_t *q) { return ringbuf_is_empty(&q->rb); }

#endif /* QUEUE_H */
