#include "hal.hpp"

using ADC0_CONFIG = HAL::gd32f4::ADC_config<HAL::gd32f4::registers::ADC0_ADDR,
                                           ADC_ADCCK_PCLK2_DIV4>;

using ADC0 = HAL::gd32f4::ADC<ADC0_CONFIG>;

extern "C"
{
int main(void)
{
    ADC0::init();
    ADC0::set_channel(ADC_CHANNEL_0);


    while (1)
    {
        uint32_t value = ADC0::get_value();
        (void)value;

        for (volatile uint32_t i = 0; i < 100000; ++i)
            ;
    }
}
}
