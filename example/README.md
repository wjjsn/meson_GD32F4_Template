# HAL Examples 说明

本文档介绍 GD32F4xx HAL 库的所有示例程序。

## 目录结构

```
example/
├── led/        # GPIO 输出示例 (LED 闪烁)
├── spi/        # SPI 通讯示例
├── usart/      # USART 串口示例
├── adc/        # ADC 模数转换示例
├── crc/        # CRC 校验示例
└── rtc/        # RTC 实时时钟和延时示例
```

## 构建说明

所有示例使用 Meson 构建系统:

```bash
# 构建所有示例
cd build
meson compile

# 或单独构建某个示例
cd build/example/spi
ninja
```

## 示例详情

---

### 1. LED 示例 (led/)

**功能**: GPIO 输出, LED 控制

**主要文件**:
- `src/main.cpp` - LED 闪烁实现

**使用组件**:
- `HAL::gd32f4::GPIO` - 通用输入输出

**示例场景**:
1. LED 基础闪烁
2. 按钮控制 LED
3. PWM 呼吸灯

---

### 2. SPI 示例 (spi/)

**功能**: SPI 主设备通讯

**主要文件**:
- `inc/spi_example.hpp` - 配置和辅助函数
- `src/main.cpp` - 10+ 使用场景

**使用组件**:
- `HAL::gd32f4::SPI_config` - SPI 配置模板
- `HAL::gd32f4::SPI` - SPI 主设备

**配置选项**:
| 配置项 | 说明 |
|--------|------|
| 时钟极性 | MODE0 (CPOL=0, CPHA=0), MODE3 (CPOL=1, CPHA=1) |
| 预分频 | 2/4/8/16/32/64/128/256 |
| 数据宽度 | 8 位 / 16 位 |
| 、片选 | 软件控制 (可对接多个从设备) |

**示例场景**:
1. 单字节发送/接收
2. 多字节连续传输
3. 多从设备切换
4. Flash 读写操作
5. 高频模式通讯

---

### 3. USART 示例 (usart/)

**功能**: 串口通讯

**主要文件**:
- `inc/usart_example.hpp` - 配置和辅助函数
- `src/main.cpp` - 10+ 使用场景

**使用组件**:
- `HAL::gd32f4::USART` - 串口通讯

**配置选项**:
| 配置项 | 说明 |
|--------|------|
| 波特率 | 9600, 115200, 921600 等 |
| 数据位 | 8 位 / 9 位 |
| 停止位 | 1 位 / 2 位 |
| 校验位 | 无校验 / 奇校验 / 偶校验 |

**示例场景**:
1. 发送字符串
2. 发送数字
3. 接收数据
4. 收发转换
5. Modbus RTU 协议
6. 超时检测

---

### 4. ADC 示例 (adc/)

**功能**: 模数转换

**主要文件**:
- `inc/adc_example.hpp` - 配置和辅助函数
- `src/main.cpp` - 10+ 使用场景

**使用组件**:
- `HAL::gd32f4::ADC_config` - ADC 配置模板
- `HAL::gd32f4::ADC` - ADC 设备

**配置选项**:
| 配置项 | 说明 |
|--------|------|
| 分辨率 | 12 位 (4095) |
| 采样时间 | 3/15/28/56/84/112/144/480 周期 |
| 转换模式 | 单次转换 / 连续转换 |
| 触发源 | 软件触发 |

**通道** (示例配置):
| 通道 | 引脚 | 用途 |
|------|------|------|
| ADC0 | PA0 | 光敏电阻 |
| ADC1 | PA1 | 热敏电阻 |
| ADC2 | PA2 | 电位器 |
| ADC3 | PA3 | 湿度传感器 |

**示例场景**:
1. 单通道读取
2. 多通道轮流读取
3. 多次平均
4. 光敏电阻
5. 热敏电阻
6. 湿度传感器
7. 电位器
8. 电池监控

---

### 5. CRC 示例 (crc/)

**功能**: CRC16_MODBUS 校验

**主要文件**:
- `inc/crc_example.hpp` - 辅助函数
- `src/main.cpp` - 10+ 使用场景

**使用组件**:
- `HAL::gd32f4::CRC16_MODBUS` - CRC 计算器

**CRC 参数**:
| 参数 | 值 |
|------|-----|
| 多项式 | 0x8005 |
| 初始值 | 0xFFFF |
| 结果取反 | 是 |
| 字节序 | 低字节在前 |

**示例场景**:
1. 基本 CRC 计算
2. Modbus 报文构造
3. CRC 验证
4. Modbus 响应验证
5. 数据完整性检查
6. 多数据包处理
7. HEX 字符串转换
8. 错误检测

---

### 6. RTC 示例 (rtc/)

**功能**: 实时时钟和延时

**主要文件**:
- `inc/rtc_example.hpp` - 时间辅助函数
- `src/main.cpp` - 17 个使用场景

**使用组件**:
- `HAL::gd32f4::RTC` - 实时时钟
- `HAL::gd32f4::SysTick` - 延时定时器

**RTC 配置**:
| 配置项 | 值 |
|--------|-----|
| 时钟源 | LXTAL 32.768 KHz |
| 异步预分频 | 127 |
| 同步预分频 | 255 |
| 时钟周期 | 1 秒 |

**SysTick 配置**:
| 延时类型 | 分辨率 |
|----------|--------|
| 纳秒延时 | 100 ns |
| 微秒延时 | 1 μs |
| 毫秒延时 | 1 ms |

**示例场景**:
1. RTC 初始化
2. 设置日期时间
3. 日期验证
4. 时间验证
5. 闰年判断
6. 月天数计算
7. 星期计算
8. SysTick 初始化
9. 微秒延时
10. 毫秒延时
11. 纳秒延时
12. 周期任务
13. 日期时间格式化
14. 综合初始化
15. LED 闪烁
16. 按键消抖
17. 定时采样

---

## 通用组件

### GPIO 配置模板

```cpp
// 输出模式
OutputConfig<GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SET>

// 复用功能模式
AFConfig<GPIO_AF_4>

// 输入模式
void
```

### 引脚定义

```cpp
// GPIO 端口
using GPIOA = GPIO_TypeDef*;
using GPIOB = GPIO_TypeDef*;
// ...

// GPIO 引脚
GPIO_PIN_0 ~ GPIO_PIN_15
```

---

## 编译时检查

HAL 库使用 `static_assert` 和 `requires` 进行编译时检查:

```cpp
// SPI 模式检查
static_assert(TIMING_CONFIG::mode == SPI_MODE_0 || TIMING_CONFIG::mode == SPI_MODE_3);

// ADC 通道检查
static_assert(CHANNEL < 16);

// PWM 频率检查
static_assert(!(frequency_ == 0 || frequency_ > 1000000));
```

---

## 性能考虑

1. **SPI**: 最高频率受 APB 时钟限制
2. **ADC**: 采样时间越长精度越高但速度越慢
3. **CRC**: 查表法比逐位计算快
4. **SysTick**: 使用 TIMER5, 无中断开销

---

## 故障排除

### SPI 通讯失败
1. 检查引脚配置 (AF4)
2. 检查时钟使能
3. 确认片选信号正确

### ADC 读数为 0
1. 检查通道配置
2. 确认参考电压
3. 检查引脚模式

### RTC 不走时
1. 检查 LXTAL 晶振连接
2. 确认备份域供电
3. 检查RTC初始化顺序

---

## 更多资源

- [HAL 库文档](../../subprojects/HAL/README.md)
- [GD32F4xx 参考手册](https://www.gd32mcu.com/)