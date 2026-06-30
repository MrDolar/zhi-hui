/**
 * @file utils.h
 * @brief 通用工具函数
 */

#ifndef UTILS_H
#define UTILS_H

#include "common.h"
#include <string.h>
#include <stdlib.h>

/* 随机数 */
void random_seed(uint32_t seed);
uint32_t random_uint32(void);
int random_range(int min, int max);
float random_float(float min, float max);

/* 时间戳 */
typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t millisecond;
} datetime_t;

uint32_t datetime_to_timestamp(const datetime_t *dt);
void timestamp_to_datetime(uint32_t timestamp, datetime_t *dt);
uint32_t get_timestamp(void);

/* 日期格式化 */
size_t datetime_to_str(const datetime_t *dt, char *buf, size_t len);
size_t timestamp_to_str(uint32_t ts, char *buf, size_t len);

/* 位计数 */
INLINE uint32_t popcount(uint32_t x) {
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    return (((x + (x >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

/* 前导零 */
INLINE uint32_t clz(uint32_t x) {
    return __builtin_clz(x);
}

/* 尾随零 */
INLINE uint32_t ctz(uint32_t x) {
    return __builtin_ctz(x);
}

/* 位反转 */
INLINE uint32_t bit_reverse(uint32_t x) {
    x = ((x & 0xAAAAAAAA) >> 1) | ((x & 0x55555555) << 1);
    x = ((x & 0xCCCCCCCC) >> 2) | ((x & 0x33333333) << 2);
    x = ((x & 0xF0F0F0F0) >> 4) | ((x & 0x0F0F0F0F) << 4);
    return __builtin_bswap32(x);
}

/* 循环移长 */
INLINE uint32_t rotl32(uint32_t x, uint32_t n) {
    return (x << n) | (x >> (32 - n));
}

INLINE uint32_t rotr32(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32 - n));
}

/* 整数转 BCD */
INLINE uint8_t int_to_bcd(uint8_t val) {
    return ((val / 10) << 4) | (val % 10);
}

/* BCD 转整数 */
INLINE uint8_t bcd_to_int(uint8_t bcd) {
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

/* 断言宏 */
#ifdef ENABLE_ASSERT
#define ASSERT(cond) do { \
    if (!(cond)) { \
        log_write(LOG_ERROR, "ASSERT", "%s:%d: %s", __FILE__, __LINE__, #cond); \
        while(1); \
    } \
} while(0)
#else
#define ASSERT(cond) ((void)0)
#endif

#endif /* UTILS_H */
