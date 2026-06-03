#include "hal.hpp"
#include "SEGGER_RTT.h"

#undef RTC
#undef SysTick

using RTC = HAL::gd32f4::RTC_Device;
using SysTick = HAL::gd32f4::SysTick_Delay<HAL::gd32f4::registers::TIMER5_ADDR>;

static const char *weekday_to_str(uint8_t week)
{
	switch (week)
	{
		case 1:
			return "周一";
		case 2:
			return "周二";
		case 3:
			return "周三";
		case 4:
			return "周四";
		case 5:
			return "周五";
		case 6:
			return "周六";
		case 0:
			return "周日";
		default:
			return "未知";
	}
}

extern "C"
{
int main(void)
{
	SEGGER_RTT_Init();

	SEGGER_RTT_WriteString(0, "============================================\r\n");
	SEGGER_RTT_WriteString(0, "  RTC 示例已启动，RTT 就绪\r\n");
	SEGGER_RTT_WriteString(0, "============================================\r\n");

	// 初始化 RTC (LXTAL 32.768KHz)
	SEGGER_RTT_WriteString(0, "[初始化] 正在使能 LXTAL 32.768KHz 与 RTC 时钟...\r\n");
	RTC::init();
	SEGGER_RTT_WriteString(0, "[初始化] LXTAL 已稳定，RTC 时钟就绪。\r\n");

	// 设置初始时间 (十进制输入)
	const uint8_t init_year	  = 0x24; // 2024
	const uint8_t init_month  = 0x01;
	const uint8_t init_date	  = 0x15;
	const uint8_t init_week	  = 0x01; // 周一
	const uint8_t init_hour	  = 0x12;
	const uint8_t init_minute = 0x30;
	const uint8_t init_second = 0;
	SEGGER_RTT_printf(0, "[初始化] 设置时间 -> 20%02u年%02u月%02u日 (%s) %02u:%02u:%02u (十进制)\r\n",
					  init_year, init_month, init_date, weekday_to_str(init_week),
					  init_hour, init_minute, init_second);
	// 转 BCD 后写入 RTC 寄存器
	RTC::set_time(init_year, init_month, init_date,
				  init_week,
				  init_hour,
				  init_minute,
				  init_second);
	SEGGER_RTT_WriteString(0, "[初始化] RTC 配置完成 (BCD 格式)。\r\n");

	// 初始化 SysTick 延时
	SysTick::init();
	SEGGER_RTT_WriteString(0, "[初始化] SysTick (TIMER5) 就绪，节拍 = 1ms。\r\n");
	SEGGER_RTT_WriteString(0, "[循环  ] 每 500ms 打印一次时间...\r\n\r\n");

	uint32_t tick = 0;
	while (1)
	{
		const auto t = RTC::get_time();

		// 读出来的字段是 BCD 格式，先转十进制再打印
		const uint8_t year_d   = t.year;
		const uint8_t month_d  = t.month;
		const uint8_t date_d   = t.date;
		const uint8_t week_d   = t.week;
		const uint8_t hour_d   = t.hour;
		const uint8_t minute_d = t.minute;
		const uint8_t second_d = t.second;

		// 详细日志：十进制可读时间
		SEGGER_RTT_printf(0,
						  "[周期 #%u] 20%02X年%02X月%02X日 (%s) %02X:%02X:%02X (DEC)\r\n",
						  tick++,
						  year_d, month_d, date_d, weekday_to_str(week_d),
						  hour_d, minute_d, second_d);

		// 同时打印寄存器中的 BCD 原始值，便于核对
		SEGGER_RTT_printf(0, "          BCD 原始  年=0x%02X 月=0x%02X 日=0x%02X 周=0x%02X 时=0x%02X 分=0x%02X 秒=0x%02X\r\n",
						  t.year, t.month, t.date, t.week,
						  t.hour, t.minute, t.second);

		SEGGER_RTT_WriteString(0, "----------------------------------------\r\n");

		SysTick::delay_1ms(500);
	}
}
}
