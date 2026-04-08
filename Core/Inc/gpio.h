#ifndef GPIO_H
#define GPIO_H

#include "gd32f4xx_gpio.h"              // Device:StdPeripherals:GPIO
#include "gd32f4xx_rcu.h"               // Device:StdPeripherals:RCU

/*	
	\brief      set GPIO mode
		\param[in]  gpio_periph: GPIO port
					only one parameter can be selected which is shown as below:
			\arg        GPIOx(x = A,B,C,D,E,F,G,H,I)
		\param[in]  pin: GPIO pin
					one or more parameters can be selected which are shown as below:
			\arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
		\param[in]  mode: GPIO pin mode
			\arg        GPIO_MODE_INPUT: input mode
			\arg        GPIO_MODE_OUTPUT: output mode
			\arg        GPIO_MODE_AF: alternate function mode
			\arg        GPIO_MODE_ANALOG: analog mode
		\param[in]  pull_up_down: GPIO pin with pull-up or pull-down resistor
			\arg        GPIO_PUPD_NONE: floating mode, no pull-up and pull-down resistors
			\arg        GPIO_PUPD_PULLUP: with pull-up resistor
			\arg        GPIO_PUPD_PULLDOWN:with pull-down resistor
				
		\brief      set GPIO output type and speed
		\param[in]  otype: GPIO pin output mode
			\arg        GPIO_OTYPE_PP: push pull mode
			\arg        GPIO_OTYPE_OD: open drain mode
		\param[in]  speed: GPIO pin output max speed
			\arg        GPIO_OSPEED_2MHZ: output max speed 2MHz
			\arg        GPIO_OSPEED_25MHZ: output max speed 25MHz
			\arg        GPIO_OSPEED_50MHZ: output max speed 50MHz
			\arg        GPIO_OSPEED_MAX: output max speed more than 50MHz
		\param[in]  bit_value:set GPIO state
			\arg        RESET: clear the port pin
			\arg        SET: set the port pin
				
	\brief      set GPIO alternate function
		\param[in]  alt_func_num: GPIO pin af function
			\arg        GPIO_AF_0: SYSTEM
			\arg        GPIO_AF_1: TIMER0, TIMER1
			\arg        GPIO_AF_2: TIMER2, TIMER3, TIMER4
			\arg        GPIO_AF_3: TIMER7, TIMER8, TIMER9, TIMER10
			\arg        GPIO_AF_4: I2C0, I2C1, I2C2
			\arg        GPIO_AF_5: SPI0, SPI1, SPI2, SPI3, SPI4, SPI5
			\arg        GPIO_AF_6: SPI2, SPI3, SPI4
			\arg        GPIO_AF_7: USART0, USART1, USART2, SPI1, SPI2
			\arg        GPIO_AF_8: UART3, UART4, USART5, UART6, UART7
			\arg        GPIO_AF_9: CAN0, CAN1, TLI, TIMER11, TIMER12, TIMER13, I2C1, I2C2, CTC
			\arg        GPIO_AF_10: USB_FS, USB_HS
			\arg        GPIO_AF_11: ENET
			\arg        GPIO_AF_12: EXMC, SDIO, USB_HS
			\arg        GPIO_AF_13: DCI
			\arg        GPIO_AF_14: TLI
			\arg        GPIO_AF_15: EVENTOUT
*/
typedef struct
{
	uint32_t gpio_periph;
	uint32_t pin;
	uint32_t mode;
	uint32_t pull_up_down; 
	
	//当为output才配置下面的
	uint8_t otype;
	uint32_t speed;
	bit_status bit_value;
	//output End
	
	//alternate function Begin
	uint32_t alt_func_num;
	//alternate function End
}GPIO;//todo:const

uint8_t gpio_init_config(GPIO *pGPIO);

#endif
