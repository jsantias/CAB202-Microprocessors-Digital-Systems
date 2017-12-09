#include <stdint.h>
#include <stdio.h>
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <cpu_speed.h>
#include <graphics.h>

void uart_init(int baud);

void setup(void) {
	set_clock_speed(CPU_8MHz);
	uart_init(9600);
	lcd_init(LCD_DEFAULT_CONTRAST);
	lcd_clear();
	show_screen();
}

void uart_init(int baud) {
	UBRR1 = (F_CPU / 4 / baud - 1) / 2;
	UCSR1A = (1 << U2X1);
	UCSR1B = (1 << RXEN1) | (1 << TXEN1);
	UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
}

//	(a) Function uart_xmit must implement the algorithm on 
//	P193 of the ATMega32U4 datasheet to transmit a byte over UART.
void uart_xmit(char sent_char){
	//	(a.a) Wait until the UART transmit buffer to be empty.
	while (!(UCSR1A & (1<<UDRE1)));

	//	(a.b) Put the character to the UART transmit buffer.
	UDR1 = sent_char;

//	a.c End of uart_recv_char
}

//	(b) Function uart_recv_char must implement the algorithm on 
//	P196 of the ATMega32U4 datasheet to receive a byte from UART.
unsigned char uart_recv_char(void){
	//	(b.a) Wait for data to be received.
	while (!(UCSR1A & (1<<RXC1)));

	//	(b.b) Get and return received data from buffer.
	return UDR1;

//	b.c End of uart_recv_char
}


// -------------------------------------------------
// Test driver.
// -------------------------------------------------

void process(void) {
	clear_screen();
	draw_string(0, 0, "UART TX/RX", FG_COLOUR);

	static char sent_char = 'a';

	uart_xmit(sent_char);

	draw_string(0, 10, "TX:", FG_COLOUR);
	draw_char(3 * 5, 10, sent_char, FG_COLOUR);

	sent_char ++;
	if ( sent_char > 'z' ) sent_char = 'a';

	char received_char = uart_recv_char();
	draw_string(0, 20, "RX:", FG_COLOUR);
	draw_char(3 * 5, 20, received_char, FG_COLOUR);

	show_screen();
}

// ----------------------------------------------------------

int main(void) {
	setup();

	for ( ;; ) {
		process();
		_delay_ms(1000);
	}
}

