/*
 * Try_button.c
 *
 * Created: 2/19/2021 6:06:58 PM
 * Author : lenovo
 */ 
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// define shared variable used for signaling
volatile uint8_t tick_signal = 0;

ISR(TIMER1_COMPA_vect, ISR_BLOCK)
{
	tick_signal++;					// signal "tick" event
	
}

int main(void)
{
	DDRB |= (1<<PORTB5);			// Configure PB5 as digital output
	PORTB &= ~(1<<PORTB5);			// Set initial level for PB5
	
	// Set up Timer/Counter1
	TCCR1B |= (1 << WGM12);			// Configure timer 1 for CTC(Clear timer on compare) mode
	OCR1A = 25000;					// Set CTC compare value to 10Hz (100mS) at
	// 16MHz AVR clock , with a prescaler of 64
	TIMSK1 |= (1 << OCIE1A);		// Enable CTC interrupt
	TCCR1B |= ((1 << CS10) | (1 << CS11));	// Start Timer/Counter1 at F_CPU/64
	
	// Enable global interrupts
	sei();
		
	while(1){
		if(tick_signal){
			PORTB ^= (1 << PORTB5); // (XOR) Toggle LED0
			tick_signal = 0;
		}
	}
}
