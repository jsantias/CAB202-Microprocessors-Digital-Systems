#include <stdint.h>
#include <stdio.h>
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <cpu_speed.h>

#include <graphics.h>
#include <macros.h>
#include "lcd_model.h"

//	(a) Implement a function called uart_setup which requires no arguments and
//	returns no value.
void uart_setup(void){
	//	(a.a) Set the data transmission rate to 1200 bits per second,
	//	using the formula provided in the Topic 10 Technical Notes.

	//	(a.b) Set the UART speed to normal speed.
	UBRR1 = (F_CPU/8/1200 - 1)/2;

	//	(a.c) Enable receive and transmit, but do not enable any UART-related
	//	interrupts.
	UCSR1B = (1<<RXEN1) | (1<<TXEN1);

	//	(a.d) Set the character size to 6 bits, with odd parity
	//	and 2 stop bits.
	UCSR1C |= (1 << UCSZ10) | (1 << UPM11)| (1 << UPM10) | (1 << USBS1);

	//UCSZ10 is character size 6 bits
	//UPM11 is the odd parity
	//USBS1 is 2 stop bits

//	(b) End of function uart_setup
}

int main(void) {
	uart_setup();

	for ( ;; ) {}
}
