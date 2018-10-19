/*
 * @file   supervisor.S
 *
 * @brief  
 *
 * @date   
 * @author 
 */

.global irq_asm_handler
.global irq_c_handler

// When an exception is generated, the processor takes care of the following:
// 1. Copy CPSR into SPSR for the mode in which exception is to be handler
// 2. Changes CPSR mode bits in order to:
//    - Change to appropriate mode for exception and map in banked register
//    - Disable interrupts (In this case, disable further IRQs)
// 3. Set LR_IRQ to the return address
// 4. Set PC to the vector address for the exception
//
// Additionally, ARM processors use a pipeline with at least a fetch, decode,
// and execute stage. There is one instruction in each pipeline staged at any
// given time. PC points to the instruction being fetched, (PC - 4) points to
// the instruction being decoded, and (PC - 8) points to the instruction being
// executed. 
//
// If an exception occurs, the value stored in LR_MODE is (PC - 4)
//
// An IRQ exception occurs AFTER the execeution of an instruction, which means
// an IRQ exception is generated AFTER PC has been updated. The instruction to
// execute after the exception is at (PC -8) instead of (PC - 4), so we need to
// subtract 4 bytes from LR_MODE = PC -4.

irq_asm_handler:
    // Make LR point to the instruction we should return to
    subs    lr, lr, #4

    // Set the stack pointer to the top of the stack
    ldr     sp, =__irq_stack_top

    // Push registers onto stack
    stmfd   sp!, {r0-r12, lr}

    // Branch and link to the IRQ handler routine written in C
	bl      irq_c_handler

    // Restore registers from stack (Notice how PC is set to LR)
    // The ^ qualifier specifies that CPSR is restored from SPSR
    ldmfd   sp!, {r0-r12, pc}^
	
reset_asm_handler:
	b _start //reset kernel