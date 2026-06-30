/**
 * @file wifi.h
 * @brief WiFi 管理
 */

#ifndef WIFI_H
#define WIFI_H

#include "common.h"

/* WiFi 状态 */
typedef enum {
    WIFI_IDLE = 0,
    WIFI_CONNECTING,
    WIFI_CONNECTED,
    WIFI_DISCONNECTED,
    WIFI_AP_MODE,
} wifi_state_t;

/* WiFi 模式 */
typedef enum {
    WIFI_MODE_STA = 0,
    WIFI_MODE_AP,
    WIFI_MODE_STA_AP,
} wifi_mode_t;

/* WiFi 配置 */
typedef struct {
    const char *ssid;
    const char *password;
    wifi_mode_t mode;
    uint8_t channel;
    bool hidden;
} wifi_cfg_t;

/* WiFi 扫描结果 */
typedef struct {
    char ssid[33];
    int8_t rssi;
    uint8_t channel;
    uint8_t bssid[6];
    bool secured;
} wifi_ap_t;

/* WiFi 回调 */
typedef void (*wifi_event_cb)(wifi_state_t state);
typedef void (*wifi_scan_cb)(const wifi_ap_t *ap, size_t count);

/* 初始化 */
int wifi_init(const wifi_cfg_t *cfg);

/* 连接 */
int wifi_connect(const char *ssid, const char *password);

/* 断开 */
int wifi_disconnect(void);

/* 启动 AP */
int wifi_ap_start(const char *ssid, const char *password, uint8_t channel);

/* 停止 AP */
int wifi_ap_stop(void);

/* 扫描 */
int wifi_scan(wifi_scan_cb cb);

/* 获取状态 */
wifi_state_t wifi_get_state(void);

/* 获取 IP */
int wifi_get_ip(char *ip, size_t len);

/* 获取 MAC */
int wifi_get_mac(char *mac, size_t len);

/* 获取 RSSI */
int wifi_get_rssi(void);

/* 设置回调 */
void wifi_set_event_cb(wifi_event_cb cb);

/* 重连 */
int wifi_reconnect(void);

/* 简化连接 */
INLINE int wifi_join(const char *ssid, const char *pass) {
    wifi_cfg_t cfg = {
        .ssid = ssid,
        .password = pass,
        .mode = WIFI_MODE_STA,
    };
    return wifi_init(&cfg);
}

#endif /* WIFI_H */
