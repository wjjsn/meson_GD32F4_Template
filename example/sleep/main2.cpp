// filepath: example/sleep/main2.cpp
// target2: 循环 "闪 10 次 -> deepsleep -> WKUP 唤醒 -> 再闪 10 次 -> ..."
//   - 不使用调度器
//   - deepsleep 保留 SRAM, 唤醒次数 wakeup_count 累加
//   - 每次进入 deepsleep 前 RTT 打印这是第几次唤醒 (即第几次即将进入睡眠)
//   - PA0 EXTI0 唤醒 deepsleep

extern "C"
{
#include "gd32f4xx.h"
#include "gd32f4xx_pmu.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_exti.h"
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
	for (uint32_t i = 0; i < 10; i++)
	{
		LED::set();
		delay_ms(200);
		LED::clear();
		delay_ms(200);
	}
}

// wakeup 计数: deepsleep 保留 SRAM, 这里用普通 .bss 即可 (deepsleep 不复位 RAM)
static volatile uint32_t wakeup_count = 0;

extern "C"
{
	int main(void)
	{
		SEGGER_RTT_Init();
		SEGGER_RTT_WriteString(0, "[target2] poweron, looping: blink 10 -> deepsleep -> WKUP -> ...\r\n");

		// LED 初始化
		LED::init();
		LED::clear();

		// WKUP 引脚 PA0 + EXTI0 (deepsleep 唤醒源)
		rcu_periph_clock_enable(RCU_GPIOA);
		rcu_periph_clock_enable(RCU_PMU);
		WKUP::init();
		pmu_wakeup_pin_enable();
		exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_RISING);
		exti_interrupt_flag_clear(EXTI_0);
		exti_interrupt_enable(EXTI_0);
		nvic_irq_enable(EXTI0_IRQn, 1U, 0U);

		// SysTick -> 1kHz
		if (SysTick_Config(SystemCoreClock / 1000U))
		{
			while (1) {}
		}
		NVIC_SetPriority(SysTick_IRQn, 0x00U);

		while (1)
		{
			// 每次唤醒: 闪 10 次, 然后打印这是第几次即将进入 deepsleep
			blink_ten_times();

			wakeup_count++;
			SEGGER_RTT_printf(0, "[target2] enter deepsleep #%lu, press WKUP (PA0) to wakeup\r\n",
							  (unsigned long)wakeup_count);

			// 等待 WKUP 释放, 避免立刻唤醒
			while (RESET != gpio_input_bit_get(registers::GPIOA_ADDR, GPIO_PIN_0)) {}

			// 关闭 SysTick, 避免进 deepsleep 期间继续累加
			SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

			// 切换到低速时钟
			enter_lowpower_clock();

			// 进入 deep-sleep: 低功耗 LDO + 低驱动 + WFI
			pmu_to_deepsleepmode(PMU_LDO_LOWPOWER, PMU_LOWDRIVER_ENABLE, WFI_CMD);

			// 唤醒后: 重新使能 SysTick
			SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
		}
	}

	void EXTI0_IRQHandler(void)
	{
		if (RESET != exti_interrupt_flag_get(EXTI_0))
		{
			exti_interrupt_flag_clear(EXTI_0);
		}
	}
}
