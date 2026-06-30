/**
 * @file mpu6050.h
 * @brief MPU6050 六轴传感器驱动
 */

#ifndef MPU6050_H
#define MPU6050_H

#include "common.h"

/* MPU6050 数据 */
typedef struct {
    int16_t ax, ay, az;  /* 加速度 */
    int16_t gx, gy, gz;  /* 陀螺仪 */
    float temp;           /* 温度 */
} mpu6050_raw_t;

typedef struct {
    float ax, ay, az;    /* 加速度 g */
    float gx, gy, gz;    /* 陀螺仪 °/s */
    float temp;           /* 温度 ℃ */
} mpu6050_data_t;

/* MPU6050 设备 */
typedef struct {
    uint8_t i2c_id;
    uint8_t addr;        /* 0x68 或 0x69 */
} mpu6050_t;

/* 初始化 */
int mpu6050_init(mpu6050_t *dev, uint8_t i2c_id, uint8_t addr);

/* 读取原始数据 */
int mpu6050_read_raw(mpu6050_t *dev, mpu6050_raw_t *raw);

/* 读取转换后数据 */
int mpu6050_read(mpu6050_t *dev, mpu6050_data_t *data);

/* 设置量程 */
int mpu6050_set_accel_range(mpu6050_t *dev, uint8_t range);  /* 0=2g,1=4g,2=8g,3=16g */
int mpu6050_set_gyro_range(mpu6050_t *dev, uint8_t range);   /* 0=250,1=500,2=1000,3=2000°/s */

/* 姿态角计算 */
typedef struct {
    float roll;
    float pitch;
    float yaw;
} mpu6050_angle_t;

void mpu6050_calc_angle(const mpu6050_data_t *data, mpu6050_angle_t *angle, float dt);

#endif /* MPU6050_H */
