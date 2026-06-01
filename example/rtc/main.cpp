#include "hal.hpp"

#undef RTC
#undef SysTick

using RTC = HAL::gd32f4::RTC_Device;
using SysTick = HAL::gd32f4::SysTick_Delay<HAL::gd32f4::registers::TIMER5_ADDR>;

extern "C"
{
int main(void)
{
    RTC::init();
    RTC::set_time(24, 1, 15, 1, 12, 30, 0);
    SysTick::init();

    while (1)
    {
        SysTick::delay_1ms(500);
    }
}
}
