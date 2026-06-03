// filepath: example/sleep/main3.cpp
// target3: 醒来一次闪十下, 然后 standby, PA0 WKUP 唤醒
//   - 不使用调度器
//   - main 顶部先闪 10 次 (200ms 翻转, 10 个完整周期)
//   - 然后进入 standby; PA0 WKUP 唤醒后等价于芯片复位, 重新从 main 开头执行
//   - 每次唤醒/上电都闪 10 次再睡

extern "C"
{
#include "gd32f4xx.h"
#include "gd32f4xx_pmu.h"
#include "gd32f4xx_rcu.h"
}

#include "hal.hpp"
#include "SEGGER_RTT.h"

using namespace HAL::gd32f4;

// LED 指示灯, 复用 PE3
using LED = GPIO<registers::GPIOE_ADDR,
				 GPIO_PIN_3,
				 GPIO_MODE_OUTPUT,
				 GPIO_PUPD_PULLDOWN,
				 OutputConfig<GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, RESET>>;

// WKUP 引脚 PA0
using WKUP = GPIO<registers::GPIOA_ADDR,
				  GPIO_PIN_0,
				  GPIO_MODE_INPUT,
				  GPIO_PUPD_NONE,
				  void>;

// 软件延时, 防止 Vcore 波动
static void soft_delay(uint32_t time)
{
	__IO uint32_t i;
	for (i = 0; i < time * 10; i++) {}
}

// 切换到低速时钟, 进入低功耗前的标准流程
static void enter_lowpower_clock(void)
{
	rcu_ahb_clock_config(RCU_AHB_CKSYS_DIV2);
	soft_delay(0x50);
	rcu_ahb_clock_config(RCU_AHB_CKSYS_DIV4);
	soft_delay(0x50);
	rcu_ahb_clock_config(RCU_AHB_CKSYS_DIV8);
	soft_delay(0x50);
	rcu_ahb_clock_config(RCU_AHB_CKSYS_DIV16);
	soft_delay(0x50);
	rcu_system_clock_source_config(RCU_CKSYSSRC_IRC16M);
	soft_delay(200);
	rcu_ahb_clock_config(RCU_AHB_CKSYS_DIV1);
}

// SysTick 以 1kHz 中断, 提供毫秒级延时
static volatile uint32_t systick_tick_ms = 0;

extern "C" void SysTick_Handler(void)
{
	systick_tick_ms = systick_tick_ms + 1U;
}

// 简单阻塞延时 (ms)
static void delay_ms(uint32_t ms)
{
	uint32_t start = systick_tick_ms;
	while ((systick_tick_ms - start) < ms) {}
}

// LED 闪烁 10 次 (开/关 翻转各 10 次, 每次亮/灭各 200ms)
static void blink_ten_times(void)
{
	SEGGER_RTT_WriteString(0, "[target3] blink 10 times\r\n");
	for (uint32_t i = 0; i < 10; i++)
	{
		LED::set();
		delay_ms(200);
		LED::clear();
		delay_ms(200);
	}
}

extern "C"
{
	int main(void)
	{
		SEGGER_RTT_Init();
		SEGGER_RTT_WriteString(0, "[target3] wakeup/poweron, will blink 10 times then pmu_to_standbymode\r\n");

		// LED 初始化
		LED::init();
		LED::clear();

		// WKUP 引脚 PA0 (standby 唤醒源)
		rcu_periph_clock_enable(RCU_GPIOA);
		rcu_periph_clock_enable(RCU_PMU);
		WKUP::init();
		pmu_wakeup_pin_enable();
		// 清除 PMU 待机标志
		pmu_flag_clear(PMU_FLAG_RESET_STANDBY);

		// SysTick -> 1kHz
		if (SysTick_Config(SystemCoreClock / 1000U))
		{
			while (1) {}
		}
		NVIC_SetPriority(SysTick_IRQn, 0x00U);

		// 闪 10 次
		blink_ten_times();

		SEGGER_RTT_WriteString(0, "[target3] entering pmu_to_standbymode, press WKUP (PA0) to wakeup (reset)\r\n");

		// 等待 WKUP 释放, 避免立刻唤醒
		while (RESET != gpio_input_bit_get(registers::GPIOA_ADDR, GPIO_PIN_0)) {}

		// 关闭 SysTick
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

		// 切换到低速时钟
		enter_lowpower_clock();

		// 进入 standby: 唤醒后等价于芯片复位, 重新从 main 开始
		pmu_to_standbymode();

		// standby 唤醒后程序不会回到这里
		while (1)
		{
		}
	}
}
