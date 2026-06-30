# API 参考文档

## 目录

- [通用定义](#通用定义)
- [硬件抽象层 (HAL)](#硬件抽象层-hal)
- [数据结构](#数据结构)
- [通信协议](#通信协议)
- [设备驱动](#设备驱动)
- [系统功能](#系统功能)
- [安全加密](#安全加密)
- [工具函数](#工具函数)

## 通用定义

### 错误码

```c
typedef enum {
    OK = 0,           // 成功
    ERR = -1,         // 通用错误
    ERR_PARAM = -2,   // 参数错误
    ERR_MEM = -3,     // 内存错误
    ERR_TIMEOUT = -4, // 超时
    ERR_BUSY = -5,    // 忙
    ERR_NOT_FOUND = -6, // 未找到
} err_t;
```

### 位操作宏

```c
BIT(n)              // 位掩码
SET_BIT(x, n)       // 置位
CLR_BIT(x, n)       // 清位
TOG_BIT(x, n)       // 翻转位
GET_BIT(x, n)       // 获取位
```

### 数组大小

```c
ARRAY_SIZE(a)       // 获取数组大小
```

### 最小/最大

```c
MIN(a, b)           // 最小值
MAX(a, b)           // 最大值
CLAMP(x, lo, hi)    // 限幅
```

### 对齐

```c
ALIGN_UP(x, a)      // 向上对齐
ALIGN_DOWN(x, a)    // 向下对齐
IS_ALIGNED(x, a)    // 检查对齐
```

### 字节序转换

```c
SWAP16(x)           // 16位字节序转换
SWAP32(x)           // 32位字节序转换
MK_U16(h, l)        // 组合16位
MK_U32(a,b,c,d)     // 组合32位
HI_BYTE(x)          // 高字节
LO_BYTE(x)          // 低字节
```

### 容器宏

```c
container_of(ptr, type, member)  // 获取容器
```

## 硬件抽象层 (HAL)

### GPIO

```c
// 初始化
int gpio_init(uint16_t pin, gpio_mode_t mode);

// 写入
int gpio_write(uint16_t pin, gpio_level_t level);

// 读取
int gpio_read(uint16_t pin);

// 翻转
int gpio_toggle(uint16_t pin);
```

### UART

```c
// 初始化
int uart_init(uint8_t id, uint32_t baud);

// 发送
int uart_send(uint8_t id, const void *data, size_t len);

// 接收
int uart_recv(uint8_t id, void *buf, size_t len, uint32_t timeout);

// 发送字符串
int uart_puts(uint8_t id, const char *str);
```

### SPI

```c
// 初始化
int spi_init(uint8_t id, uint32_t freq);

// 写入
int spi_write(uint8_t id, const uint8_t *data, size_t len);

// 读取
int spi_read(uint8_t id, uint8_t *data, size_t len);

// 传输
int spi_xfer(uint8_t id, const uint8_t *tx, uint8_t *rx, size_t len);
```

### I2C

```c
// 初始化
int i2c_init(uint8_t id, uint32_t freq);

// 写寄存器
int i2c_write_reg(uint8_t id, uint8_t addr, uint8_t reg, uint8_t val);

// 读寄存器
int i2c_read_reg(uint8_t id, uint8_t addr, uint8_t reg, uint8_t *val);

// 检测设备
bool i2c_detect(uint8_t id, uint8_t addr);
```

### ADC

```c
// 初始化
int adc_init(uint8_t id, uint8_t bits);

// 读取
int adc_read(uint8_t id, uint8_t ch);

// 读取电压 (mV)
int adc_read_mv(uint8_t id, uint8_t ch);

// 多次采样平均
int adc_read_avg(uint8_t id, uint8_t ch, uint8_t samples);
```

### PWM

```c
// 初始化
int pwm_init(uint8_t id, uint8_t ch, uint32_t freq, uint8_t duty);

// 设置占空比
int pwm_set_duty(uint8_t id, uint8_t ch, uint8_t duty);

// 启动
int pwm_start(uint8_t id, uint8_t ch);

// 停止
int pwm_stop(uint8_t id, uint8_t ch);
```

### 定时器

```c
// 初始化（周期）
int timer_init(uint8_t id, uint32_t us, timer_cb cb);

// 初始化（单次）
int timer_once(uint8_t id, uint32_t us, timer_cb cb);

// 启动
int timer_start(uint8_t id);

// 停止
int timer_stop(uint8_t id);

// 延时
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

// 获取滴答
uint32_t get_tick(void);
uint32_t get_tick_ms(void);

// 超时检测
bool is_timeout(uint32_t start, uint32_t timeout_ms);
```

## 数据结构

### 环形缓冲区

```c
// 静态创建
RINGBUF_DEF(name, size);

// 初始化
int ringbuf_init(ringbuf_t *rb, uint8_t *buf, size_t size);

// 写入一字节
int ringbuf_put(ringbuf_t *rb, uint8_t data);

// 读取一字节
int ringbuf_get(ringbuf_t *rb, uint8_t *data);

// 写入多字节
size_t ringbuf_write(ringbuf_t *rb, const uint8_t *data, size_t len);

// 读取多字节
size_t ringbuf_read(ringbuf_t *rb, uint8_t *buf, size_t len);

// 状态查询
size_t ringbuf_count(const ringbuf_t *rb);
size_t ringbuf_space(const ringbuf_t *rb);
bool ringbuf_is_full(const ringbuf_t *rb);
bool ringbuf_is_empty(const ringbuf_t *rb);

// 清空
void ringbuf_flush(ringbuf_t *rb);
```

### 队列

```c
// 初始化
int queue_init(queue_t *q, uint8_t *buf, size_t size);

// 入队
int queue_push(queue_t *q, uint8_t data);

// 出队
int queue_pop(queue_t *q, uint8_t *data);

// 批量入队
size_t queue_push_batch(queue_t *q, const uint8_t *data, size_t len);

// 批量出队
size_t queue_pop_batch(queue_t *q, uint8_t *buf, size_t len);

// 设置锁（多线程）
void queue_set_lock(queue_t *q, void (*lock)(void), void (*unlock)(void));
```

### 链表

```c
// 静态创建
LIST_HEAD(name);

// 初始化
void list_init(list_node_t *node);

// 插入（尾部）
void list_add(list_node_t *head, list_node_t *node);

// 插入（头部）
void list_add_head(list_node_t *head, list_node_t *node);

// 删除
void list_del(list_node_t *node);

// 遍历
list_for_each(pos, head) {
    // 处理 pos
}

// 获取容器
list_entry(ptr, type, member)

// 获取长度
size_t list_len(const list_node_t *head);
```

### JSON

```c
// 解析
json_node_t *json_parse(const char *str, size_t len);

// 释放
void json_free(json_node_t *root);

// 查找
json_node_t *json_find(json_node_t *root, const char *key);

// 获取值
const char *json_get_string(json_node_t *root, const char *key, const char *def);
int json_get_int(json_node_t *root, const char *key, int def);
bool json_get_bool(json_node_t *root, const char *key, bool def);
float json_get_float(json_node_t *root, const char *key, float def);

// 数组操作
json_node_t *json_array_get(json_node_t *arr, int index);
int json_array_size(json_node_t *arr);

// 生成
int json_serialize(json_node_t *root, char *buf, size_t size);
```

### CRC

```c
// CRC8
uint8_t crc8(const uint8_t *data, size_t len);
uint8_t crc8_maxim(const uint8_t *data, size_t len);

// CRC16
uint16_t crc16(const uint8_t *data, size_t len);
uint16_t crc16_ccitt(const uint8_t *data, size_t len);
uint16_t crc16_modbus(const uint8_t *data, size_t len);

// CRC32
uint32_t crc32(const uint8_t *data, size_t len);

// 校验和
uint8_t checksum8(const uint8_t *data, size_t len);
uint16_t checksum16(const uint8_t *data, size_t len);
uint32_t checksum32(const uint8_t *data, size_t len);

// 异或校验
uint8_t xor_check(const uint8_t *data, size_t len);

// LRC
uint8_t lrc(const uint8_t *data, size_t len);
```

## 通信协议

### MQTT

```c
// 初始化
int mqtt_init(mqtt_client_t *c, const mqtt_cfg_t *cfg);

// 连接
int mqtt_connect(mqtt_client_t *c);

// 断开
int mqtt_disconnect(mqtt_client_t *c);

// 发布
int mqtt_publish(mqtt_client_t *c, const char *topic, const void *payload, size_t len, mqtt_qos_t qos, bool retain);

// 订阅
int mqtt_subscribe(mqtt_client_t *c, const char *topic, mqtt_qos_t qos);

// 简化发布
int mqtt_pub(mqtt_client_t *c, const char *topic, const char *msg);

// 设置回调
void mqtt_set_msg_cb(mqtt_client_t *c, mqtt_msg_cb cb);
void mqtt_set_connect_cb(mqtt_client_t *c, mqtt_connect_cb cb);

// 处理接收
int mqtt_yield(mqtt_client_t *c, uint32_t timeout_ms);
```

### HTTP

```c
// 同步请求
int http_request(const http_req_t *req, http_resp_t *resp);

// 简化接口
int http_get(const char *url, http_resp_t *resp);
int http_post(const char *url, const char *data, http_resp_t *resp);

// 释放响应
void http_resp_free(http_resp_t *resp);

// URL 编码/解码
size_t url_encode(char *dst, size_t dst_len, const char *src);
size_t url_decode(char *dst, size_t dst_len, const char *src);
```

### WiFi

```c
// 初始化
int wifi_init(const wifi_cfg_t *cfg);

// 连接
int wifi_connect(const char *ssid, const char *password);

// 断开
int wifi_disconnect(void);

// 扫描
int wifi_scan(wifi_scan_cb cb);

// 获取状态
wifi_state_t wifi_get_state(void);

// 获取 IP
int wifi_get_ip(char *ip, size_t len);

// 简化连接
int wifi_join(const char *ssid, const char *pass);
```

## 设备驱动

### DHT11/DHT22

```c
// 初始化
int dht_init(dht_t *dht, uint8_t pin, dht_type_t type);

// 读取
int dht_read(dht_t *dht, dht_data_t *data);

// 简化读取
float dht_read_temp(dht_t *dht);
float dht_read_humi(dht_t *dht);
```

### BMP280/BME280

```c
// 初始化
int bmp280_init(bmp280_t *dev, uint8_t i2c_id, uint8_t addr);

// 读取
int bmp280_read(bmp280_t *dev, bmp280_data_t *data);

// 简化读取
float bmp280_read_temp(bmp280_t *dev);
float bmp280_read_pressure(bmp280_t *dev);
float bmp280_read_altitude(bmp280_t *dev);

// 气压转海拔
float pressure_to_altitude(float pressure_hpa);
```

### MPU6050

```c
// 初始化
int mpu6050_init(mpu6050_t *dev, uint8_t i2c_id, uint8_t addr);

// 读取
int mpu6050_read(mpu6050_t *dev, mpu6050_data_t *data);

// 设置量程
int mpu6050_set_accel_range(mpu6050_t *dev, uint8_t range);
int mpu6050_set_gyro_range(mpu6050_t *dev, uint8_t range);

// 姿态角计算
void mpu6050_calc_angle(const mpu6050_data_t *data, mpu6050_angle_t *angle, float dt);
```

### HC-SR04

```c
// 初始化
int hcsr04_init(hcsr04_t *dev, uint8_t trig, uint8_t echo);

// 读取距离
float hcsr04_read_cm(hcsr04_t *dev);
float hcsr04_read_inch(hcsr04_t *dev);

// 多次采样平均
float hcsr04_read_avg(hcsr04_t *dev, uint8_t samples);
```

### SSD1306 OLED

```c
// 初始化
int ssd1306_init(ssd1306_t *dev, uint8_t i2c_id, uint8_t addr);

// 清屏
void ssd1306_clear(ssd1306_t *dev);

// 刷新
void ssd1306_display(ssd1306_t *dev);

// 像素操作
void ssd1306_set_pixel(ssd1306_t *dev, int16_t x, int16_t y, uint8_t color);

// 画线
void ssd1306_draw_line(ssd1306_t *dev, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);

// 画矩形
void ssd1306_draw_rect(ssd1306_t *dev, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
void ssd1306_fill_rect(ssd1306_t *dev, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);

// 画圆
void ssd1306_draw_circle(ssd1306_t *dev, int16_t x0, int16_t y0, int16_t r, uint8_t color);
void ssd1306_fill_circle(ssd1306_t *dev, int16_t x0, int16_t y0, int16_t r, uint8_t color);

// 显示字符/字符串
void ssd1306_draw_char(ssd1306_t *dev, int16_t x, int16_t y, char c, uint8_t color, uint8_t size);
void ssd1306_draw_string(ssd1306_t *dev, int16_t x, int16_t y, const char *str, uint8_t color, uint8_t size);

// 亮度
void ssd1306_set_brightness(ssd1306_t *dev, uint8_t brightness);
```

## 系统功能

### 看门狗

```c
// 启用
int wdt_enable(uint32_t timeout_ms);

// 喂狗
void wdt_feed(void);
void wdt_refresh(void);
```

### 低功耗

```c
// 初始化
int lp_init(void);

// 进入低功耗
int lp_enter(lp_mode_t mode);

// 设置唤醒源
int lp_set_wake_src(uint32_t src);

// 配置 RTC 唤醒
int lp_config_wake_rtc(uint32_t seconds);

// 简化接口
void lp_sleep(uint32_t seconds);
void lp_deep_sleep(void);
```

### OTA 升级

```c
// 初始化
int ota_init(void);

// 检查更新
int ota_check(const char *url, ota_info_t *info);

// 开始升级
int ota_start(const ota_info_t *info);

// 获取状态
ota_state_t ota_get_state(void);

// 获取进度
uint8_t ota_get_progress(void);

// 应用升级
void ota_apply(void);

// 简化接口
int ota_upgrade(const char *url);
```

### 内存管理

```c
// 初始化
int mem_init(void *pool, size_t size);

// 分配
void *mem_alloc(size_t size);

// 释放
void mem_free(void *ptr);

// 分配并清零
void *mem_calloc(size_t count, size_t size);

// 获取信息
void mem_get_info(mem_info_t *info);

// 简化宏
MALLOC(size)
FREE(ptr)
CALLOC(n, size)
```

## 安全加密

### AES

```c
// 初始化
int aes_init(aes_ctx_t *ctx, const uint8_t *key, aes_key_len_t len, aes_mode_t mode, const uint8_t *iv);

// 加密
int aes_encrypt(aes_ctx_t *ctx, const uint8_t *in, uint8_t *out, size_t len);

// 解密
int aes_decrypt(aes_ctx_t *ctx, const uint8_t *in, uint8_t *out, size_t len);

// 简化接口
int aes_encrypt_ecb(const uint8_t *key, const uint8_t *in, uint8_t *out, size_t len);
int aes_decrypt_ecb(const uint8_t *key, const uint8_t *in, uint8_t *out, size_t len);
```

### SHA

```c
// SHA1
void sha1(const uint8_t *data, size_t len, uint8_t hash[20]);

// SHA256
void sha256(const uint8_t *data, size_t len, uint8_t hash[32]);

// HMAC-SHA256
void hmac_sha256(const uint8_t *key, size_t key_len,
                 const uint8_t *data, size_t data_len,
                 uint8_t hash[32]);

// MD5
void md5(const uint8_t *data, size_t len, uint8_t hash[16]);

// 哈希转字符串
void sha256_to_hex(const uint8_t *hash, char *hex, size_t len);
```

### Base64

```c
// 编码
size_t base64_encode(const uint8_t *in, size_t in_len, char *out, size_t out_len);

// 解码
size_t base64_decode(const char *in, uint8_t *out, size_t out_len);

// 计算长度
size_t base64_encode_len(size_t in_len);
size_t base64_decode_len(const char *in);

// URL 安全
size_t base64url_encode(const uint8_t *in, size_t in_len, char *out, size_t out_len);
size_t base64url_decode(const char *in, uint8_t *out, size_t out_len);
```

## 工具函数

### 日志

```c
// 初始化
void log_init(log_level_t level);

// 设置级别
void log_set_level(log_level_t level);

// 设置输出
void log_set_output(log_output_cb cb);

// 日志宏
LOG_E(tag, fmt, ...)  // 错误
LOG_W(tag, fmt, ...)  // 警告
LOG_I(tag, fmt, ...)  // 信息
LOG_D(tag, fmt, ...)  // 调试
LOG_V(tag, fmt, ...)  // 详细

// 十六进制转储
void log_hex_dump(const char *tag, const void *data, size_t len);

// 性能测量
PERF_START(name)
PERF_END(name)
```

### 数学函数

```c
// 限幅
int clamp(int val, int min, int max);
float fclamp(float val, float min, float max);

// 线性映射
int map(int val, int in_min, int in_max, int out_min, int out_max);
float fmap(float val, float in_min, float in_max, float out_min, float out_max);

// 平滑滤波
float smooth_update(smooth_t *s, float new_val);

// 移动平均
float moving_avg_update(moving_avg_t *ma, float val);
float moving_avg_get(const moving_avg_t *ma);

// PID 控制器
void pid_init(pid_t *pid, float kp, float ki, float kd);
float pid_update(pid_t *pid, float measurement, float dt);
void pid_set_limits(pid_t *pid, float min, float max);
void pid_reset(pid_t *pid);

// 角度归一化
float normalize_angle(float angle);
```

### 字符串函数

```c
// 安全拷贝
size_t str_copy(char *dst, size_t dst_len, const char *src);

// 拼接
size_t str_append(char *dst, size_t dst_len, const char *src);

// 格式化
size_t str_format(char *buf, size_t len, const char *fmt, ...);

// 查找
const char *str_find(const char *str, const char *sub);

// 前缀/后缀
bool str_startswith(const char *str, const char *prefix);
bool str_endswith(const char *str, const char *suffix);

// 去除空白
char *str_trim(char *str);

// 分割
size_t str_split(char *str, const char *delim, char **tokens, size_t max_tokens);

// 转换
int str_to_int(const char *str, int def);
float str_to_float(const char *str, float def);
bool str_to_bool(const char *str);

// 整数/浮点转字符串
char *int_to_str(int val, char *buf, size_t len);
char *float_to_str(float val, uint8_t decimals, char *buf, size_t len);

// 十六进制
char *hex_to_str(const uint8_t *hex, size_t len, char *str, size_t str_len);
size_t str_to_hex(const char *str, uint8_t *hex, size_t hex_len);

// 大小写
void str_tolower(char *str);
void str_toupper(char *str);
```

### 通用工具

```c
// 随机数
void random_seed(uint32_t seed);
uint32_t random_uint32(void);
int random_range(int min, int max);
float random_float(float min, float max);

// 时间戳
uint32_t get_timestamp(void);
void timestamp_to_datetime(uint32_t timestamp, datetime_t *dt);
size_t datetime_to_str(const datetime_t *dt, char *buf, size_t len);

// 位操作
uint32_t popcount(uint32_t x);
uint32_t clz(uint32_t x);
uint32_t ctz(uint32_t x);
uint32_t bit_reverse(uint32_t x);
uint32_t rotl32(uint32_t x, uint32_t n);
uint32_t rotr32(uint32_t x, uint32_t n);

// BCD
uint8_t int_to_bcd(uint8_t val);
uint8_t bcd_to_int(uint8_t bcd);
```
