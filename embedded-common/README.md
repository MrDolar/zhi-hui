# Embedded Common Library

嵌入式通用代码库 - 用最少代码干最多事

## 特性

- **简约风格**: 精简代码，高效实现
- **统一接口**: 函数指针抽象，易于移植
- **模块化**: 按需包含，减小体积
- **跨平台**: 支持 STM32/ESP32/Arduino/RISC-V

## 模块

| 模块 | 说明 | 文件 |
|------|------|------|
| HAL | 硬件抽象层 | hal/*.h |
| Protocol | 通信协议 | protocol/*.h |
| DataStructure | 数据结构 | data_structure/*.h |
| Driver | 设备驱动 | driver/*.h |
| System | 系统功能 | system/*.h |
| Security | 安全加密 | security/*.h |
| Utility | 工具函数 | utility/*.h |

## 快速开始

```c
#include "hal/gpio.h"
#include "utility/log.h"

int main(void) {
    log_init(LOG_DEBUG);
    gpio_init(LED_PIN, GPIO_OUTPUT);
    gpio_write(LED_PIN, GPIO_HIGH);
    log_info("LED ON");
    return 0;
}
```

## 移植

1. 实现 `port/` 目录下的平台接口
2. 包含对应平台的头文件
3. 编译运行

## 许可证

MIT License
