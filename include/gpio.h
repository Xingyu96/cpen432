/**
 * @file   gpio.h
 *
 * @brief  Lower level definitions for accessing gpio on raspberry pi
 *
 * @date   July 15 2015
 * @author Aaron Reyes <areyes@andrew.cmu.edu>
 */

#ifndef _GPIO_H_
#define _GPIO_H_

#include <kstdint.h>

/*
 * GPIO MMIO function select types for gpio_config(pin, fun)
 */

/** @brief GPIO Function Select INPUT */
#define GPIO_FUN_INPUT  0
/** @brief GPIO Function Select OUTPUT */
#define GPIO_FUN_OUTPUT 1
/** @brief GPIO Function Select 0 */
#define GPIO_FUN_ALT0   4
/** @brief GPIO Function Select 1 */
#define GPIO_FUN_ALT1   5
/** @brief GPIO Function Select 2 */
#define GPIO_FUN_ALT2   6
/** @brief GPIO Function Select 3 */
#define GPIO_FUN_ALT3   7
/** @brief GPIO Function Select 4 */
#define GPIO_FUN_ALT4   3
/** @brief GPIO Function Select 5 */
#define GPIO_FUN_ALT5   2

/*
 * pull up/down/disable states for gpio_set_pull(pin, state)
 */

/** @brief GPIO pull up */
#define GPIO_PULL_UP      2
/** @brief GPIO pull down */
#define GPIO_PULL_DOWN    1
/** @brief GPIO pull disable */
#define GPIO_PULL_DISABLE 0

/** @brief configures a pin for a given functionality.
 *
 *  See BCM2835 peripherals pg 102 - 103 for various alternate
 *  functions of GPIO pins.
 *
 *  @param pin the pin number to configure (0 to 53 on pi)
 *  @param fun the function type (see defines)
 */
void gpio_config(uint8_t pin, uint8_t fun);

/** @brief sets a given GPIO pin high.
 *
 *  @param pin the pin number to set (0 to 53 on pi)
 */
void gpio_set(uint8_t pin);

/** @brief sets a given GPIO pin low.
 *
 *  @param pin the pin number to clear (0 to 53 on pi)
 */
void gpio_clr(uint8_t pin);

/** @brief configures a given pin to be pulled high, low or have pull
 *         up/down disabled.
 *
 *  @param pin the pin number to configure (0 to 53 on pi)
 *  @param state should be GPIO_PULL_* from defines.
 */
void gpio_set_pull(uint8_t pin, uint8_t state);

#endif /* _GPIO_H_ */