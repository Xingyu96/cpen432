/**
 * @file   kstdint.h
 *
 * @brief  KERNEL standard integer types.
 *
 * @date   July 17 2015
 * @author Aaron Reyes <areyes@andrew.cmu.edu>
 */

#ifndef _KSTDINT_H_
#define _KSTDINT_H_

/** @brief signed 8 bit */
typedef char                int8_t;
/** @brief signed 16 bit */
typedef short               int16_t;
/** @brief signed 32 bit */
typedef int                 int32_t;
/** @brief signed 64 bit */
typedef long long           int64_t;
/** @brief unsigned 8 bit */
typedef unsigned char       uint8_t;
/** @brief unsigned 16 bit */
typedef unsigned short      uint16_t;
/** @brief unsigned 32 bit */
typedef unsigned int        uint32_t;
/** @brief unsigned 64 bit */
typedef unsigned long long  uint64_t;

/** @brief non-existant pointer */
#define NULL 0

#endif /* _KSTDINT_H_ */