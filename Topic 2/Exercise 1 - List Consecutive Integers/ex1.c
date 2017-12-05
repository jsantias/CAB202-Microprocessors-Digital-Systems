#include <stdio.h>

void list_integers( void ) {
	// (a) Print the introductory message.
	printf("The integer subrange from 21 to 337...\n");

	// (b) Declare an integer variable that will be used as a counter. 
	//     Make it initially equal to the start value.
	int i = 21;

	// (c) Begin a WHILE statement that will execute its loop body if the
	//     counter is less than or equal to the end value.
	while( i <= 337 ) {
	
		// (e) Print the current value of the counter variable.
		printf("%d\n", i);

		// (f) Add 1 to the counter variable.
		i++;

	// (d) End the WHILE statement that starts at (c).
	}
}

int main( void ) {
	list_integers();
	return 0;
}
