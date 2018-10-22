#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include <timer.h>
#include <printk.h>
#include <led.h>
#include <uart.h>

#define RPI_INTERRUPT_CONTROLLER_BASE   (MMIO_BASE_PHYSICAL+ 0xB200)
#define RPI_INTERRUPT_CONTROLLER_ENABLE_IRQS_1_REG    		(volatile uint32_t *)(RPI_INTERRUPT_CONTROLLER_BASE + 0x10)
#define RPI_INTERRUPT_CONTROLLER_ENABLE_BASIC_IRQS_REG		(volatile uint32_t *)(RPI_INTERRUPT_CONTROLLER_BASE + 0x18)
#define RPI_INTERRUPT_CONTROLLER_DISABLE_IRQS_1_REG			(volatile uint32_t *)(RPI_INTERRUPT_CONTROLLER_BASE + 0x1C)
#define RPI_INTERRUPT_CONTROLLER_DISABLE_BASIC_IRQS_REG		(volatile uint32_t *)(RPI_INTERRUPT_CONTROLLER_BASE + 0x24)

#define RPI_AUX_INT_IRQ   	(1 << 29)

#define RPI_BASIC_ARM_TIMER_IRQ         (1 << 0)
#define RPI_BASIC_ARM_MAILBOX_IRQ       (1 << 1)
#define RPI_BASIC_ARM_DOORBELL_0_IRQ    (1 << 2)
#define RPI_BASIC_ARM_DOORBELL_1_IRQ    (1 << 3)
#define RPI_BASIC_GPU_0_HALTED_IRQ      (1 << 4)
#define RPI_BASIC_GPU_1_HALTED_IRQ      (1 << 5)
#define RPI_BASIC_ACCESS_ERROR_1_IRQ    (1 << 6)
#define RPI_BASIC_ACCESS_ERROR_0_IRQ    (1 << 7)

#define KEYBOARD_BUFFER_SIZE			19

#define RESET				'r'
#define PING				'p'
#define BAN					'b'
#define UNBAN1				'u'
#define UNBAN2				'b'
#define FREQ1				'1'
#define FREQ2				'2'
#define FREQ3				'3'
#define EOT					4
#define BACKSPACE			127

#define FREQ1_CYCLE			500000
#define FREQ2_CYCLE			2000000
#define FREQ3_CYCLE			5000000

#define TIMER_IRQ_FREQ		1000000



void irq_c_handler(void);
void enable_timer_irq(void);
void enable_uart_irq(void);
int stop_blinking(void);
int start_blinking_with_freq(int freq);
void handle_keyboard_input(uint8_t received_byte);
void handle_eot_command(void);
void handle_local_command(void);
void handle_remote_command(void);


#endif
