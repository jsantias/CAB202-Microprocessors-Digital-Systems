#include "cab202_graphics.h"

void draw_lines(void){
	// (a) Draw a line from (75,14) to (8,4).
	draw_line( 75, 14, 8, 4, '&' );
	// (b) Draw a line from (8,4) to (15,9).
	draw_line( 8, 4, 15, 9, '&' );
	// (c) Draw a line from (15,9) to (3,21).
	draw_line( 15, 9, 3, 21, '&' );
	// (d) Draw a line from (3,21) to (75,14).
	draw_line( 3, 21, 75, 14, '&' );
	// Leave the following instruction unchanged.
	show_screen();
}

int main( void ) {
	setup_screen( );

	draw_lines();

	draw_string( 0, screen_height( ) - 1, "Press any key to finish..." );
	wait_char( );
	cleanup_screen( );
	return 0;
}
