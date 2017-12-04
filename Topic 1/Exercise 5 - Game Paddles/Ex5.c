#include "cab202_graphics.h"

 
int yt;
int yb;
int left_top;
int left_bottom;
int right_bottom;
int right_top;
 
void draw_paddles( void ) {
    left_top = (( screen_height() - 5) / 2 + 1);
    left_bottom = ( left_top + 5 - 1);
    draw_line( 2, left_top, 2, left_bottom, '~');
    right_top = ( (screen_height() - 5) / 2 + 4);
    right_bottom = ( right_top + 5 - 1);
    draw_line( (screen_width() - 5), right_top, (screen_width() - 5), right_bottom, '~');
    show_screen( );
}

 
int main( void ) {
    setup_screen();
    draw_paddles();
    draw_string( 0, screen_height( ) - 1, "Press any key to finish..." );
    wait_char();
    cleanup_screen();
    return 0;
}
