#include "hal.hpp"
#include "SEGGER_RTT.h"
#include "gd25q.hpp"

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

using SPI1_BUS = HAL::gd32f4::SPI<SPI1_MOSI, SPI1_MISO, SPI1_SCLK, SPI1_CONFIG>;

using SPI1_NSS_GPIO = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOB_ADDR,
										GPIO_PIN_12,
										GPIO_MODE_OUTPUT,
										GPIO_PUPD_PULLUP,
										HAL::gd32f4::OutputConfig<GPIO_OTYPE_PP,
																  GPIO_OSPEED_50MHZ,
																  SET>>;

using SPI1_FLASH = HAL::gd32f4::SPI_device<SPI1_BUS, SPI1_NSS_GPIO>;

// 写入/读取的目标地址（扇区起始地址，扇区大小 4KB）
constexpr std::uint32_t TEST_ADDR = 0x000000;

extern "C"
{
int main(void)
{
	SPI1_FLASH::init();

	SEGGER_RTT_Init();
	SEGGER_RTT_WriteString(0, "GD25Q flash R/W test\r\n");

	// 1. 读 JEDEC ID，确认 SPI/Flash 链路正常
	{
		std::uint32_t jedec_id = GD25Q<SPI1_FLASH>::read_id();
		SEGGER_RTT_printf(0, "JEDEC ID: 0x%06X\r\n", jedec_id);
	}

	// 2. 擦除扇区
	SEGGER_RTT_WriteString(0, "Erasing sector...\r\n");
	GD25Q<SPI1_FLASH>::sector_erase(TEST_ADDR);
	SEGGER_RTT_WriteString(0, "Sector erased.\r\n");

	// 3. 写入 5 字节
	std::uint8_t tx_data[5] = {'H', 'E', 'L', 'L', 'O'};
	SEGGER_RTT_printf(0, "Writing 5 bytes to addr 0x%06X...\r\n", TEST_ADDR);
	GD25Q<SPI1_FLASH>::page_write(tx_data, TEST_ADDR, sizeof(tx_data));
	SEGGER_RTT_WriteString(0, "Write done.\r\n");

	// 4. 读出
	std::uint8_t rx_data[5] = {};
	GD25Q<SPI1_FLASH>::buffer_read(rx_data, TEST_ADDR, sizeof(rx_data));

	SEGGER_RTT_printf(0, "Read back: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\r\n",
					  rx_data[0], rx_data[1], rx_data[2], rx_data[3], rx_data[4]);
	SEGGER_RTT_printf(0, "As ASCII : %c%c%c%c%c\r\n",
					  rx_data[0], rx_data[1], rx_data[2], rx_data[3], rx_data[4]);

	// 5. 校验
	bool ok = true;
	for (std::size_t i = 0; i < 5; ++i)
	{
		if (rx_data[i] != "HELLO"[i])
		{
			ok = false;
			break;
		}
	}
	SEGGER_RTT_WriteString(0, ok ? "VERIFY: PASS\r\n" : "VERIFY: FAIL\r\n");

	while (1)
	{
		for (volatile std::uint32_t i = 0; i < 1000000; ++i);
	}
}
}
