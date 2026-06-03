#include "Platform/gd32f4.hpp"
#include "hal.hpp"
#include "OLED/ssd1306/0.91.hpp"
#include "SEGGER_RTT.h"

// Configuration Begin
//  I2C0 SCL = PB8, SDA = PB9, AF4
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
									  400000>;

// SSD1306 7-bit 地址 0x3C
#define OLED_ADDRESS 0x78
using OLED_I2C = HAL::gd32f4::I2C_device_addr<I2C0_BUS, OLED_ADDRESS>;
using Screen   = OLED<OLED_I2C>;
// Configuration End

extern "C"
{
	int main(void)
	{
		// System Initialize Begin
		SEGGER_RTT_Init();
		// System Initialize End

		// Peripherals Initialize Begin
		Screen screen;
		I2C0_BUS::init();
		screen.init();
		// Peripherals Initialize End

		SEGGER_RTT_WriteString(0, "OLED example started, RTT ready\n");

		// User Begin
		uint32_t counter = 0;
		// User End

		while (1)
		{
			screen.printf(0, 0, "OLED 0.91\"");
			screen.printf(2, 0, "I2C0 400kHz");
			screen.printf(4, 0, "cnt:%lu", counter);
			screen.printf(6, 0, "GD32F470");
			screen.update();

			SEGGER_RTT_printf(0, "tick: %lu\n", counter);

			for (volatile uint32_t i = 0; i < 1000000; i++);

			++counter;
		}
	}
}
