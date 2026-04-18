
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
