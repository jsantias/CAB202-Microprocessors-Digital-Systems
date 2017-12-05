#include <stdlib.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>

//  (a) Declare four global integer variables, as follows:
//      horiz - the horizontal location of the left edge of the rectangle
//      vert - the vertical location of the top edge of the rectangle 
//      across - the width of the rectangle.
//      down - the height of the rectangle.
int horiz, vert, across, down;

//  (b) Declare a global variable of type char called display_char.
//      This is the character that is to be used to render the rectangle.
char display_char;

void draw_rect(void) {
    //  (c) Insert code to draw the outline of the rectangle defined by the global variables.
    //      If either of the width or height is less than or equal to zero,
    //      the function must not draw anything.
    if ((across < 1) || (down < 1)){
    	return;	
    }
    int horiz2 = horiz + across - 1;
    int vert2 = vert + down - 1;

    draw_line(horiz, vert, horiz2, vert, display_char);
    draw_line(horiz, vert2, horiz2, vert2, display_char);
    draw_line(horiz, vert, horiz, vert2, display_char);
    draw_line(horiz2, vert, horiz2, vert2, display_char);
}
