/**
 * @file sensor_example.c
 * @brief 传感器使用示例
 */

#include "common.h"
#include "hal/i2c.h"
#include "driver/dht.h"
#include "driver/bmp280.h"
#include "driver/mpu6050.h"
#include "driver/hcsr04.h"
#include "utility/log.h"
#include "utility/math.h"

/* DHT 温湿度传感器 */
void dht_example(void) {
    LOG_I("SENSOR", "=== DHT Sensor Example ===");

    dht_t dht;
    dht_init(&dht, 2, DHT22);

    for (int i = 0; i < 5; i++) {
        dht_data_t data;
        if (dht_read(&dht, &data) == OK) {
            LOG_I("DHT", "Temp: %.1f°C, Humi: %.1f%%", 
                  data.temperature, data.humidity);
        } else {
            LOG_E("DHT", "Read failed");
        }
        delay_ms(2000);
    }
}

/* BMP280 气压传感器 */
void bmp280_example(void) {
    LOG_I("SENSOR", "=== BMP280 Example ===");

    bmp280_t bmp;
    bmp280_init(&bmp, 0, 0x76);

    for (int i = 0; i < 5; i++) {
        bmp280_data_t data;
        if (bmp280_read(&bmp, &data) == OK) {
            LOG_I("BMP280", "Temp: %.2f°C, Pres: %.2f hPa, Alt: %.2f m",
                  data.temperature, data.pressure, data.altitude);
        }
        delay_ms(1000);
    }
}

/* MPU6050 六轴传感器 */
void mpu6050_example(void) {
    LOG_I("SENSOR", "=== MPU6050 Example ===");

    mpu6050_t mpu;
    mpu6050_init(&mpu, 0, 0x68);

    for (int i = 0; i < 5; i++) {
        mpu6050_data_t data;
        if (mpu6050_read(&mpu, &data) == OK) {
            LOG_I("MPU6050", "Accel: %.2f,%.2f,%.2f g, Gyro: %.2f,%.2f,%.2f °/s",
                  data.ax, data.ay, data.az,
                  data.gx, data.gy, data.gz);
        }
        delay_ms(100);
    }

    // 姿态角计算
    mpu6050_angle_t angle;
    mpu6050_data_t data;
    mpu6050_read(&mpu, &data);
    mpu6050_calc_angle(&data, &angle, 0.01f);
    LOG_I("MPU6050", "Roll: %.2f°, Pitch: %.2f°", angle.roll, angle.pitch);
}

/* HC-SR04 超声波 */
void hcsr04_example(void) {
    LOG_I("SENSOR", "=== HC-SR04 Example ===");

    hcsr04_t hcsr;
    hcsr04_init(&hcsr, 5, 18);  // Trig=5, Echo=18

    for (int i = 0; i < 10; i++) {
        float distance = hcsr04_read_avg(&hcsr, 5);
        if (distance > 0) {
            LOG_I("HCSR04", "Distance: %.2f cm", distance);
        } else {
            LOG_W("HCSR04", "Out of range");
        }
        delay_ms(500);
    }
}

/* 综合传感器示例 */
void sensor_demo(void) {
    LOG_I("SENSOR", "=== Sensor Demo ===");

    // 初始化 I2C
    i2c_init(0, 400000);  // 400kHz

    // 运行各传感器示例
    dht_example();
    bmp280_example();
    mpu6050_example();
    hcsr04_example();
}
