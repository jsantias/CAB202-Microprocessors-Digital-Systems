#include <stdlib.h>
#include <cab202_graphics.h>

//  (a) Begin the definition a function called fill_rect that returns nothing, 
//      but requires the following arguments:
//      l - an integer that specifies the absolute horizontal location 
//          of the left hand side of the rectangle.
//      t - an integer that specifies the absolute vertical location 
//          of the upper edge of the rectangle.
//      r - an integer that specifies the absolute horizontal location 
//          of the right hand side of the rectangle.
//      b - an integer that specifies the absolute vertical location 
//          of the lower edge of the rectangle.
//      symbol = a character value that will be used to draw the
//          display the rectangle.
void fill_rect(int l, int t, int r, int b, char symbol){

    //  (c) Test to see if either the width or height of the rectangle is less 
    //      than 1. If so, the function should stop immediately and draw nothing.
    if (b >= 1 && r >= 1 && b >= t && r >= l){
    //  (d) Use statements of your own choice fill the region defined by the
    //      arguments with the designated character value. 

        int y_ctr = t;
        for(;y_ctr <= b; y_ctr++){
            draw_line(l, y_ctr, r, y_ctr, symbol);
        }
    }

//  (b) End the definition of fill_rect.
}