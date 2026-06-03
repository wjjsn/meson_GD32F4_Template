#include "Platform/gd32f4.hpp"
#include "hal.hpp"
#include "SEGGER_RTT.h"

using USART0_TX = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOA_ADDR,
									GPIO_PIN_9,
									GPIO_MODE_AF,
									GPIO_PUPD_PULLUP,
									HAL::gd32f4::AFConfig<GPIO_AF_7>>;

using USART0_RX = HAL::gd32f4::GPIO<HAL::gd32f4::registers::GPIOA_ADDR,
									GPIO_PIN_10,
									GPIO_MODE_AF,
									GPIO_PUPD_NONE,
									HAL::gd32f4::AFConfig<GPIO_AF_7>>;

using USART0 = HAL::gd32f4::USART_Device<USART0_TX,
										 USART0_RX,
										 HAL::gd32f4::registers::USART0_ADDR,
										 115200,
										 USART_PM_NONE,
										 USART_WL_8BIT,
										 USART_STB_1BIT>;
#include "chry_ringbuffer.hpp"

chry_ringbuffer_t ctx_uart1_buffer;
using uart1_buffer = Cherry_RingBuffer<&ctx_uart1_buffer, 128>;

extern "C"
{
int main(void)
{
	SEGGER_RTT_Init();
	SEGGER_RTT_WriteString(0, "USART example started, RTT ready\r\n");

	USART0::init();
	USART0::enable_it(0, 0);

	constexpr uint8_t msg[] = "Hello USART\r\n";

	while (1)
    {
		USART0::transmit(msg, sizeof(msg) - 1, 0);
		SEGGER_RTT_WriteString(0, "USART example started, RTT ready\r\n");
		for (uint32_t i = 0; i < 10000000; ++i);
	}
}

void USART0_IRQHandler()
{
	if (usart_interrupt_flag_get(HAL::gd32f4::registers::USART0_ADDR, USART_INT_FLAG_RBNE) == SET) // rx接收中断
	{
		const uint8_t byte = usart_data_receive(HAL::gd32f4::registers::USART0_ADDR);
		uart1_buffer::write_byte(byte); // 将接收到的数据写入环形缓冲区
		// 通过 RTT 输出日志
		SEGGER_RTT_printf(0, "RX: 0x%02X ('%c')\r\n", byte, (byte >= 32 && byte < 127) ? byte : '.');
	}
}
}
