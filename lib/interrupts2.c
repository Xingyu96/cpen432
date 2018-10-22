/**
 * @file   interrupts.c
 *
 * @brief  interrupt service routine implementation
 *
 * @date   2018-10-18
 * @author UBC cpen432 2018w1
 */

#include "interrupts2.h"

/* @brief current state of led (ON/OFF)*/
static int led_state = ON;
static uint8_t keyboard_input_buffer[ KEYBOARD_BUFFER_SIZE ];
static int input_index = 0;

/**
 * @brief gets called after irq stack is saved
 * 
 */
void irq_c_handler(void) {
	//printk("in irq\n");
    // Handle Timer IRQ
    if (timer_is_pending()) {
        timer_clear_pending();
        // Toggle the LED
        if(!led_state){
            turn_led_on();
        }else{
            turn_led_off();
        }
        led_state = !led_state;
    }
    
    // Handler UART RX IRQ
    else if(*AUX_IRQ && (uart_get_interrupt_id() == AUX_MU_IIR_RX_INT_ID)){ 
    	uint8_t receiver_fifo_data = (uint8_t) uart_get_byte();	
    	handle_keyboard_input(receiver_fifo_data);
    }
    
    
}

/**
 * @brief Enable timer interrupt sources
 * 
 */
void enable_timer_irq(void) {
    // Enable timer IRQ
    *RPI_INTERRUPT_CONTROLLER_ENABLE_BASIC_IRQS_REG = RPI_BASIC_ARM_TIMER_IRQ;
}

/**
 * @brief Enable uart interrupt sources
 * 
 */
void enable_uart_irq(void) {
    // Enable Auxiliary IRQ for Mini-UART
    *RPI_INTERRUPT_CONTROLLER_ENABLE_IRQS_1_REG  = RPI_AUX_INT_IRQ;   
}

/**
 * @brief Disable timer interrupt, stop timer, turn off led
 *
 * @param freq - blinking cycle
 * @return led state is off
 */
int stop_blinking(void) {   
	
	turn_led_off();
	timer_stop();
	*RPI_INTERRUPT_CONTROLLER_DISABLE_BASIC_IRQS_REG = RPI_BASIC_ARM_TIMER_IRQ;
	
	return 0;
}

/**
 * @brief Enbale timer interrupt, start timer, turn on led
 *
 * @param freq - blinking cycle
 * @return led state is on 
 */
int start_blinking_with_freq(int freq) { 
	*RPI_INTERRUPT_CONTROLLER_ENABLE_BASIC_IRQS_REG = RPI_BASIC_ARM_TIMER_IRQ;
	timer_start(freq);
	turn_led_on();
	
    return 1;
}

/**
 * @brief Handle incoming char from keyboard
 *
 * @param received_byte 		incoming byte from uart
 */
void handle_keyboard_input(uint8_t received_byte) {
	
	// if buffer overflow - invalid command
	if(input_index == KEYBOARD_BUFFER_SIZE){ 
		input_index = 0;
		
		printk("Command too long! Please try again...\n");
	}

	// if EOT character inputed - handle command execution
	else if(received_byte == EOT){
		handle_eot_command();
		input_index = 0;
		
		printk("Execution Complete. Enter next Command...\n");
	}
	
	// if backspace entered, delete character on console
	else if(received_byte == BACKSPACE){
		input_index -= 1;
		
		printk("\b \b");
	}
	
	// else, handle character input
	else {
		keyboard_input_buffer[input_index] = received_byte;
		input_index += 1;
		
		printk("%c", received_byte);
	}
	
}

/**
 * @brief Handle eot character input
 *
 */
void handle_eot_command(void) {
	
	if(input_index > 1){
		handle_remote_command();
	} else {
		handle_local_command();
	}
}

/**
 * @brief Handle eot character input
 *
 */
void handle_local_command(void) {
	switch(keyboard_input_buffer[0]){
		case RESET: 
			printk("\nRESET!\n");
			led_state = stop_blinking();
			break;
		case FREQ1:
			printk("\nFREQ 0.5\n");
			led_state = start_blinking_with_freq(FREQ1_CYCLE);
			break;
		case FREQ2:
			printk("\nFREQ 2 \n");
			led_state = start_blinking_with_freq(FREQ2_CYCLE);
			break;
		case FREQ3:
			printk("\nFREQ 5 \n");
			led_state = start_blinking_with_freq(FREQ3_CYCLE);
			break;
		default:
			printk("\nINVALID ENTRY\n");
	}
}

/**
 * @brief Handle eot character input
 *
 */
void handle_remote_command(void) {
	
	switch(keyboard_input_buffer[0]){
		case RESET: 
			printk("\nREMOTE RESET!\n");
			break;
		case PING: 
			printk("\nREMOTE PING\n");
			break;
		case FREQ1:
			printk("\nREMOTE FREQ 0.5\n");
			break;
		case FREQ2:
			printk("\nREMOTE FREQ 2 \n");
			break;
		case FREQ3:
			printk("\nREMOTE FREQ 5 \n");
			break;
		case BAN: 
			printk("\nREMOTE BAN\n");
			break;
		case UNBAN1: 
			if(keyboard_input_buffer[1] == UNBAN2){
				printk("\nREMOTE PING\n");
			}
			break;
		default:
			printk("\nREMOTE INVALID ENTRY\n");
	}
	
}
