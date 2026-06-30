/**
 * @file wdt.h
 * @brief 看门狗
 */

#ifndef WDT_H
#define WDT_H

#include "common.h"

/* 看门狗配置 */
typedef struct {
    uint32_t timeout_ms;
    bool auto_feed;     /* 自动喂狗 */
} wdt_cfg_t;

/* 初始化 */
int wdt_init(const wdt_cfg_t *cfg);

/* 喂狗 */
void wdt_feed(void);

/* 启动看门狗 */
int wdt_start(void);

/* 停止看门狗 */
int wdt_stop(void);

/* 获取剩余时间 */
uint32_t wdt_get_remain(void);

/* 简化接口 */
INLINE int wdt_enable(uint32_t timeout_ms) {
    wdt_cfg_t cfg = {timeout_ms, false};
    return wdt_init(&cfg);
}

INLINE void wdt_refresh(void) {
    wdt_feed();
}

#endif /* WDT_H */
