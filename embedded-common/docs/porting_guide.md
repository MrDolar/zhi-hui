# 移植指南

本文档介绍如何将嵌入式通用代码库移植到不同平台。

## 目录结构

```
embedded-common/
├── include/          # 头文件（平台无关）
├── src/              # 源文件（平台无关）
├── port/             # 移植层（平台相关）
│   ├── stm32/
│   ├── esp32/
│   ├── arduino/
│   └── linux/
└── examples/         # 示例代码
```

## 移植步骤

### 1. 创建平台目录

```bash
mkdir -p port/your_platform
```

### 2. 实现 HAL 接口

#### GPIO 实现

```c
// port/your_platform/gpio_impl.c
#include "hal/gpio.h"

static int gpio_init_impl(uint16_t pin, gpio_mode_t mode) {
    // 你的平台 GPIO 初始化代码
    return OK;
}

static int gpio_write_impl(uint16_t pin, gpio_level_t level) {
    // 你的平台 GPIO 写入代码
    return OK;
}

static int gpio_read_impl(uint16_t pin) {
    // 你的平台 GPIO 读取代码
    return 0;
}

static int gpio_toggle_impl(uint16_t pin) {
    // 你的平台 GPIO 翻转代码
    return OK;
}

static int gpio_deinit_impl(uint16_t pin) {
    // 你的平台 GPIO 去初始化代码
    return OK;
}

// 注册操作接口
static const gpio_ops_t gpio_ops_impl = {
    .init = gpio_init_impl,
    .write = gpio_write_impl,
    .read = gpio_read_impl,
    .toggle = gpio_toggle_impl,
    .deinit = gpio_deinit_impl,
};

const gpio_ops_t *gpio_ops = &gpio_ops_impl;
```

#### UART 实现

```c
// port/your_platform/uart_impl.c
#include "hal/uart.h"

static int uart_init_impl(uint8_t id, const uart_cfg_t *cfg) {
    // 你的平台 UART 初始化代码
    return OK;
}

static int uart_write_impl(uint8_t id, const uint8_t *data, size_t len) {
    // 你的平台 UART 发送代码
    return len;
}

static int uart_read_impl(uint8_t id, uint8_t *buf, size_t len, uint32_t timeout) {
    // 你的平台 UART 接收代码
    return 0;
}

// 注册操作接口
static const uart_ops_t uart_ops_impl = {
    .init = uart_init_impl,
    .write = uart_write_impl,
    .read = uart_read_impl,
};

const uart_ops_t *uart_ops = &uart_ops_impl;
```

### 3. 实现系统函数

```c
// port/your_platform/system.c
#include "common.h"
#include "hal/timer.h"

// 系统滴答
static volatile uint32_t sys_tick = 0;

void SysTick_Handler(void) {
    sys_tick++;
}

uint32_t get_tick(void) {
    return sys_tick;
}

uint32_t get_tick_ms(void) {
    return sys_tick;
}

void delay_us(uint32_t us) {
    // 你的平台延时代码
}

void delay_ms(uint32_t ms) {
    uint32_t start = get_tick_ms();
    while (get_tick_ms() - start < ms);
}
```

### 4. 配置头文件

```c
// port/your_platform/config.h
#ifndef PLATFORM_CONFIG_H
#define PLATFORM_CONFIG_H

// 系统配置
#define SYSTEM_TICK_FREQ    1000    // 1kHz
#define SYSTEM_TICK_MS      1

// 内存配置
#define MEM_POOL_SIZE       (32 * 1024)

// 功能开关
#define ENABLE_LOG          1
#define ENABLE_ASSERT       1
#define ENABLE_WDT          1

#endif
```

### 5. 创建平台头文件

```c
// port/your_platform/platform.h
#ifndef PLATFORM_H
#define PLATFORM_H

#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

// 平台特定头文件
#include "your_platform_sdk.h"

#endif
```

### 6. 修改构建系统

#### Makefile

```makefile
# 选择平台
PLATFORM ?= stm32

# 添加平台源文件
PLATFORM_SRC = port/$(PLATFORM)/*.c

# 添加平台头文件
PLATFORM_INC = -Iport/$(PLATFORM)
```

#### CMakeLists.txt

```cmake
# 选择平台
set(PLATFORM "stm32" CACHE STRING "Target platform")

# 添加平台源文件
file(GLOB PLATFORM_SOURCES "port/${PLATFORM}/*.c")

# 添加平台头文件
target_include_directories(embedded PRIVATE "port/${PLATFORM}")
```

## 平台移植清单

### STM32 移植

- [ ] 实现 GPIO (HAL_GPIO)
- [ ] 实现 UART (HAL_UART)
- [ ] 实现 SPI (HAL_SPI)
- [ ] 实现 I2C (HAL_I2C)
- [ ] 实现 ADC (HAL_ADC)
- [ ] 实现 PWM (HAL_TIM)
- [ ] 实现定时器 (SysTick)
- [ ] 实现延时函数
- [ ] 配置中断优先级

### ESP32 移植

- [ ] 实现 GPIO (gpio_num_t)
- [ ] 实现 UART (uart_port_t)
- [ ] 实现 SPI (spi_host_device_t)
- [ ] 实现 I2C (i2c_port_t)
- [ ] 实现 ADC (adc_channel_t)
- [ ] 实现 PWM (ledc_channel_t)
- [ ] 实现 WiFi (esp_wifi)
- [ ] 实现蓝牙 (esp_bt)

### Arduino 移植

- [ ] 实现 GPIO (digitalRead/Write)
- [ ] 实现 UART (Serial)
- [ ] 实现 SPI (SPI)
- [ ] 实现 I2C (Wire)
- [ ] 实现 ADC (analogRead)
- [ ] 实现 PWM (analogWrite)
- [ ] 实现定时器 (millis/micros)

## 测试

### 单元测试

```bash
# 编译测试
make test

# 运行测试
./test/test.elf
```

### 集成测试

```bash
# 编译示例
make examples

# 运行示例
./examples/basic/main.elf
```

## 常见问题

### 1. 编译错误

**问题**: 找不到头文件

**解决**: 检查头文件路径，确保包含正确的目录

### 2. 链接错误

**问题**: 未定义的符号

**解决**: 确保实现了所有 HAL 接口

### 3. 运行时错误

**问题**: 系统崩溃

**解决**: 检查中断优先级和内存分配

## 贡献指南

1. Fork 项目
2. 创建功能分支
3. 提交更改
4. 创建 Pull Request

## 许可证

MIT License
