#ifndef LED_H
#define LED_H

#include "gpio.h"

/*
	\brief      set GPIO mode
		\param[in]  gpio_periph: GPIO port
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C,D,E,F,G,H,I)
		\param[in]  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
			\param[in]  LitState: state of LED while lit
                one or more parameters can be selected which are shown as below:
      \arg        SET, RESTE
		\param[in]  InitState: initial state of LED pin
                one or more parameters can be selected which are shown as below:
      \arg        SET, RESTE
*/
typedef struct
{
	uint32_t gpio_periph;
	uint32_t pin;
	FlagStatus LitState;
	FlagStatus InitState;
}LED;//todo:const

void led_init_config(LED *pLED);
void led_on(LED *pLED);
void led_off(LED *pLED);
void led_toggle(LED *pLED);

#endif