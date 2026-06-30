/**
 * @file mqtt.h
 * @brief MQTT 客户端
 */

#ifndef MQTT_H
#define MQTT_H

#include "common.h"

/* MQTT QoS */
typedef enum {
    MQTT_QOS0 = 0,  /* 最多一次 */
    MQTT_QOS1 = 1,  /* 至少一次 */
    MQTT_QOS2 = 2,  /* 恰好一次 */
} mqtt_qos_t;

/* MQTT 连接状态 */
typedef enum {
    MQTT_DISCONNECTED = 0,
    MQTT_CONNECTING,
    MQTT_CONNECTED,
} mqtt_state_t;

/* MQTT 回调 */
typedef void (*mqtt_msg_cb)(const char *topic, const uint8_t *payload, size_t len);
typedef void (*mqtt_connect_cb)(mqtt_state_t state);

/* MQTT 配置 */
typedef struct {
    const char *host;
    uint16_t port;
    const char *client_id;
    const char *username;
    const char *password;
    const char *will_topic;
    const char *will_msg;
    uint16_t keepalive;
    bool clean_session;
} mqtt_cfg_t;

/* MQTT 客户端结构 */
typedef struct {
    mqtt_cfg_t cfg;
    mqtt_state_t state;
    mqtt_msg_cb on_message;
    mqtt_connect_cb on_connect;
    int sock;
    uint16_t msg_id;
} mqtt_client_t;

/* 初始化 */
int mqtt_init(mqtt_client_t *c, const mqtt_cfg_t *cfg);

/* 连接 */
int mqtt_connect(mqtt_client_t *c);

/* 断开 */
int mqtt_disconnect(mqtt_client_t *c);

/* 发布 */
int mqtt_publish(mqtt_client_t *c, const char *topic, const void *payload, size_t len, mqtt_qos_t qos, bool retain);

/* 订阅 */
int mqtt_subscribe(mqtt_client_t *c, const char *topic, mqtt_qos_t qos);

/* 取消订阅 */
int mqtt_unsubscribe(mqtt_client_t *c, const char *topic);

/* 设置回调 */
void mqtt_set_msg_cb(mqtt_client_t *c, mqtt_msg_cb cb);
void mqtt_set_connect_cb(mqtt_client_t *c, mqtt_connect_cb cb);

/* 处理接收（主循环调用） */
int mqtt_yield(mqtt_client_t *c, uint32_t timeout_ms);

/* 简化发布 */
INLINE int mqtt_pub(mqtt_client_t *c, const char *topic, const char *msg) {
    return mqtt_publish(c, topic, msg, strlen(msg), MQTT_QOS0, false);
}

/* 发布整数 */
int mqtt_pub_int(mqtt_client_t *c, const char *topic, int val);

/* 发布浮点数 */
int mqtt_pub_float(mqtt_client_t *c, const char *topic, float val, uint8_t decimals);

#endif /* MQTT_H */
