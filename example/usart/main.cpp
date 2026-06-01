#include "hal.hpp"

using USART1_TX = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOA_ADDR,
                                   GPIO_PIN_9,
                                   GPIO_MODE_AF,
                                   GPIO_PUPD_PULLUP,
                                   HAL::gd32f4::AFConfig<GPIO_AF_7>>;

using USART1_RX = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOA_ADDR,
                                   GPIO_PIN_10,
                                   GPIO_MODE_AF,
                                   GPIO_PUPD_NONE,
                                   HAL::gd32f4::AFConfig<GPIO_AF_7>>;

using USART1 = HAL::gd32f4::USART_Device<USART1_TX,
                                        USART1_RX,
                                        HAL::gd32f4::registers::USART1_ADDR,
                                        115200,
                                        USART_PM_NONE,
                                        USART_WL_8BIT,
                                        USART_STB_1BIT>;

extern "C"
{
int main(void)
{
    USART1::init();

    constexpr uint8_t msg[] = "Hello USART\r\n";

    while (1)
    {
        USART1::transmit(msg, sizeof(msg) - 1, 0);
        for (volatile uint32_t i = 0; i < 100000; ++i)
            ;
    }
}
}
