#include <stdint.h>
#include <stdio.h>
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <cpu_speed.h>

#include <graphics.h>
#include <macros.h>
#include "lcd_model.h"

void setup(void) {
	set_clock_speed(CPU_8MHz);
	lcd_init(LCD_DEFAULT_CONTRAST);
	lcd_clear();

	//	(a) Initialise Timer 0 in normal mode so that it overflows 
	//	with a period of approximately 0.008 seconds.
	//	Hint: use the table you completed in a previous exercise.
	TCCR0A = 0;
	TCCR0B = 4;

	//	(b) Enable timer overflow interrupt for Timer 0.
	TIMSK0 = 1; 
	//	(c) Turn on interrupts.
	sei();

	//	(d) Enable the joystick down switch for digital input.
	CLEAR_BIT(DDRB, 7);

	//	(e) Display your student number, "n9983244", with nominal
	//	top-left corner at screen location (12,21).
	draw_string(12, 21, "n9983244", FG_COLOUR);

	// Keep the next instruction intact.
	show_screen();
}

//	(f) Create a volatile global variable called bit_count.
//	The variable should be an 8-bit unsigned integer. 
//	Initialise the variable to 0.
uint8_t bit_count = 0;
uint8_t mask = 0b0111111;

//	(g) Define a volatile global 8-bit unsigned global variable 
//	called switch_closed which will store the current state of the switch.
volatile uint8_t switch_closed = 0;

//	(h) Define an interrupt service routine to process timer overflow
//	interrupts for Timer 0. Every time the interrupt service
//	routine is called, bit_count should:
ISR(TIMER0_OVF_vect){
//		(h.a) Left-shift bit_count one place;
		bit_count = bit_count << 1;
//		(h.b) Bitwise AND with a mask in which the 6 bits on the right 
//			are 1 and the others are 0.
		bit_count = bit_count & mask;
//		(h.c) Use bitwise OR to add the current open/closed value of the 
//			joystick down switch to the history.
		bit_count = bit_count | BIT_VALUE(PINB, 7);

//		(h.d) If bit_count is equal to the bit mask, then the switch has been 
//			observed 6 times in a row to be closed. Assign the value 1 to 
//			switch_closed, indicating that the switch should now be considered to be
//			officially "closed".
		if (bit_count == mask){	switch_closed =  1;}

//		(h.e) If bit_count is equal to 0, then the switch has been observed 
//			to be open at least 6 in a row, so store 0 in switch_closed, 
//			indicating that the switch should now be considered to be officially "closed".
		else if (bit_count == 0){ switch_closed = 0;}
}

// -------------------------------------------------
// Test driver.
// -------------------------------------------------

void process(void) {
	static uint8_t prevState = 0;
	if ( switch_closed != prevState ) {
		prevState = switch_closed;
		draw_string( 30, 40, prevState ? "closed" : "open  ", FG_COLOUR);
		show_screen();
	}
}

int main(void) {
	setup();

	for ( ;; ) {
		process();
	}
}
