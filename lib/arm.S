/**
 * @file   arm.S
 *
 * @brief  assembly routines to manipulate the ARM processing environment in C
 *
 * @date   July 30 2015
 * @author Aaron Reyes <areyes@andrew.cmu.edu>
 */

#include <psr.h>

.section ".text"


.global delay_cycles
delay_cycles:
  subs r0, r0, #1
  bne  delay_cycles
  mov  pc, lr


.global read_cpsr
read_cpsr:
  mrs r0, cpsr
  mov pc, lr


.global write_cpsr
write_cpsr:
  msr cpsr, r0
  mov pc, lr


.global disable_interrupts
disable_interrupts:
  mrs r0, cpsr
  ldr r1, =(PSR_ABORT | PSR_IRQ | PSR_FIQ)
  orr r0, r0, r1
  msr cpsr, r0
  mov pc, lr


.global enable_interrupts
enable_interrupts:
  mrs r0, cpsr
  ldr r1, =(PSR_ABORT | PSR_IRQ | PSR_FIQ)
  bic r0, r0, r1
  msr cpsr, r0
  mov pc, lr
