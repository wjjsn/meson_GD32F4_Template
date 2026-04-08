#include "led.h"

void led_init_config(LED *pLED)
{
	GPIO LED_GPIO;
	LED_GPIO.gpio_periph=pLED->gpio_periph;
	LED_GPIO.pin=pLED->pin;
	LED_GPIO.mode=GPIO_MODE_OUTPUT;
	if(pLED->LitState==SET)
	{LED_GPIO.pull_up_down=GPIO_PUPD_PULLDOWN;LED_GPIO.otype=GPIO_OTYPE_PP;}
	else if(pLED->LitState==RESET)
	{LED_GPIO.pull_up_down=GPIO_PUPD_PULLUP;LED_GPIO.otype=GPIO_OTYPE_OD;}
	LED_GPIO.speed=GPIO_OSPEED_2MHZ;
	LED_GPIO.bit_value=pLED->InitState;
	
	gpio_init_config(&LED_GPIO);
}

void led_on(LED *pLED)
{
	if(pLED->LitState==SET){gpio_bit_set(pLED->gpio_periph, pLED->pin);}
	if(pLED->LitState==RESET){gpio_bit_reset(pLED->gpio_periph, pLED->pin);}
}

void led_off(LED *pLED)
{
	if(pLED->LitState==SET){gpio_bit_reset(pLED->gpio_periph, pLED->pin);}
	if(pLED->LitState==RESET){gpio_bit_set(pLED->gpio_periph, pLED->pin);}
}

void led_toggle(LED *pLED)
{
	gpio_bit_toggle(pLED->gpio_periph, pLED->pin);
}