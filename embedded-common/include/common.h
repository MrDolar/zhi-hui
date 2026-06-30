/**
 * @file common.h
 * @brief 通用定义和宏
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* 错误码 */
typedef enum {
    OK = 0,
    ERR = -1,
    ERR_PARAM = -2,
    ERR_MEM = -3,
    ERR_TIMEOUT = -4,
    ERR_BUSY = -5,
    ERR_NOT_FOUND = -6,
} err_t;

/* 位操作 */
#define BIT(n)              (1U << (n))
#define SET_BIT(x, n)       ((x) |= BIT(n))
#define CLR_BIT(x, n)       ((x) &= ~BIT(n))
#define TOG_BIT(x, n)       ((x) ^= BIT(n))
#define GET_BIT(x, n)       (((x) >> (n)) & 1U)

/* 数组大小 */
#define ARRAY_SIZE(a)       (sizeof(a) / sizeof((a)[0]))

/* 最小/最大 */
#define MIN(a, b)           ((a) < (b) ? (a) : (b))
#define MAX(a, b)           ((a) > (b) ? (a) : (b))
#define CLAMP(x, lo, hi)    MIN(MAX(x, lo), hi)

/* 对齐 */
#define ALIGN_UP(x, a)      (((x) + (a) - 1) & ~((a) - 1))
#define ALIGN_DOWN(x, a)    ((x) & ~((a) - 1))
#define IS_ALIGNED(x, a)    (((x) & ((a) - 1)) == 0)

/* 字节序转换 */
#define SWAP16(x)           ((uint16_t)(((x) >> 8) | ((x) << 8)))
#define SWAP32(x)           ((uint32_t)(((x) >> 24) | (((x) >> 8) & 0xFF00) | \
                              (((x) << 8) & 0xFF0000) | ((x) << 24)))

/* 打包/解包 */
#define MK_U16(h, l)        ((uint16_t)(((h) << 8) | (l)))
#define MK_U32(a, b, c, d)  ((uint32_t)(((a) << 24) | ((b) << 16) | ((c) << 8) | (d)))
#define HI_BYTE(x)          ((uint8_t)((x) >> 8))
#define LO_BYTE(x)          ((uint8_t)((x) & 0xFF))

/* 容器宏 */
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

/* 调试宏 */
#ifdef DEBUG
#define ASSERT(cond) do { if (!(cond)) while(1); } while(0)
#else
#define ASSERT(cond) ((void)0)
#endif

/* 平台抽象 */
#define WEAK                __attribute__((weak))
#define PACKED              __attribute__((packed))
#define ALIGNED(n)          __attribute__((aligned(n)))
#define SECTION(s)          __attribute__((section(s)))
#define NORETURN            __attribute__((noreturn))
#define INLINE              static inline __attribute__((always_inline))

/* 时间宏 */
#define MS_TO_TICKS(ms)     ((ms) * SYSTEM_TICK_FREQ / 1000)
#define TICKS_TO_MS(ticks)  ((ticks) * 1000 / SYSTEM_TICK_FREQ)
#define US_TO_TICKS(us)     ((us) * SYSTEM_TICK_FREQ / 1000000)

#endif /* COMMON_H */
