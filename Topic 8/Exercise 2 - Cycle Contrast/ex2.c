#include <avr/io.h>
#include <util/delay.h>
#include <cpu_speed.h>
#include <macros.h>
#include <graphics.h>
#include <lcd.h>
#include "lcd_model.h"

void setup( void ) {
	set_clock_speed(CPU_8MHz);

	//	(a) Enable input from the left button and right button.
	CLEAR_BIT(DDRF, 6);
	CLEAR_BIT(DDRF, 5);

	//	(b) Initialise the LCD display using the default contrast setting.
	lcd_init(LCD_DEFAULT_CONTRAST);

	//	(c) Use one of the functions declared in "graphics.h" to display 
	//	your student number, "n9983244", using the foreground colour, 
	//	positioning the left edge of the text at 12 and the nominal top 
	//	of the text at 17.
	draw_string(12, 17, "n9983244", FG_COLOUR);

	//	(d) Use one of the functions declared in "graphics.h" to copy the contents 
	//	of the screen buffer to the LCD.
	show_screen();
}

//	(e) Declare a global variable of type int named contrast and 
//	initialise it to the default LCD contrast.
int contrast = LCD_DEFAULT_CONTRAST;

void process(void) {
	//	(f) Test pin corresponding to right button. If closed,
	//	decrement contrast by 6. If the result is less than zero,
	//	set contrast to the maximum permitted LCD contrast value.
	if (BIT_IS_SET(PINF, 5)){
		contrast = contrast - 6;
		if (contrast < 0){
			contrast = 127;
		}
	}


	//	(g) Test pin corresponding to left button. If closed,
	//	increment contrast by 6. If the result is greater
	//	then the maximum permitted LCD contrast value, set contrast 
	//	to zero.
	if (BIT_IS_SET(PINF, 6)){
		contrast = contrast + 6;
		if (contrast > 127){
			contrast = 0;
		}
	}

	//	(h) Send a sequence of commands to the LCD to enable extended
	//	instructions, set contrast to the current value of contrast, 
	//	and finally restore the LCD to basic instruction mode.
	LCD_CMD( lcd_set_function, lcd_instr_extended );
	LCD_CMD( lcd_set_contrast, contrast );
	LCD_CMD( lcd_set_function, lcd_instr_basic );
}


int main(void) {
	setup();

	for ( ;; ) {
		process();
		_delay_ms(100);
	}

	return 0;
}