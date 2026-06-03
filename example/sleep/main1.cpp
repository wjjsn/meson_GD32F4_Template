// filepath: example/sleep/main1.cpp
// target1: 闪灯调度器空闲进 pmu_to_sleepmode
//   - LED 每 500ms 翻转一次 (由调度器驱动)
//   - 调度器每轮 poll 没有任务到期时返回 false, 此时主循环进入 sleep mode
//   - SysTick 中断会唤醒 CPU, 醒来继续轮询调度器
//
// 注意: 调度器本身不主动调用 pmu_to_sleepmode, 由 main 根据 poll() 的 bool 返回值手动进 sleep.

// 必须先包含 SPL 头文件, 再包含 hal.hpp
// hal.hpp 会 undef 掉 GPIOA/GPIOB 等宏, 之后再用裸 SPL 函数就拿不到这些宏了
extern "C"
{
#include "gd32f4xx.h"
#include "gd32f4xx_pmu.h"
}

#include "hal.hpp"
#include "schedule.hpp"
#include "SEGGER_RTT.h"

using namespace HAL::gd32f4;

// LED 指示灯, 复用 PE3
using LED = GPIO<registers::GPIOE_ADDR,
                 GPIO_PIN_3,
                 GPIO_MODE_OUTPUT,
                 GPIO_PUPD_PULLDOWN,
                 OutputConfig<GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, RESET>>;

// SysTick 以 1kHz 中断, 累加毫秒计数
static volatile uint32_t systick_tick_ms = 0;

extern "C" uint64_t systick_get_ms()
{
    return systick_tick_ms;
}

static void led_toggle()
{
    LED::toggle();
}

// 调度器: 每 500ms 翻转一次 LED
using Scheduler = StaticTimerManager<
    systick_get_ms,
    TaskConfig{500, led_toggle}
>;

extern "C"
{
int main(void)
{
    SEGGER_RTT_Init();
    SEGGER_RTT_WriteString(0, "target1: LED blink + idle pmu_to_sleepmode\r\n");

    LED::init();
    LED::clear();

    // SysTick -> 1kHz
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        while (1) {}
    }
    NVIC_SetPriority(SysTick_IRQn, 0x00U);

    while (1)
    {
        // 调度器空闲 (本轮无任务到期) 时进入 sleep mode
        // 任何中断 (含 SysTick) 都能唤醒; 唤醒后继续轮询
        if (!Scheduler::poll())
        {
            pmu_to_sleepmode(WFI_CMD);
        }
    }
}

void SysTick_Handler(void)
{
    systick_tick_ms = systick_tick_ms + 1U;
}
}
