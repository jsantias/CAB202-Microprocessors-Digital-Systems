#include <stdio.h>

void count_down( void ) {
	// (a) Print the introductory message.
	printf("Count down from 337, by steps of 6, finishing before lower bound (92) is reached.\n");

	// (b) Declare an integer variable that you will use to count. Initially 
	//     the counter should equal the start value, 337.
	int i = 337;

	// (c) Begin a WHILE statement that will execute its loop body if the 
	//     counter is greater than the end value, 92.
	while( i <= 337 && i > 92 ) {

		// (e) Print the value of the counter on a line by itself.
		printf("%d\n", i);
		// (f) Subtract the step size, 6, from the counter.
		i = i - 6;
	// (d) End the WHILE statement
	}
}

int main( void ) {
	count_down();
	return 0;
}
