/**
 * @file main.c
 * @brief 嵌入式通用代码库示例
 */

#include "common.h"
#include "hal/gpio.h"
#include "hal/uart.h"
#include "hal/timer.h"
#include "data_structure/ringbuf.h"
#include "data_structure/queue.h"
#include "data_structure/crc.h"
#include "driver/dht.h"
#include "driver/bmp280.h"
#include "driver/mpu6050.h"
#include "driver/hcsr04.h"
#include "protocol/mqtt.h"
#include "protocol/http.h"
#include "system/wdt.h"
#include "security/aes.h"
#include "security/sha.h"
#include "security/base64.h"
#include "utility/log.h"
#include "utility/math.h"
#include "utility/str.h"

/* 定义 LED 引脚 */
#define LED_PIN     13
#define BUTTON_PIN  0

/* 定义串口 */
#define UART_DEBUG  0

/* 环形缓冲区 */
RINGBUF_DEF(uart_rx_buf, 256);

/* MQTT 客户端 */
static mqtt_client_t mqtt;

/* DHT 传感器 */
static dht_t dht;

/* 串口接收回调 */
static void uart_rx_callback(uint8_t data) {
    ringbuf_put(&uart_rx_buf, data);
}

/* MQTT 消息回调 */
static void mqtt_message_cb(const char *topic, const uint8_t *payload, size_t len) {
    char msg[len + 1];
    memcpy(msg, payload, len);
    msg[len] = '\0';
    LOG_I("MQTT", "Recv [%s]: %s", topic, msg);
}

/* MQTT 连接回调 */
static void mqtt_connect_cb(mqtt_state_t state) {
    if (state == MQTT_CONNECTED) {
        LOG_I("MQTT", "Connected");
        mqtt_subscribe(&mqtt, "device/cmd", MQTT_QOS1);
    } else {
        LOG_W("MQTT", "Disconnected");
    }
}

/* 按键中断 */
static volatile bool button_pressed = false;
static void button_isr(void *arg) {
    button_pressed = true;
}

/* 定时器回调 */
static void timer_callback(void *arg) {
    static uint8_t count = 0;
    gpio_toggle(LED_PIN);
    count++;
    if (count >= 10) {
        count = 0;
        LOG_D("TIMER", "Heartbeat");
    }
}

/* 初始化硬件 */
static void hardware_init(void) {
    // 日志初始化
    log_init(LOG_DEBUG);
    LOG_I("MAIN", "System starting...");

    // GPIO 初始化
    gpio_init(LED_PIN, GPIO_OUTPUT);
    gpio_init(BUTTON_PIN, GPIO_INPUT_PU);
    gpio_write(LED_PIN, GPIO_HIGH);

    // UART 初始化
    uart_init(UART_DEBUG, 115200);
    uart_set_rx_cb(UART_DEBUG, uart_rx_callback);
    LOG_I("MAIN", "UART initialized");

    // 定时器初始化
    timer_init(0, 1000000, timer_callback);  // 1秒
    timer_start(0);

    // 看门狗初始化
    wdt_enable(5000);  // 5秒超时
    LOG_I("MAIN", "Watchdog enabled");

    // DHT 传感器初始化
    dht_init(&dht, 2, DHT22);
    LOG_I("MAIN", "DHT22 initialized");
}

/* 网络初始化 */
static void network_init(void) {
    // WiFi 连接
    LOG_I("MAIN", "Connecting to WiFi...");
    // wifi_join("SSID", "PASSWORD");

    // MQTT 初始化
    mqtt_cfg_t mqtt_cfg = {
        .host = "broker.emqx.io",
        .port = 1883,
        .client_id = "embedded_device_001",
        .keepalive = 60,
        .clean_session = true,
    };
    mqtt_init(&mqtt, &mqtt_cfg);
    mqtt_set_msg_cb(&mqtt, mqtt_message_cb);
    mqtt_set_connect_cb(&mqtt, mqtt_connect_cb);
    mqtt_connect(&mqtt);
    LOG_I("MAIN", "MQTT initialized");
}

/* 传感器数据采集 */
static void sensor_task(void) {
    static uint32_t last_time = 0;
    if (!is_timeout(last_time, 5000)) return;  // 5秒间隔
    last_time = get_tick_ms();

    // 读取 DHT
    dht_data_t dht_data;
    if (dht_read(&dht, &dht_data) == OK) {
        LOG_I("SENSOR", "Temp: %.1f°C, Humi: %.1f%%", 
              dht_data.temperature, dht_data.humidity);

        // 发布到 MQTT
        char payload[64];
        str_format(payload, sizeof(payload), 
                   "{\"temp\":%.1f,\"humi\":%.1f}", 
                   dht_data.temperature, dht_data.humidity);
        mqtt_pub(&mqtt, "device/sensor", payload);
    }

    wdt_feed();  // 喂狗
}

/* 数据处理示例 */
static void data_process_demo(void) {
    // CRC 校验示例
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint16_t crc = crc16(data, sizeof(data));
    LOG_I("CRC", "CRC16: 0x%04X", crc);

    // Base64 编码示例
    const char *str = "Hello, Embedded!";
    char encoded[64];
    base64_encode((const uint8_t *)str, strlen(str), encoded, sizeof(encoded));
    LOG_I("BASE64", "Encoded: %s", encoded);

    // SHA256 示例
    uint8_t hash[32];
    sha256((const uint8_t *)str, strlen(str), hash);
    char hex[65];
    sha256_to_hex(hash, hex, sizeof(hex));
    LOG_I("SHA256", "Hash: %s", hex);

    // JSON 解析示例
    const char *json_str = "{\"name\":\"device\",\"value\":123}";
    json_node_t *json = json_parse(json_str, strlen(json_str));
    if (json) {
        const char *name = json_get_string(json, "name", "");
        int value = json_get_int(json, "value", 0);
        LOG_I("JSON", "Name: %s, Value: %d", name, value);
        json_free(json);
    }

    // PID 控制示例
    static pid_t pid;
    pid_init(&pid, 1.0f, 0.1f, 0.05f);
    pid_set_limits(&pid, -100, 100);
    float output = pid_update(&pid, 25.0f, 0.1f);
    LOG_I("PID", "Output: %.2f", output);

    // 移动平均示例
    static float avg_buf[10];
    static moving_avg_t ma = {avg_buf, 10, 0, 0};
    for (int i = 0; i < 15; i++) {
        float val = random_float(20.0f, 30.0f);
        float avg = moving_avg_update(&ma, val);
        if (i >= 9) {
            LOG_I("AVG", "Value: %.2f, Average: %.2f", val, avg);
        }
    }
}

/* 主函数 */
int main(void) {
    // 硬件初始化
    hardware_init();

    // 网络初始化
    network_init();

    // 数据处理演示
    data_process_demo();

    LOG_I("MAIN", "System ready!");

    // 主循环
    while (1) {
        // 处理 MQTT
        mqtt_yield(&mqtt, 100);

        // 传感器采集
        sensor_task();

        // 处理串口数据
        uint8_t ch;
        while (ringbuf_get(&uart_rx_buf, &ch) == OK) {
            // 回显
            uart_send(UART_DEBUG, &ch, 1);
        }

        // 按键检测
        if (button_pressed) {
            button_pressed = false;
            LOG_I("MAIN", "Button pressed!");
            gpio_toggle(LED_PIN);
        }

        // 喂狗
        wdt_feed();
    }

    return 0;
}
