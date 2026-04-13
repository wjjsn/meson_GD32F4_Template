#include "hal.hpp"

using PE3 = HAL::gd32f4::GPIO<GPIOE, GPIO_PIN_3, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, HAL::gd32f4::OutputConfig<GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, SET>>;
extern "C"
{
	int main(void)
	{
		PE3::init();
		while (1)
		{
			PE3::set();
			for (uint64_t i = 0; i < 1145141; ++i)
			{
			}
			PE3::clear();
			for (uint64_t i = 0; i < 1145141; ++i)
			{
			}
		}
	}
}