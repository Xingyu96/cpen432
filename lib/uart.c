/**
 * @file   uart.c
 *
 * @brief  lower level hardware interactions for uart on pi
 *
 * @date   2018-10-18
 * @author UBC cpen432 2018w1
 */

#include <kstdint.h>
#include <uart.h>
#include <BCM2836_2.h>


/**
 * @brief initializes UART to 115200 baud in 8-bit mode
 */
void uart_init(void) {
  // Configure GPIOs before enabling Mini UART using AUXENB
  // Pg. 10: "[...] the correct GPIO function mode is selected before enabling the mini UART"

  // Disable pull-up/down
  gpio_set_pull(RX_PIN, GPIO_PULL_DISABLE);
  gpio_set_pull(TX_PIN, GPIO_PULL_DISABLE);

  // Select UART1 alternative function (TXD1/RXD1)
  gpio_config(RX_PIN, GPIO_FUN_ALT5);
  gpio_config(TX_PIN, GPIO_FUN_ALT5);

  // Enable Mini-UART to get R/W to registers
  *AUX_ENABLES_REG |= (1 << AUX_ENABLES_REG_UART_ENABLE);

  // Enable RX interrupt
  *AUX_MU_IER_REG |= (1 << AUX_MU_IER_REG_RX_ENABLE);
  
  // Disable TX interrupt
  *AUX_MU_IER_REG |= (0 << AUX_MU_IER_REG_TX_ENABLE);

  // Configure 115200 baudrate 
  *AUX_MU_BAUD_REG = (SYSTEM_CLOCK_FREQ / (8 * BAUDRATE)) - 1;
}

/**
 * @brief closes UART
 */
void uart_close(void) {
  
  // Disable Mini-UART
  *AUX_ENABLES_REG |= (0 << AUX_ENABLES_REG_UART_ENABLE);

}

/**
 * @brief sends a byte over UART
 *
 * @param byte the byte to send
 */
void uart_put_byte(uint8_t byte) {

  while(1)
  {
    if(*AUX_MU_LSR_REG & 0x20)
    {
      break;
    }
  }

  *AUX_MU_IO_REG = byte;
}

/**
 * @brief reads a byte over UART
 *
 * @return the byte received
 */
uint8_t uart_get_byte(void) {

	uint8_t data = (uint8_t)*AUX_MU_IO_REG;
	
	return data;
}

/**
 * @brief Check UART interrupt ID
 * 
 * @return The UART interrupt ID
 */
uint32_t uart_get_interrupt_id() {
    return (*AUX_MU_IIR_REG & AUX_MU_IIR_INT_ID_MASK) >> 1;
}
