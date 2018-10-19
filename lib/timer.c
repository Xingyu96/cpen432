/**
 * @file   timer.c
 *
 * @brief  basic routine implementations for interacting with the ARM timer.
 *
 * @date   2018-10-18
 * @author UBC cpen432 2018w1
 */

#include <timer.h>
#include <printk.h>



/**
 * @brief Reads the timer register and returns the time elapsed
 * 
 * @return Time elapsed in miliseconds
 */
uint32_t timer_latch(void) {
  
  // Read timer value
  uint32_t ms_elapsed = (1000000 - *ARM_TIMER_VALUE_REG) / TICKS_PER_MSEC_PREDIVIDER_250;

  return ms_elapsed;
  
}

// PA2 code below
/**
 * @brief Configures the arm timer to start running with the given frequency. 
          The Timer should run in 32 bit mode, with a prescaler of 1, and with 
 *		  interrupts enabled.
 *
 * @param freq  frequency at which to run the timer
 */
void timer_start(int freq){

  // Configure predivider to divide clock by 250
  *ARM_TIMER_PREDIVIDER_REG = PREDIVIDER_250;

  // Enable 32-bit timer
  *ARM_TIMER_CONTROL_REG |= (1 << ARM_TIMER_CONTROL_COUNTER_RESOLUTION_POS);

  // Enable the timer
  *ARM_TIMER_CONTROL_REG |= (1 << ARM_TIMER_CONTROL_TIMER_ENABLE_POS);

  // Enable timer interrupt
  *ARM_TIMER_CONTROL_REG |= (1 << ARM_TIMER_CONTROL_INTERRUPT_ENABLE_POS);

  // Load the initial timer value based 
  *ARM_TIMER_LOAD_REG = freq;

}

/**
 * @brief Sets the timer frequency
 *
 * @param freq  frequency at which to run the timer
 */
void timer_set_freq(int freq){
	*ARM_TIMER_LOAD_REG = freq;
}

/**
 * @brief Disables timer IRQs
 */
void timer_stop(void){
  // Disable the timer
  *ARM_TIMER_CONTROL_REG &= ~(1 << ARM_TIMER_CONTROL_TIMER_ENABLE_POS);
  
  // Disable timer interrupt
  *ARM_TIMER_CONTROL_REG &= ~(1 << ARM_TIMER_CONTROL_INTERRUPT_ENABLE_POS);
}

/**
 * @brief Determines if there is currently a pending timer interrupt
 *
 * @return 1 if timer interrupt is pending, 0 if not.
 */
int timer_is_pending(void){
	return (int)(*ARM_TIMER_RAW_IRQ_REG & 1);
}

/**
 * @brief Should be called to acknowledge that a timer interrupt occurred and 
 *          has been appropriately handled. Will clear the pending bit in the 
 *		    timer's MMIO registers.
 */
void timer_clear_pending(void){
	*ARM_TIMER_IRQ_CLEAR_REG = ARM_TIMER_IRQ_CLEAR_SIGNAL;
}
