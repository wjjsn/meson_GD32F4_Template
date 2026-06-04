#include "Platform/gd32f4.hpp"
#include "hal.hpp"
#include "gd30ad3340.hpp"
#include "SEGGER_RTT.h"
#include <charconv>

// ========== I2C 总线配置 ==========
// I2C0: SCL = PB8, SDA = PB9, AF4
using I2C0_SCL = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOB_ADDR,
                                   GPIO_PIN_8,
                                   GPIO_MODE_AF,
                                   GPIO_PUPD_NONE,
                                   HAL::gd32f4::AFConfig<GPIO_AF_4>>;
using I2C0_SDA = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOB_ADDR,
                                   GPIO_PIN_9,
                                   GPIO_MODE_AF,
                                   GPIO_PUPD_NONE,
                                   HAL::gd32f4::AFConfig<GPIO_AF_4>>;

using I2C0_BUS = HAL::gd32f4::I2C_bus<I2C0_SDA,
                                      I2C0_SCL,
                                      HAL::gd32f4::registers::I2C0_ADDR,
                                      400000>;  // 400kHz 快速模式

// ========== GD30AD3340 设备 ==========
// 7-bit 地址 0x48 → 8-bit 写入地址 0x90 (ADDR 接 GND)
using ADC_I2C = HAL::gd32f4::I2C_device_addr<I2C0_BUS, 0x90>;
using ADC     = GD30AD3340<ADC_I2C>;

// ========== LED 指示 (PE3) ==========
using LED = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOE_ADDR,
                              GPIO_PIN_3,
                              GPIO_MODE_OUTPUT,
                              GPIO_PUPD_PULLDOWN,
                              HAL::gd32f4::OutputConfig<GPIO_OTYPE_PP,
                                                        GPIO_OSPEED_2MHZ,
                                                        SET>>;

#include <charconv>
#include <cstdio>
extern "C"
{

	FILE *const stderr = nullptr;
	int _exit(){
        while(1);
    } int main(void)
	{
        // ===== 系统初始化 =====
        SEGGER_RTT_Init();
        LED::init();

        // ===== 外设初始化 =====
        I2C0_BUS::init();
        ADC adc;

        // 单端 AIN0, ±2.048V, 连续转换, 128SPS, 禁用比较器
        adc.init(ADC::MUX_AIN0_GND,
                 ADC::PGA_2048,
                 ADC::MODE_CONTINUOUS,
                 ADC::DR_100,
                 ADC::COMP_QUE_DIS);

        SEGGER_RTT_WriteString(0, "GD30AD3340 example started, RTT ready\n");

        // ===== 主循环 =====
        uint32_t counter = 0;
        while (1)
        {
            // 读取原始值和电压
            // int16_t raw   = adc.read_raw();
            // float   volt  = adc.read_voltage();
            float   temp  = adc.read_temperature();
            char buf[32];
            auto r = std::to_chars(buf, buf + sizeof(buf)-1, temp);
            if (r.ec == std::errc()) {
                *r.ptr = '\0';  // 添加字符串结尾
            } else {
                buf[0] = '\0'; // 转换失败则输出空字符串
            }
            SEGGER_RTT_printf(0, "temp: %s C\n", buf);

            // LED 翻转指示运行状态
            LED::toggle();

            for (uint32_t i = 0; i < 20000000; i++);
            ++counter;
        }
    }
}
