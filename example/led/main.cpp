#include "hal.hpp"

using LED = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOE_ADDR,
                              GPIO_PIN_3,
                              GPIO_MODE_OUTPUT,
                              GPIO_PUPD_PULLDOWN,
                              HAL::gd32f4::OutputConfig<GPIO_OTYPE_PP,
                                                        GPIO_OSPEED_2MHZ,
                                                        SET>>;

extern "C"
{
int main(void)
{
    LED::init();

    while (1)
    {
        LED::set();
        for (volatile uint32_t i = 0; i < 100000; ++i)
            ;
        LED::clear();
        for (volatile uint32_t i = 0; i < 100000; ++i)
            ;
    }
}
}
