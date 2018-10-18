/**
 * @file   uart.h
 *
 * @brief  hardware interaction for UART on pi
 *
 * @date   
 * @author name <email>
 */

#ifndef _UART_H_
#define _UART_H_

#include <kstdint.h>
#include <BCM2836_2.h>
#include <gpio.h>

#define AUX_ENABLES_REG   (volatile uint32_t *)(MMIO_BASE_PHYSICAL + 0x215004)
#define AUX_MU_IO_REG     (volatile uint32_t *)(MMIO_BASE_PHYSICAL + 0x215040)
#define AUX_MU_IER_REG    (volatile uint32_t *)(MMIO_BASE_PHYSICAL + 0x215044)
#define AUX_MU_IIR_REG    (volatile uint32_t *)(MMIO_BASE_PHYSICAL + 0x215048)
#define AUX_MU_LSR_REG    (volatile uint32_t *)(MMIO_BASE_PHYSICAL + 0x215054)
#define AUX_MU_BAUD_REG   (volatile uint32_t *)(MMIO_BASE_PHYSICAL + 0x215068)
#define AUX_IRQ			  (volatile uint32_t *)(MMIO_BASE_PHYSICAL + 0x215000)

#define AUX_ENABLES_REG_UART_ENABLE   0
#define AUX_MU_IER_REG_RX_ENABLE      0
#define AUX_MU_IER_REG_TX_ENABLE      1
#define AUX_MU_IER_REG_RX_FIFO_CLEAR  1
#define AUX_MU_IER_REG_TX_FIFO_CLEAR  2

/** @brief AUX_MU_IER_REG interrupt id register bits  */
#define INTERRUPT_ID_HIGH_BIT		(1 << 2)
#define INTERRUPT_ID_LOW_BIT		(1 << 1)
#define INTERRUPT_PENDING_BIT		(1 << 0)
#define CLEAR_RECEIVE_FIFO_BIT	(1 << 1)

#define AUX_MU_IIR_INT_ID_MASK (uint32_t) 0x6
#define AUX_MU_IIR_RX_INT_ID   (uint32_t) 0x2

/** @brief interrupt id status */
#define RECEIVER_VALID_BYTE		4 // 0b100

/** @brief uart baudrate **/
#define BAUDRATE            115200

/** @brief GPIO UART RX pin */
#define RX_PIN 	15

/** @brief GPIO UART TX pin */
#define TX_PIN 	14
/**
 * @brief initializes UART to 115200 baud in 8-bit mode
 */
void uart_init(void);

/**
 * @brief closes UART
 */
void uart_close(void);

/**
 * @brief sends a byte over UART
 *
 * @param byte the byte to send
 */
void uart_put_byte(uint8_t byte);

/**
 * @brief reads a byte over UART
 *
 * @return the byte received
 */
uint8_t uart_get_byte(void);

uint32_t uart_get_interrupt_id();

#endif /* _UART_H_ */
