#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "gd32f4xx.h"
#include "led.h"

LED LED1 = {GPIOE, GPIO_PIN_4, SET, RESET};
int main(void)
{
	led_init_config(&LED1);
	while (1)
	{
		led_on(&LED1);
		for (uint64_t i = 0; i < 1145141; ++i)
		{
		}
		led_off(&LED1);
		for (uint64_t i = 0; i < 1145141; ++i)
		{
		}
	}
}
