#include "gpio.h"

uint8_t gpio_init_config(GPIO *pGPIO)
{
	rcu_periph_enum periph;
	switch(pGPIO->gpio_periph)
	{
		case GPIOA:
			periph=RCU_GPIOA;
			break;
		case GPIOB:
			periph=RCU_GPIOB;
			break;
		case GPIOC:
			periph=RCU_GPIOC;
			break;
		case GPIOD:
			periph=RCU_GPIOD;
			break;
		case GPIOE:
			periph=RCU_GPIOE;
			break;
		case GPIOF:
			periph=RCU_GPIOF;
			break;
		case GPIOG:
			periph=RCU_GPIOG;
			break;
		case GPIOH:
			periph=RCU_GPIOH;
			break;
		case GPIOI:
			periph=RCU_GPIOI;
			break;
	}
	rcu_periph_clock_enable(periph);
	gpio_mode_set(pGPIO->gpio_periph, pGPIO->mode, pGPIO->pull_up_down, pGPIO->pin);
	if(pGPIO->mode==GPIO_MODE_OUTPUT)
	{
		gpio_output_options_set(pGPIO->gpio_periph, pGPIO->otype, pGPIO->speed, pGPIO->pin);
		gpio_bit_write(pGPIO->gpio_periph, pGPIO->pin, pGPIO->bit_value);
	}
	if(pGPIO->mode==GPIO_MODE_AF){gpio_af_set(pGPIO->gpio_periph, pGPIO->alt_func_num, pGPIO->pin);}
	return 0;
}
