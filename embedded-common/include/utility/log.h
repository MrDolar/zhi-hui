/**
 * @file log.h
 * @brief 日志系统
 */

#ifndef LOG_H
#define LOG_H

#include "common.h"

/* 日志级别 */
typedef enum {
    LOG_NONE = 0,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_VERBOSE,
} log_level_t;

/* 日志输出接口 */
typedef void (*log_output_cb)(const char *msg, size_t len);

/* 初始化 */
void log_init(log_level_t level);

/* 设置级别 */
void log_set_level(log_level_t level);

/* 设置输出接口 */
void log_set_output(log_output_cb cb);

/* 日志输出 */
void log_write(log_level_t level, const char *tag, const char *fmt, ...);

/* 简化宏 */
#define LOG_E(tag, fmt, ...) log_write(LOG_ERROR, tag, fmt, ##__VA_ARGS__)
#define LOG_W(tag, fmt, ...) log_write(LOG_WARN, tag, fmt, ##__VA_ARGS__)
#define LOG_I(tag, fmt, ...) log_write(LOG_INFO, tag, fmt, ##__VA_ARGS__)
#define LOG_D(tag, fmt, ...) log_write(LOG_DEBUG, tag, fmt, ##__VA_ARGS__)
#define LOG_V(tag, fmt, ...) log_write(LOG_VERBOSE, tag, fmt, ##__VA_ARGS__)

/* 带颜色输出 */
#define LOG_RED     "\033[31m"
#define LOG_GREEN   "\033[32m"
#define LOG_YELLOW  "\033[33m"
#define LOG_BLUE    "\033[34m"
#define LOG_RESET   "\033[0m"

/* 十六进制转储 */
void log_hex_dump(const char *tag, const void *data, size_t len);

/* 性能测量 */
#define PERF_START(name) uint32_t name##_start = get_tick()
#define PERF_END(name) LOG_D("PERF", "%s: %lu ms", #name, get_tick_ms() - name##_start)

#endif /* LOG_H */
