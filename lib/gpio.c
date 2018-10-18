/**
 * @file   gpio.c
 *
 * @brief  Lower level definitions for accessing gpio on raspberry pi
 *
 * @date   July 15 2015
 * @author Aaron Reyes <areyes@andrew.cmu.edu>
 */

#include <kstdint.h>
#include <BCM2836_2.h>

/** @brief base of GPIO in memory mapped IO on pi
 * see page 89 onward in BCM2835 peripherals for info about MMIO for GPIO */
#define GPIO_BASE (MMIO_BASE_PHYSICAL + 0x200000)

/* GPIO MMIO control/status register offsets from GPIO_BASE */
/** @brief GPIO Function Select 0 */
#define GPIO_REG_GPFSEL0    0
/** @brief GPIO Function Select 1 */
#define GPIO_REG_GPFSEL1    1
/** @brief GPIO Function Select 2 */
#define GPIO_REG_GPFSEL2    2
/** @brief GPIO Function Select 3 */
#define GPIO_REG_GPFSEL3    3
/** @brief GPIO Function Select 4 */
#define GPIO_REG_GPFSEL4    4
/** @brief GPIO Function Select 5 */
#define GPIO_REG_GPFSEL5    5
/** @brief GPIO Pin Output Set 0 */
#define GPIO_REG_GPSET0     7
/** @brief GPIO Pin Output Set 1 */
#define GPIO_REG_GPSET1     8
/** @brief GPIO Pin Output Clear 0 */
#define GPIO_REG_GPCLR0     10
/** @brief GPIO Pin Output Clear 1 */
#define GPIO_REG_GPCLR1     11
/** @brief GPIO Pin Level 0 */
#define GPIO_REG_GPLEV0     13
/** @brief GPIO Pin Level 1 */
#define GPIO_REG_GPLEV1     14
/** @brief GPIO Pin Event Detect Status 0 */
#define GPIO_REG_GPEDS0     16
/** @brief GPIO Pin Event Detect Status 1 */
#define GPIO_REG_GPEDS1     17
/** @brief GPIO Pin Rising Edge Detect Enable 0 */
#define GPIO_REG_GPREN0     19
/** @brief GPIO Pin Rising Edge Detect Enable 1 */
#define GPIO_REG_GPREN1     20
/** @brief GPIO Pin Falling Edge Detect Enable 0 */
#define GPIO_REG_GPFEN0     22
/** @brief GPIO Pin Falling Edge Detect Enable 1 */
#define GPIO_REG_GPFEN1     23
/** @brief GPIO Pin High Detect Enable 0 */
#define GPIO_REG_GPHEN0     25
/** @brief GPIO Pin High Detect Enable 1 */
#define GPIO_REG_GPHEN1     26
/** @brief GPIO Pin Low Detect Enable 0 */
#define GPIO_REG_GPLEN0     28
/** @brief GPIO Pin Low Detect Enable 1 */
#define GPIO_REG_GPLEN1     29
/** @brief GPIO Pin Async. Rising Edge Detect 0 */
#define GPIO_REG_GPAREN0    31
/** @brief GPIO Pin Async. Rising Edge Detect 1 */
#define GPIO_REG_GPAREN1    32
/** @brief GPIO Pin Async. Falling Edge Detect 0 */
#define GPIO_REG_GPAFEN0    34
/** @brief GPIO Pin Async. Falling Edge Detect 1 */
#define GPIO_REG_GPAFEN1    35
/** @brief GPIO Pin Pull-up/down Enable */
#define GPIO_REG_GPPUD      37
/** @brief GPIO Pin Pull-up/down Enable Clock 0 */
#define GPIO_REG_GPPUDCLK0  38
/** @brief GPIO Pin Pull-up/down Enable Clock 1 */
#define GPIO_REG_GPPUDCLK1  39

/** @brief GPIO register set */
static volatile uint32_t* gpio = (volatile uint32_t*)GPIO_BASE;


void gpio_config(uint8_t pin, uint8_t fun) {
  if ((pin > 53) || (fun > 7)) {
    return;
  }
  // get the offset into MM GPIO
  uint32_t reg = pin / 10;
  // get contents of correct GPIO_REG_GPFSEL register
  uint32_t config = gpio[reg];
  // get the bit offset into the GPIO_REG_GPFSEL register
  uint32_t offset = (pin % 10) * 3;
  config &= ~(0x7 << offset);
  config |= (fun << offset);
  gpio[reg] = config;
}


void gpio_set(uint8_t pin) {
  if (pin > 53) {
    return;
  }
  if (pin > 31) {
    gpio[GPIO_REG_GPSET1] = (1 << (pin - 32));
  } else {
    gpio[GPIO_REG_GPSET0] = (1 << pin);
  }
}


void gpio_clr(uint8_t pin) {
  if (pin > 53) {
    return;
  }
  if (pin > 31) {
    gpio[GPIO_REG_GPCLR1] = (1 << (pin - 32));
  } else {
    gpio[GPIO_REG_GPCLR0] = (1 << pin);
  }
}


void gpio_set_pull(uint8_t pin, uint8_t state) {
  if ((pin > 53) || (state > 2)) {
    return;
  }
  // see pg 101 in BCM2835 peripherals for info
  gpio[GPIO_REG_GPPUD] = state;
  DelayLoop(150);
  if (pin > 31) {
    gpio[GPIO_REG_GPPUDCLK1] = (1 << (pin - 32));
    DelayLoop(150);
    gpio[GPIO_REG_GPPUDCLK1] = 0; // force change to take effect
  } else {
    gpio[GPIO_REG_GPPUDCLK0] = (1 << pin);
    DelayLoop(150);
    gpio[GPIO_REG_GPPUDCLK0] = 0; // force change to take effect
  }
}
