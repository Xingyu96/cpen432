/**
 * @file   led.c
 *
 * @brief  led realated services and functions
 *
 * @date   2018-10-18
 * @author UBC cpen432 2018w1
 */

#include <kstdint.h>
#include <gpio.h>
#include <led.h>


volatile uint32_t* gpio = (uint32_t*)GPIO_BASE;

/** @brief initialize onboard led */
void led_init(void){
	gpio[LED_GPF_SEL] |= (1 << LED_GPF_BIT);
}

/** @brief turn on onboard led */
void turn_led_on(void){
	do{ gpio[LED_GPSET] = (1 << LED_GPIO_BIT); } while(0);
}

/** @brief turn off onboard led */
void turn_led_off(void){
	do{ gpio[LED_GPCLR] = (1 << LED_GPIO_BIT); } while(0);
}
