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
    TCCR2A |= (1 << WGM20) | (1 << WGM21);
    TCCR2A |= (1 << COM2A1) | (1 << COM2B1);
    TCCR2B |= (1 << CS21);

}

void set_rgb(uint8_t r, uint8_t g, uint8_t b){
    OCR0B = r;
    OCR0A = g;
    OCR2B = b;
}

void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        set_rgb(255 - pos * 3, 0, pos * 3);
    } else if (pos < 170) {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    } else {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}