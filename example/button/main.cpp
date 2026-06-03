#include "hal.hpp"
#include "button.hpp"
#include "schedule.hpp"
#include "SEGGER_RTT.h"
#include "gd32f4xx.h"

// Configuration Begin
// 6 个按键, 都为低电平触发 (按下拉低)
using KEY1_GPIO = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOB_ADDR,
									GPIO_PIN_11,
									GPIO_MODE_INPUT,
									GPIO_PUPD_PULLUP,
									void>;
using KEY2_GPIO = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOE_ADDR,
									GPIO_PIN_15,
									GPIO_MODE_INPUT,
									GPIO_PUPD_PULLUP,
									void>;
using KEY3_GPIO = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOE_ADDR,
									GPIO_PIN_13,
									GPIO_MODE_INPUT,
									GPIO_PUPD_PULLUP,
									void>;
using KEY4_GPIO = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOE_ADDR,
									GPIO_PIN_11,
									GPIO_MODE_INPUT,
									GPIO_PUPD_PULLUP,
									void>;
using KEY5_GPIO = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOE_ADDR,
									GPIO_PIN_9,
									GPIO_MODE_INPUT,
									GPIO_PUPD_PULLUP,
									void>;
using KEY6_GPIO = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOE_ADDR,
									GPIO_PIN_7,
									GPIO_MODE_INPUT,
									GPIO_PUPD_PULLUP,
									void>;
// Configuration End

// 按键事件回调: 输出按键编号 + 事件类型到 RTT
static void on_k1_single()
{
	SEGGER_RTT_WriteString(0, "KEY1: single click\n");
}
static void on_k1_double()
{
	SEGGER_RTT_WriteString(0, "KEY1: double click\n");
}
static void on_k1_hold()
{
	SEGGER_RTT_WriteString(0, "KEY1: hold\n");
}
static void on_k2_single()
{
	SEGGER_RTT_WriteString(0, "KEY2: single click\n");
}
static void on_k2_double()
{
	SEGGER_RTT_WriteString(0, "KEY2: double click\n");
}
static void on_k2_hold()
{
	SEGGER_RTT_WriteString(0, "KEY2: hold\n");
}
static void on_k3_single()
{
	SEGGER_RTT_WriteString(0, "KEY3: single click\n");
}
static void on_k3_double()
{
	SEGGER_RTT_WriteString(0, "KEY3: double click\n");
}
static void on_k3_hold()
{
	SEGGER_RTT_WriteString(0, "KEY3: hold\n");
}
static void on_k4_single()
{
	SEGGER_RTT_WriteString(0, "KEY4: single click\n");
}
static void on_k4_double()
{
	SEGGER_RTT_WriteString(0, "KEY4: double click\n");
}
static void on_k4_hold()
{
	SEGGER_RTT_WriteString(0, "KEY4: hold\n");
}
static void on_k5_single()
{
	SEGGER_RTT_WriteString(0, "KEY5: single click\n");
}
static void on_k5_double()
{
	SEGGER_RTT_WriteString(0, "KEY5: double click\n");
}
static void on_k5_hold()
{
	SEGGER_RTT_WriteString(0, "KEY5: hold\n");
}
static void on_k6_single()
{
	SEGGER_RTT_WriteString(0, "KEY6: single click\n");
}
static void on_k6_double()
{
	SEGGER_RTT_WriteString(0, "KEY6: double click\n");
}
static void on_k6_hold()
{
	SEGGER_RTT_WriteString(0, "KEY6: hold\n");
}

// 6 个独立按键, 低电平触发, 每个注册 长按 + 单击 + 双击 三组回调
using Key1 = StaticKey<KEY1_GPIO, false, on_k1_hold, on_k1_single, on_k1_double>;
using Key2 = StaticKey<KEY2_GPIO, false, on_k2_hold, on_k2_single, on_k2_double>;
using Key3 = StaticKey<KEY3_GPIO, false, on_k3_hold, on_k3_single, on_k3_double>;
using Key4 = StaticKey<KEY4_GPIO, false, on_k4_hold, on_k4_single, on_k4_double>;
using Key5 = StaticKey<KEY5_GPIO, false, on_k5_hold, on_k5_single, on_k5_double>;
using Key6 = StaticKey<KEY6_GPIO, false, on_k6_hold, on_k6_single, on_k6_double>;


// SysTick 以 1kHz 中断, 在中断中累加毫秒计数, 供调度器读取
static volatile uint32_t systick_tick_ms = 0;

extern "C" uint64_t systick_get_ms()
{
	return systick_tick_ms;
}

// 时间片轮询调度器:
//   - 每 5ms 扫描一次按键 (消抖 + 连击计数)
//   - 每 300ms 处理一次连击事件 (主循环上下文执行)
using Scheduler = StaticTimerManager<
	systick_get_ms,
	TaskConfig{5, []
			   { Key1::detect_key_click(); Key2::detect_key_click();
	                               Key3::detect_key_click(); Key4::detect_key_click();
	                               Key5::detect_key_click(); Key6::detect_key_click(); }},
	TaskConfig{300, []
			   { Key1::cope_click_data(); Key2::cope_click_data();
	                               Key3::cope_click_data(); Key4::cope_click_data();
	                               Key5::cope_click_data(); Key6::cope_click_data(); }}>;

extern "C"
{
	int main(void)
	{
		// System Initialize Begin
		SEGGER_RTT_Init();

		// SysTick -> 1kHz 中断, 累加 systick_tick_ms
		if (SysTick_Config(SystemCoreClock / 1000U))
		{
			while (1) {}
		}
		NVIC_SetPriority(SysTick_IRQn, 0x00U);
		// System Initialize End

		// Peripherals Initialize Begin
		Key1::init();
		Key2::init();
		Key3::init();
		Key4::init();
		Key5::init();
		Key6::init();
		// Peripherals Initialize End

		SEGGER_RTT_WriteString(0, "Button example started (systick + StaticTimerManager)\n");

		// User Begin
		// User End

		while (1)
		{
			Scheduler::poll();
		}
	}

	void SysTick_Handler(void)
	{
		systick_tick_ms = systick_tick_ms + 1U;
	}
}
