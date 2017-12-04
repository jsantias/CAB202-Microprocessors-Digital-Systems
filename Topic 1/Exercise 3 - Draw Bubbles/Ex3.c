#include "cab202_graphics.h"

void draw_bubbles(void){
	// (a) Draw a bubble at (62,1)
	draw_char( 62, 1, 'O' );

	// (b) Draw a bubble at (13,15)
	draw_char( 13, 15, 'O' );

	// (c) Draw a bubble at (16,22)
	draw_char( 16, 22, 'O' );

	// (d) Draw a bubble at (41,23)
	draw_char( 41, 23, 'O' );

	// Keep the following line without change.
	show_screen();
}

int main( void ) {
	setup_screen();

	draw_bubbles();

	draw_string( 0, screen_height() - 1, "Press any key to finish..." );
	wait_char();
	cleanup_screen();
	return 0;
}
