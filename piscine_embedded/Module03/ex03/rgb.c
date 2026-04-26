#include "utils.h"


void init_rgb(){
    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);

    //p113 TIMER0 fast PMW
    //table 15-8 (mode 3)
    TCCR0A |= (1 << WGM00) | (1 << WGM01);
    // p113 table 15-4 & 15-7
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1);

    // prescaler 8 p117 table 15-9
    TCCR0B |= (1 << CS01);

    //p164
    // timer02 bcs in schema LED_B PD3(OC2B/INT1)
    TCCR2A |= (1 << WGM20) | (1 << WGM21);
    TCCR2A |= (1 << COM2A1);
    TCCR2B |= (1 << CS21);
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b){
    if (r == 0){
        TCCR0A &= ~(1 << COM0B1);
        PORTD &= ~(1 << PD5);
    }
    if (g == 0){
        TCCR0A &= ~(1 << COM0A1);
        PORTD &= ~(1 << PD6);
    }
    if (b == 0) {
        TCCR2A &= ~(1 << COM2A1);
        PORTD &= ~(1 << PD3);
    }
    if (r != 0){
        TCCR0A |= (1 << COM0B1);
        PORTD |= (1 << PD5);
    }
    if (g != 0){
        TCCR0A |= (1 << COM0A1);
        PORTD |= (1 << PD6);
    }
    if (b != 0) {
        TCCR2A |= (1 << COM2A1);
        PORTD |= (1 << PD3);
    }

    OCR0B = r;
    OCR0A = g;
    OCR2B = b;
}


uint8_t hex_to_bit(char *hex) {
    // * 16 bcs HEX is base 16
    return atoi_hex(hex[0]) * 16 + atoi_hex(hex[1]);
}

