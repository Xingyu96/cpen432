/**
 * @file printk.c
 *
 * @brief printk() implementation for KERNEL using UART
 *
 * @date   2018-10-18
 * @author UBC cpen432 2018w1
 */

#include <kstdint.h>
#include <kstdarg.h>
#include <printk.h>
#include <uart.h>

/**
 * static array of digits for use in printnum(s)
 */
static char digits[] = "0123456789abcdef";

/**
 * @brief Prints a number to UART
 *
 * @param Number type: base 8, 10, 16
 * @param Number to be printed 
 * @param Signed or Unsigned Number type
 */
static void printnumk(uint8_t base, uint32_t num, char signedness) { 
    uint8_t pos[MAXBUF];
    int i = 0;

  // Prepend values as needed depending on the base
  switch (base)
  {
    case 8:
      uart_put_byte('0');
      break;
    case 10:
      if (signedness =='s' && IS_NEG_32(num)) {
        
        uart_put_byte('-');           // Prepend a negative sign
        num = FLIP_SIGN_32(num); // Invert the bits and add 1 to get abs(num)
      }
      break;
    case 16:
      uart_put_byte('0');
      uart_put_byte('x');
      break;
    default:
      break;
  }
  
  if (num == 0)
  {
    uart_put_byte('0');
    return;
  }

  // Starting with the least significant digit, convert digits to the correct 
  // base and store them in a buffer
  while(num != 0)
  {
    pos[i++] = num % base;
    num = num / base;
  }

  // Starting with the most significant digit, transmit the converted digits 
  // over UART
  while(i > 0)
  {
    uart_put_byte(digits[pos[--i]]);
  }
} 


/**
 * @brief A kernel printf() function for debugging the kernel
 *
 * @param format the format string
 * @return 0 on success or -1 on failure
 */
int printk(const char *fmt, ...) {

    va_list args;
    va_start(args, fmt);
    uint8_t error_flag = 0;

  while (*fmt != END_CHAR)
  { 
    if (*fmt == RETURN_CHAR)
    {
      // Print CR + LF
      uart_put_byte('\n');
      uart_put_byte('\r'); 
      fmt = fmt + 1;
    }
    else if (*fmt != FORMAT_CHAR)
    { 
      uart_put_byte(*fmt);
      fmt = fmt + 1; 
    }
    else
    {
      switch(*(fmt + 1))
      {
        case OCTAL_SPECIFIER: 
        {
          // In ANSI C, %x is unsigned
          uint32_t arg = va_arg(args, uint32_t);
          printnumk(OCTAL_BASE, arg, 'u');
          break;
        }
        case SINGEDINT_SPECIFIER:
        {
          uint32_t arg  = va_arg(args, int);
          printnumk(DECIMAL_BASE, arg, 's');
          break;
        }
        case UNSIGNEDINT_SPECIFIER:
        {
          uint32_t arg = va_arg(args, uint32_t);
          printnumk(DECIMAL_BASE, arg, 'u');
          break;
        }
        case HEXADECIMAL_SPECIFIER:
        {
          // In ANSI C, %x is unsigned
          uint32_t arg = va_arg(args, uint32_t);
          printnumk(HEXADECIMAL_BASE, arg, 'u');
          break;
        }
        case CHAR_SPECIFIER:
        {
          // Note that the C standard sayswe can't pass char type to va_arg() 
          // Types that are narrower than int will be promoted to int
          uint8_t arg = (uint8_t) va_arg(args, uint32_t);
          uart_put_byte(arg);
          break;
        }
        case POINTER_SPECIFIER:
        {
          uint32_t arg = va_arg(args, uint32_t);
          printnumk(HEXADECIMAL_BASE, arg, 'u');
          break;
        }
        default:
          break;
      } 
      fmt = fmt + 2;
    }
  } 
    va_end(args);

    return error_flag;
}
