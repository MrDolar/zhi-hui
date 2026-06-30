/**
 * @file dht.h
 * @brief DHT11/DHT22 温湿度传感器驱动
 */

#ifndef DHT_H
#define DHT_H

#include "common.h"

/* DHT 类型 */
typedef enum {
    DHT11 = 0,
    DHT22,
} dht_type_t;

/* DHT 数据 */
typedef struct {
    float temperature;
    float humidity;
} dht_data_t;

/* DHT 设备 */
typedef struct {
    uint8_t pin;
    dht_type_t type;
} dht_t;

/* 初始化 */
int dht_init(dht_t *dht, uint8_t pin, dht_type_t type);

/* 读取数据 */
int dht_read(dht_t *dht, dht_data_t *data);

/* 读取温度 */
INLINE float dht_read_temp(dht_t *dht) {
    dht_data_t data;
    return dht_read(dht, &data) == OK ? data.temperature : -999.0f;
}

/* 读取湿度 */
INLINE float dht_read_humi(dht_t *dht) {
    dht_data_t data;
    return dht_read(dht, &data) == OK ? data.humidity : -999.0f;
}

#endif /* DHT_H */
