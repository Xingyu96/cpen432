/**
 * @file   printk.h
 *
 * @brief  kernel debugging routine
 *
 * @date   submission_date
 * @author your_name <email>
 */

#ifndef _PRINTK_H_
#define _PRINTK_H_

#define MAXBUF 64
#define MAX_32 0xFFFFFFFF

#define FLIP_SIGN_32(X) (X ^ MAX_32) + 1
#define IS_NEG_32(X) (X >> 31) == 1

#define OCTAL_BASE 8
#define DECIMAL_BASE 10
#define HEXADECIMAL_BASE 16

#define END_CHAR '\0'
#define RETURN_CHAR '\n'
#define FORMAT_CHAR '%'
#define OCTAL_SPECIFIER 'o'
#define SINGEDINT_SPECIFIER 'd'
#define UNSIGNEDINT_SPECIFIER 'u'
#define HEXADECIMAL_SPECIFIER 'x'
#define CHAR_SPECIFIER 'c'
#define POINTER_SPECIFIER 'p'


/**
 * @brief A kernel printf() function for debugging the kernel
 *
 * @param format the format string
 * @return 0 on success or -1 on failure
 */
int printk(const char *format, ...);

#endif /* _PRINTK_H_ */
