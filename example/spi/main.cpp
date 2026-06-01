#include "hal.hpp"

using SPI1_MOSI = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOB_ADDR,
                                   GPIO_PIN_15,
                                   GPIO_MODE_AF,
                                   GPIO_PUPD_NONE,
                                   HAL::gd32f4::AFConfig<GPIO_AF_5>>;

using SPI1_MISO = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOB_ADDR,
                                   GPIO_PIN_14,
                                   GPIO_MODE_AF,
                                   GPIO_PUPD_PULLDOWN,
                                   HAL::gd32f4::AFConfig<GPIO_AF_5>>;

using SPI1_SCLK = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOB_ADDR,
                                   GPIO_PIN_13,
                                   GPIO_MODE_AF,
                                   GPIO_PUPD_NONE,
                                   HAL::gd32f4::AFConfig<GPIO_AF_5>>;

using SPI1_CONFIG = HAL::gd32f4::SPI_config<HAL::gd32f4::registers::SPI1_ADDR,
                                            SPI_PSC_64,
                                            SPI_CK_PL_LOW_PH_1EDGE,
                                            SPI_MASTER,
                                            SPI_NSS_SOFT>;

using SPI1 = HAL::gd32f4::SPI<SPI1_MOSI, SPI1_MISO, SPI1_SCLK, SPI1_CONFIG>;


using SPI1_NSS_GPIO = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOG_ADDR,
                                       GPIO_PIN_10,
                                       GPIO_MODE_OUTPUT,
                                       GPIO_PUPD_PULLUP,
                                       HAL::gd32f4::OutputConfig<GPIO_OTYPE_PP,
                                                                 GPIO_OSPEED_50MHZ,
                                                                 SET>>;

extern "C"
{
int main(void)
{
    SPI1::init();
    SPI1_NSS_GPIO::init();

    constexpr uint8_t tx_data[] = {0x9F, 0x00, 0x00, 0x00};
    uint8_t rx_data[sizeof(tx_data)] = {0};

    while (1)
    {
        SPI1_NSS_GPIO::clear();
        SPI1::send_rec_bytes(tx_data, rx_data, sizeof(tx_data));
        SPI1_NSS_GPIO::set();

        for (volatile uint32_t i = 0; i < 100000; ++i)
            ;
    }
}
}
