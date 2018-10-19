/**
 * @file   arm.h
 *
 * @brief  C helper routines for interaction with low level ARM stuff
 *
 * @date   August 11 2015
 * @author Aaron Reyes <areyes@andrew.cmu.edu>
 */

#ifndef _ARM_H_
#define _ARM_H_

#include <kstdint.h>

/**
 * @brief hangs the processor but still allows for interrupts to occur
 */
void hang(void);

/**
 * @brief Loop 'count' times in a way that the compiler won't optimize away
 *
 * @param count number of cycles to delay for
 */
void delay_cycles(uint32_t count);

/**
 * @brief reads the current CPSR value
 * @return the CPSR value
 */
uint32_t read_cpsr(void);

/**
 * @brief writes the given value to the cpsr
 *
 * @param cpsr the value to write
 */
void write_cpsr(uint32_t cpsr);

/**
 * @brief enables IRQ, FIQ and DATA_ABORT interrupts
 */
void enable_interrupts(void);

/**
 * @brief disables IRQ, FIQ and DATA_ABORT interrupts
 */
void disable_interrupts(void);

#endif /* _ARM_H_ */