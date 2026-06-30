/**
 * @file bmp280.h
 * @brief BMP280/BME280 气压传感器驱动
 */

#ifndef BMP280_H
#define BMP280_H

#include "common.h"

/* BMP280 设备 */
typedef struct {
    uint8_t i2c_id;
    uint8_t addr;       /* 0x76 或 0x77 */
    int32_t t_fine;
} bmp280_t;

/* BMP280 数据 */
typedef struct {
    float temperature;
    float pressure;
    float altitude;
    float humidity;     /* 仅 BME280 */
} bmp280_data_t;

/* 初始化 */
int bmp280_init(bmp280_t *dev, uint8_t i2c_id, uint8_t addr);

/* 读取数据 */
int bmp280_read(bmp280_t *dev, bmp280_data_t *data);

/* 读取温度 */
float bmp280_read_temp(bmp280_t *dev);

/* 读取气压 */
float bmp280_read_pressure(bmp280_t *dev);

/* 读取海拔 */
float bmp280_read_altitude(bmp280_t *dev);

/* 气压转海拔 */
INLINE float pressure_to_altitude(float pressure_hpa) {
    return 44330.0f * (1.0f - powf(pressure_hpa / 1013.25f, 0.1903f));
}

#endif /* BMP280_H */
