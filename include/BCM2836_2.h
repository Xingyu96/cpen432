/**
 * @file   BCM2836.h
 *
 * @brief  Constants specific to the Raspberry Pi 2
 *
 * @date   July 15 2015
 * @author Aaron Reyes <areyes@andrew.cmu.edu>
 */

#ifndef _BCM2836_H_
#define _BCM2836_H_

/** @brief the physical address for the start of MMIO */
#define MMIO_BASE_PHYSICAL 0x3F000000

/** @brief user entry point after kernel is done booting */
#define USER_BASE_PHYSICAL 0x00300000

/** @brief the kernel entry point after the GPU is done booting */
#define KERN_BASE_PHYSICAL 0x00008000

/** @brief the system clock frequency (Hz) */
#define SYSTEM_CLOCK_FREQ  250000000U

#endif /* _BCM2836_H_ */