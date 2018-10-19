/**
 * @file   timer.h
 *
 * @brief  basic routines for interacting with the ARM timer.
 *
 * @date   
 * @author cpen432
 */

#ifndef _TIMER_H_
#define _TIMER_H_


#include <kstdint.h>	// to support uint32_t data types
#include <BCM2836_2.h>


#define ARM_TIMER_BASE_2              (MMIO_BASE_PHYSICAL + 0xB000)
#define ARM_TIMER_LOAD_REG          (volatile uint32_t *)(ARM_TIMER_BASE_2 + 0x400)
#define ARM_TIMER_VALUE_REG         (volatile uint32_t *)(ARM_TIMER_BASE_2 + 0x404)
#define ARM_TIMER_CONTROL_REG       (volatile uint32_t *)(ARM_TIMER_BASE_2 + 0x408)
#define ARM_TIMER_PREDIVIDER_REG    (volatile uint32_t *)(ARM_TIMER_BASE_2 + 0x41C)
#define ARM_TIMER_RAW_IRQ_REG		(volatile uint32_t *)(ARM_TIMER_BASE_2 + 0x410)
#define ARM_TIMER_IRQ_CLEAR_REG		(volatile uint32_t *)(ARM_TIMER_BASE_2 + 0x40C)


#define ARM_TIMER_LOAD_MAX_32BIT (uint32_t)(0xFFFFFFFF)


#define ARM_TIMER_CONTROL_COUNTER_RESOLUTION_POS    1 // 0: 16-bit counter
                                                      // 1: 32-bit counter
#define ARM_TIMER_CONTROL_PRESCALER_POS             2 // 00: Pre-scale is clock / 1
                                                      // 01: Pre-scale is clock / 16
                                                      // 10: Pre-scale is clock / 256
#define ARM_TIMER_CONTROL_INTERRUPT_ENABLE_POS      5 // 0: Timer interrupt disabled
                                                      // 1: Timer interrupt enabled
#define ARM_TIMER_CONTROL_TIMER_ENABLE_POS          7 // 0: Timer disabled
   		                                              // 1: Timer enabled



#define ARM_TIMER_IRQ_CLEAR_SIGNAL					0X1U // 1: writing anything will clear

#define PRESCALER_256 256U

#define MILLISECOND_PER_SECOND           1000U
#define PREDIVIDER_250 249U
#define TICKS_PER_SEC_PREDIVIDER_250    (uint32_t)(SYSTEM_CLOCK_FREQ / (PREDIVIDER_250 + 1))
#define TICKS_PER_MSEC_PREDIVIDER_250   (uint32_t)(TICKS_PER_SEC_PREDIVIDER_250 / MILLISECOND_PER_SECOND)


/**
 * @brief Configures the arm timer to start running from the starting 
 *        point with the given frequency. The Timer should run in 32 bit mode, 
 *        with a prescaler of 1
 */
void start_countdown(void);

/**
 * @brief Called to check the value of the timer.
 *
 * @return timer's decrementing value
 */
uint32_t timer_latch(void);

/**
 * @brief Configures the arm timer to start running with the given frequency. 
          The Timer should run in 32 bit mode, with a prescaler of 1, and with 
 *		  interrupts enabled.
 *
 * @param freq  frequency at which to run the timer
 */
void timer_start(int freq);

/**
 * @brief Sets the timer frequency
 *
 * @param freq  frequency at which to run the timer
 */
void timer_set_freq(int freq);

/**
 * @brief Disables timer IRQs
 */
void timer_stop(void);

/**
 * @brief Determines if there is currently a pending timer interrupt
 *
 * @return 1 if timer interrupt is pending, 0 if not.
 */
int timer_is_pending(void);

/**
 * @brief Should be called to acknowledge that a timer interrupt occurred and 
 *          has been appropriately handled. Will clear the pending bit in the 
 *		    timer's MMIO registers.
 */
void timer_clear_pending(void);

#endif /* _TIMER_H_ */
