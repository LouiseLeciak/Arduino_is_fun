
#include "utils.h"

void init_leds() {
    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);
}

void turn_blue() {
    PORTD |= (1 << BLUE);
    PORTD &= ~(1 << GREEN);
    PORTD &= ~(1 << RED);
}

void turn_green() {
    PORTD &= ~(1 << BLUE);
    PORTD |= (1 << GREEN);
    PORTD &= ~(1 << RED);
}

void turn_red() {
    PORTD &= ~(1 << BLUE);
    PORTD &= ~(1 << GREEN);
    PORTD |= (1 << RED);
}

// red		= 0010 0000,
// green	= 0100 0000,
// blue		= 0000 1000,
// yellow	= 0110 0000, red + green
// cyan		= 0100 1000, green + blue
// magenta	= 0010 1000, red + blue
// white	= 0110 1000  all 3

void turn_yellow(){
    PORTD &= ~(1 << BLUE);
    PORTD |= (1 << GREEN);
    PORTD |= (1 << RED);
}

void turn_cyan(){
    PORTD |= (1 << BLUE);
    PORTD |= (1 << GREEN);
    PORTD &= ~(1 << RED);
}

void turn_magenta(){
    PORTD |= (1 << BLUE);
    PORTD &= ~(1 << GREEN);
    PORTD |= (1 << RED);
}

void turn_white(){
    PORTD |= (1 << BLUE);
    PORTD |= (1 << GREEN);
    PORTD |= (1 << RED);
}
