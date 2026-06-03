#include "hal.hpp"
#include "button.hpp"
#include "SEGGER_RTT.h"

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

// Configuration Begin
constexpr uint32_t SCAN_PERIOD_MS = 20;	   // 按键扫描周期
constexpr uint32_t TICK_FREQ_HZ	  = 10000; // 计数频率 10kHz -> 每 tick 0.1ms
// Configuration End

extern "C"
{
	int main(void)
	{
		// System Initialize Begin
		SEGGER_RTT_Init();
		// System Initialize End

		// Peripherals Initialize Begin
		Key1::init();
		Key2::init();
		Key3::init();
		Key4::init();
		Key5::init();
		Key6::init();

		// 配置 TIMER6 为按键扫描周期中断.
		// 实际 APB1 时钟由硬件决定, 运行时读取后算出 PSC/ARR, 避免凭假设写死.
		rcu_periph_clock_enable(HAL::gd32f4::RCU_periph<HAL::gd32f4::registers::TIMER6_ADDR>::periph);
		rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

		const uint32_t tim_clk = rcu_clock_freq_get(CK_APB1) * 4U; // GD32F4 APB1 倍频后给 TIMER
		SEGGER_RTT_printf(0, "TIMER6 input clock: %lu Hz\n", tim_clk);

		const uint16_t psc = static_cast<uint16_t>(tim_clk / TICK_FREQ_HZ - 1U);
		const uint32_t arr = SCAN_PERIOD_MS * (TICK_FREQ_HZ / 1000U) - 1U;
		SEGGER_RTT_printf(0, "TIMER6: psc=%u, arr=%lu, period=%lu ms\n",
						  psc, arr, (arr + 1U) * 1000U / TICK_FREQ_HZ);

		timer_parameter_struct timer_param = {
			psc,
			TIMER_COUNTER_EDGE,
			TIMER_COUNTER_UP,
			TIMER_CKDIV_DIV1,
			static_cast<uint16_t>(arr),
			0U};
		timer_init(HAL::gd32f4::registers::TIMER6_ADDR, &timer_param);
		timer_interrupt_enable(HAL::gd32f4::registers::TIMER6_ADDR, TIMER_INT_UP);
		nvic_irq_enable(TIMER6_IRQn, 1, 0);
		timer_enable(HAL::gd32f4::registers::TIMER6_ADDR);
		// Peripherals Initialize End

		SEGGER_RTT_WriteString(0, "Button example started, RTT ready\n");

		// User Begin
		// User End

		while (1)
		{
			Key1::cope_click_data();
			Key2::cope_click_data();
			Key3::cope_click_data();
			Key4::cope_click_data();
			Key5::cope_click_data();
			Key6::cope_click_data();

			for (uint32_t i = 0; i < 10000000; i++);
		}
	}

	void TIMER6_IRQHandler()
	{
		if (timer_interrupt_flag_get(HAL::gd32f4::registers::TIMER6_ADDR, TIMER_INT_UP) == SET)
		{
			timer_interrupt_flag_clear(HAL::gd32f4::registers::TIMER6_ADDR, TIMER_INT_UP);
			Key1::detect_key_click();
			Key2::detect_key_click();
			Key3::detect_key_click();
			Key4::detect_key_click();
			Key5::detect_key_click();
			Key6::detect_key_click();
		}
	}
}
