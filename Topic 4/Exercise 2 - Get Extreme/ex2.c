#include <stdio.h>

//  (a) Begin the definition a function called get_extreme that returns the
//      maximum of its three double precision floating point 
//      arguments.
//      The arguments may be called anything, you like, but something simple
//      like x, y, and z will suffice. 
double get_extreme(double x, double y, double z){
    double result;

    //  (c) Implement the logic required to calculate the maximum
    //      of the three input values, and return the result.
    if (x == y && x == z && y == z){
        result = x;
    }

     if (x < y && y < z && x < z){
        result = z;
    }

    if (x > y && y > z && x > z){
        result = x;
    }

    if ((x == y && y < z && x < z) || (x < y && y == z && x < z)){
        result = z;
    }

    if ((x == y && y > z && x > z) || (x > y && y == z && x > z)){
        result = x;
    }
    if (x > y && z > y && x > z){
        result = x;
    }
    if (x > y && z > y && x < z){
        result = z;
    }
    if (x < y && y > z){
        result = y;
    }
    return result;


//  (b) End the definition of get_extreme.
}