// filepath: example/dac/main.cpp
// DAC 输出示例
// 功能: 在 PA4 (DAC0_OUT0) 输出可调电压, 通过 RTT 观察当前输出电压值
// 参考: Examples/DAC/DAC_ADC_convert/main.c
//        Examples/DAC/DACC_output_voltage/main.c
//
// 重要: 必须先 include SPL 头文件, 再 include hal.hpp
//        因为 HAL 的 Platform/gd32f4.hpp 会 #undef GPIOA/GPIOB 等宏,
//        之后用裸 SPL 的 gpio_mode_set(GPIOA, ...) 就拿不到这些宏了

extern "C"
{
#include "gd32f4xx.h"
#include "gd32f4xx_dac.h"
#include "gd32f4xx_rcu.h"
}

#include "hal.hpp"
#include "SEGGER_RTT.h"

using namespace HAL::gd32f4;

// DAC0_OUT0 -> PA4, 必须配为 GPIO_MODE_ANALOG
// 使用 HAL 的 AnalogConfig 特化 (与 example/adc 保持一致)
using DAC0_GPIO = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOA_ADDR,
									GPIO_PIN_4,
									GPIO_MODE_ANALOG,
									GPIO_PUPD_NONE,
									HAL::gd32f4::AnalogConfig>;

extern "C" int main(void)
{
	SEGGER_RTT_Init();
	SEGGER_RTT_WriteString(0, "DAC example started, RTT ready\r\n");

	// 1. 使能 DAC 时钟 (PA4 的 GPIO 时钟由 DAC0_GPIO::init() 内部使能)
	rcu_periph_clock_enable(RCU_DAC);

	// 2. 配置 PA4 为模拟模式 (HAL AnalogConfig 模板)
	DAC0_GPIO::init();

	// 3. 复位 DAC0
	dac_deinit(DAC0);

	// 4. 选择软件触发
	dac_trigger_source_config(DAC0, DAC_OUT0, DAC_TRIGGER_SOFTWARE);
	dac_trigger_enable(DAC0, DAC_OUT0);

	// 5. 关闭波形发生器, 开启输出缓冲 (关闭缓冲可得到更高的输出范围, 但驱动能力下降)
	dac_wave_mode_config(DAC0, DAC_OUT0, DAC_WAVE_DISABLE);
	dac_output_buffer_enable(DAC0, DAC_OUT0);

	// 6. 使能 DAC0_OUT0 通道
	dac_enable(DAC0, DAC_OUT0);

	// 输出循环: 在 0 ~ 3.3V 之间产生三角波
	// 12-bit DAC: 0 -> 0V, 4095 -> 3.3V
	uint32_t counter	= 0;
	const uint16_t step = 16; // 步进, 越大越快
	int16_t dir			= 1;
	uint16_t value		= 0;

	while (1)
	{
		// 12-bit 右对齐写入
		dac_data_set(DAC0, DAC_OUT0, DAC_ALIGN_12B_R, value);
		dac_software_trigger_enable(DAC0, DAC_OUT0);
		// 通过 RTT 输出当前电压
		float voltage = (float)value * 3.3f / 4095.0f;
		if ((counter & 0x1F) == 0) // 每 32 次打印一次, 避免 RTT 刷屏
		{
			SEGGER_RTT_printf(0, "[DAC] tick=%lu, value=%u, voltage=%.3f V\r\n",
							  counter, value, (double)voltage);
		}

		// 三角波
		if (dir > 0)
		{
			value += step;
			if (value >= 4095)
			{
				value = 4095;
				dir	  = -1;
			}
		}
		else
		{
			if (value <= step)
			{
				value = 0;
				dir	  = 1;
			}
			else
			{
				value -= step;
			}
		}

		for (uint32_t i = 0; i < 5000; i++);
		++counter;
	}
}
