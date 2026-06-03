#include "hal.hpp"
#include "SEGGER_RTT.h"
using ADC0_CONFIG = HAL::gd32f4::ADC_config<HAL::gd32f4::registers::ADC0_ADDR,
											ADC_ADCCK_PCLK2_DIV6>;

using ADC0 = HAL::gd32f4::ADC<ADC0_CONFIG>;

// ADC_CHANNEL_10 -> PC0, 必须配为 GPIO_MODE_ANALOG
// PULLDOWN 对齐 reference 2025030920/SysFunction/Src/main.c:26
using ADC0_GPIO = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOC_ADDR,
									GPIO_PIN_0,
									GPIO_MODE_ANALOG,
									GPIO_PUPD_PULLDOWN,
									HAL::gd32f4::AnalogConfig>;

extern "C"
{
int main(void)
{
	SEGGER_RTT_Init();
	SEGGER_RTT_WriteString(0, "ADC example started, RTT ready\r\n");

	// 初始化顺序: GPIO (analog) -> ADC 控制器 -> 选通道
	// 与 reference 2025030920/SysFunction/Src/main.c:48-50 一致
	ADC0_GPIO::init();
		ADC0::init();
		ADC0::set_channel(ADC_CHANNEL_10);

		while (1)
		{
			uint16_t value = ADC0::get_value();
			SEGGER_RTT_printf(0, "ADC Value: %d, %fV\r\n", value, value * 3.3 / 4095);

			for (uint32_t i = 0; i < 100000; ++i);
		}
}
}
