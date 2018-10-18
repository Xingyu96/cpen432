/**
 * @file   kstdarg.h
 *
 * @brief  KERNEL standard argument defines for variable argument functions
 *
 * @date   July 28 2015
 * @author Aaron Reyes <areyes@andrew.cmu.edu>
 */

#ifndef _KSTDARG_H_
#define _KSTDARG_H_

/*
 * this isn't pretty, but its the cost of using a GNU toolchain and
 * not writing a kernel in assembly alone. We must use compiler magic
 * to manage variable argument functions passed via the stack in C.
 */
#ifdef __GNUC__
  typedef __builtin_va_list va_list;
  #define va_start(v,l) __builtin_va_start(v,l)
  #define va_end(v)     __builtin_va_end(v)
  #define va_arg(v,l)   __builtin_va_arg(v,l)
#else
  #error "Compiler is not GNU and cannot use __builtin_va_list for stdarg.h"
#endif

#endif /* _KSTDARG_H_ */