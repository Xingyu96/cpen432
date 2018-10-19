/**
 * @file   psr.h
 *
 * @brief  ARM architecture specific defines for interacting with CPSR/SPSR.
 *         NOTE: this file is safe to be included for the GNU assembler.
 *
 * @date   July 30 2015
 * @author Aaron Reyes <areyes@andrew.cmu.edu>
 */

#ifndef _PSR_H_
#define _PSR_H_

/*
 * masks for the CPSR/SPSR flags
 */

/** @brief mask for CPSR negative flag */
#define PSR_NEG      0x80000000
/** @brief mask for CPSR zero flag */
#define PSR_ZERO     0x40000000
/** @brief mask for CPSR carry flag */
#define PSR_CARRY    0x20000000
/** @brief mask for CPSR overflow flag */
#define PSR_OFLW     0x10000000
/** @brief mask for CPSR abort interrupt flag */
#define PSR_ABORT    0x00000100
/** @brief mask for CPSR IRQ interrupt flag */
#define PSR_IRQ      0x00000080
/** @brief mask for CPSR FIQ interrupt flag */
#define PSR_FIQ      0x00000040
/** @brief mask for CPSR mode flag */
#define PSR_MODE     0x0000001f

/*
 * CPSR/SPSR mode constants
 */

/** @brief CPSR user mode */
#define PSR_MODE_USR  0x10
/** @brief CPSR FIQ mode */
#define PSR_MODE_FIQ  0x11
/** @brief CPSR IRQ mode */
#define PSR_MODE_IRQ  0x12
/** @brief CPSR supervisor mode */
#define PSR_MODE_SVC  0x13
/** @brief CPSR abort mode */
#define PSR_MODE_ABT  0x17
/** @brief CPSR undefined mode */
#define PSR_MODE_UND  0x1b
/** @brief CPSR system mode */
#define PSR_MODE_SYS  0x1F

#endif /* _PSR_H_ */