/*
 * startup.S
 *
 * This file contains code taken from Linux:
 *	safe_svcmode_maskall macro
 *	defined in arch/arm/include/asm/assembler.h
 *	Copyright (C) 1996-2000 Russell King
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <uspienv/sysconfig.h>
#include <uspienv/psr.h>

/*
 * Helper macro to enter SVC mode cleanly and mask interrupts. reg is
 * a scratch register for the macro to overwrite.
 *
 * This macro is intended for forcing the CPU into SVC mode at boot time.
 * you cannot return to the original mode.
 */
	.macro safe_svcmode_maskall reg:req

	mrs	\reg , cpsr
	eor	\reg, \reg, #0x1A		/* test for HYP mode */
	tst	\reg, #0x1F
	bic	\reg , \reg , #0x1F		/* clear mode bits */
	orr	\reg , \reg , #0xC0 | 0x13	/* mask IRQ/FIQ bits and set SVC mode */
	bne	1f				/* branch if not HYP mode */
	orr	\reg, \reg, #0x100		/* mask Abort bit */
	adr	lr, 2f
	msr	spsr_cxsf, \reg
	.word	0xE12EF30E			/* msr ELR_hyp, lr */
	.word	0xE160006E			/* eret */
1:	msr	cpsr_c, \reg
2:

	.endm

	.text

	.globl	_start
_start:

    ldr pc, _reset_h
    ldr pc, _undefined_instruction_vector_h
    ldr pc, _software_interrupt_vector_h
    ldr pc, _prefetch_abort_vector_h
    ldr pc, _data_abort_vector_h
    ldr pc, _unused_handler_h
    ldr pc, _irq_asm_handler_h
    ldr pc, _fast_interrupt_vector_h

_reset_h:                           .word   _reset_
_undefined_instruction_vector_h:    .word   undefined_instruction_vector
_software_interrupt_vector_h:       .word   software_interrupt_vector
_prefetch_abort_vector_h:           .word   prefetch_abort_vector
_data_abort_vector_h:               .word   data_abort_vector
_unused_handler_h:                  .word   _reset_
_irq_asm_handler_h:            		  .word   irq_asm_handler
_fast_interrupt_vector_h:           .word   fast_interrupt_vector

_reset_:
  // We enter execution in supervisor mode 
  mov     r0, #0x8000
  mov     r1, #0x0000
  ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
  stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
  ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
  stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
  // setup a default irq stack
  mrs r0, cpsr                          // stash cpsr so we can go back
  msr cpsr_c, #(PSR_MODE_IRQ | PSR_IRQ | PSR_FIQ) // jump to IRQ
  msr cpsr_c, #(PSR_MODE_IRQ | PSR_IRQ | PSR_FIQ) // jump to IRQ
  ldr sp, =__irq_stack_top              // setup the irq stack
  msr cpsr_c, r0                        // jump back to the original mode
  // setup the stack to start where the kernel is loaded and grow DOWN as needed
  ldr sp, =__svc_stack_top
  ldr r0, =__bss_start
  ldr r1, =__bss_end
  mov r2, #0
  mov r3, #0
  mov r4, #0
  mov r5, #0
  // clear out bss
bss_loop:
  stmia r0!, {r2-r5}
  cmp r0, r1
  blo bss_loop
  // no need to initialize .data or .rodata since rpi runs out of ram entirely
  // call kernel_main()
  bl  main

  // stop a bad kernel
.global hang
hang:
  wfi
  b hang

undefined_instruction_vector:
  b undefined_instruction_vector

software_interrupt_vector:
  b software_interrupt_vector

prefetch_abort_vector:
  subs pc, lr, #4

data_abort_vector:
  bx lr

fast_interrupt_vector:
  subs pc, lr, #4

#if RASPPI != 1
	
	.globl	_start_secondary
_start_secondary:
	dsb
1:	wfi
	b	1b

#endif

/* End */
