/**
 * @file ota.h
 * @brief OTA 升级
 */

#ifndef OTA_H
#define OTA_H

#include "common.h"

/* OTA 状态 */
typedef enum {
    OTA_IDLE = 0,
    OTA_DOWNLOADING,
    OTA_VERIFYING,
    OTA_INSTALLING,
    OTA_DONE,
    OTA_ERROR,
} ota_state_t;

/* OTA 回调 */
typedef void (*ota_progress_cb)(ota_state_t state, uint8_t percent);
typedef void (*ota_error_cb)(int err);

/* OTA 配置 */
typedef struct {
    const char *url;
    const char *md5;
    const char *version;
    uint32_t size;
} ota_info_t;

/* 初始化 */
int ota_init(void);

/* 检查更新 */
int ota_check(const char *url, ota_info_t *info);

/* 开始升级 */
int ota_start(const ota_info_t *info);

/* 获取状态 */
ota_state_t ota_get_state(void);

/* 获取进度 */
uint8_t ota_get_progress(void);

/* 设置回调 */
void ota_set_progress_cb(ota_progress_cb cb);
void ota_set_error_cb(ota_error_cb cb);

/* 中止升级 */
int ota_abort(void);

/* 验证固件 */
int ota_verify(const char *md5);

/* 应用升级（重启） */
void ota_apply(void);

/* 回滚 */
int ota_rollback(void);

/* 简化接口 */
INLINE int ota_upgrade(const char *url) {
    ota_info_t info;
    if (ota_check(url, &info) != OK) return ERR;
    return ota_start(&info);
}

#endif /* OTA_H */
