#ifndef _LED_H_
#define _LED_H_

#include <BCM2836_2.h>

/** @brief base of GPIO in memory mapped IO on pi
 * see page 89 onward in BCM2835 peripherals for info about MMIO for GPIO */
#define GPIO_BASE		(MMIO_BASE_PHYSICAL + 0x200000)
#define GPIO 			(volatile uint32_t *) (GPIO_BASE)

#define LED_GPF_SEL		4
#define LED_GPF_BIT		21
#define	LED_GPIO_BIT	15

#define LED_GPSET       8
#define LED_GPCLR       11
#define ON				1
#define OFF				0

void led_init(void);
void turn_led_on(void);
void turn_led_off(void);

#endif
