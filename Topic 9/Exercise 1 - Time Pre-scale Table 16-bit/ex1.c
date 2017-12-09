// Replace 0 with the correct value for each unknown cell.
double prescale_001 = 1;
double prescale_010 = 8;
double prescale_011 = 64;
double prescale_100 = 256;
double prescale_101 = 1024;
double counter_frequency_001 = 8000000;
double counter_frequency_010 = 1000000;
double counter_frequency_011 = 125000;
double counter_frequency_100 = 31250;
double counter_frequency_101 = 7812.5;
double overflow_period_001 = 0.000032*256;
double overflow_period_010 = 0.000256*256; 
double overflow_period_011 = 0.002048*256; 
double overflow_period_100 = 0.008192*256;
double overflow_period_101 = 0.032768*256;
double overflow_freq_001 = 8000000.0/65536.0; 
double overflow_freq_010 = 1000000.0/65536.0;
double overflow_freq_011 = 125000.0/65536.0;
double overflow_freq_100 = 31250.0/65536.0;
double overflow_freq_101 = 7812.5/65536.0;
// There is no test driver for this program.

int main() {
	for ( ;; ) {
	}

	return 0;
}