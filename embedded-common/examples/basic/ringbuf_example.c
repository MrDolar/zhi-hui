/**
 * @file ringbuf_example.c
 * @brief 环形缓冲区使用示例
 */

#include "common.h"
#include "data_structure/ringbuf.h"
#include "utility/log.h"

/* 静态创建环形缓冲区 */
RINGBUF_DEF(rb, 64);

void ringbuf_example(void) {
    LOG_I("RINGBUF", "=== Ring Buffer Example ===");

    // 写入数据
    const char *data = "Hello, Ring Buffer!";
    size_t written = ringbuf_write(&rb, (const uint8_t *)data, strlen(data));
    LOG_I("RINGBUF", "Written: %zu bytes", written);

    // 读取数据
    uint8_t buf[32];
    size_t read = ringbuf_read(&rb, buf, sizeof(buf) - 1);
    buf[read] = '\0';
    LOG_I("RINGBUF", "Read: %zu bytes, Data: %s", read, buf);

    // 测试满缓冲区
    for (int i = 0; i < 100; i++) {
        if (ringbuf_put(&rb, i & 0xFF) != OK) {
            LOG_I("RINGBUF", "Buffer full at %d", i);
            break;
        }
    }

    // 查看状态
    LOG_I("RINGBUF", "Count: %zu, Space: %zu", 
          ringbuf_count(&rb), ringbuf_space(&rb));

    // 清空
    ringbuf_flush(&rb);
    LOG_I("RINGBUF", "Flushed, Empty: %s", 
          ringbuf_is_empty(&rb) ? "Yes" : "No");
}
