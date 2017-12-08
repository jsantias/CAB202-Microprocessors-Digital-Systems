#include <avr/io.h>
#include <util/delay.h>
#include <cpu_speed.h>
#include <macros.h>

void setup_led_buttons( void ) { 
    //  (a) Set the CPU speed to 8MHz (you must also be compiling at 8MHz).
    set_clock_speed(CPU_8MHz);

    //  (b) Configure the data direction register for Port B to enable output
    //  to LED0 and LED1. The data direction for LED0 is controlled by Pin 2, 
    //  while that for LED1 is controlled by Pin 3. No other pins should be 
    //  affected.
    SET_BIT(DDRB, 2);
    SET_BIT(DDRB, 3);

    //  (c) Turn off LED0, LED1, and all other outputs connected to Port B, by 
    //  clearing all bits in the Port B output register.
    PORTB = 0;

    //  (d) Configure the data direction register for Port F to enable input
    //  from the left button (SW1 on the pin-out diagram) and right button 
    //  (SW2 on the pin-out diagram). SW1 is activated by clearing Pin 6, and 
    //  SW2 is activated by clearing Pin 5. No other pins should be affected by 
    //  this operation.
    CLEAR_BIT(DDRF, 6);
    CLEAR_BIT(DDRF, 5);
    // PORTF &= ~(1 << 6);
    // PORTF &= ~(1 << 5);
}

void process_led_buttons( void ) {
    //  (e) Test the relevant pins in the Port F input register. If the 
    //  bits corresponding to _both_ SW1 and SW2 are set, turn off _both_ LEDs. No 
    //  pins other than those controlling LED0 and LED1 should be affected by 
    //  this action.
    if ( BIT_IS_SET(PINF, 6) & BIT_IS_SET(PINF, 5)) {
        CLEAR_BIT(PORTB, 2);
        CLEAR_BIT(PORTB, 3);
    }

    //  (f) Otherwise, if the bit corresponding to SW1 is set, turn on LED0
    //  while ensuring that no other pins are affected.
    else if ( BIT_IS_SET(PINF, 6) ) {
        SET_BIT(PORTB, 2);
    }

    //  (g) Otherwise, if the bit corresponding to SW2 is set, turn on LED1
    //  while ensuring that no other pins are affected.
    else if ( BIT_IS_SET(PINF, 5) ) {
        SET_BIT(PORTB, 3);
    }
}


int main(void) {
    setup_led_buttons();

    while ( 1 ) {
        process_led_buttons();
        _delay_ms(100);
    }

    return 0;
}
