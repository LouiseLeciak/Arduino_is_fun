#include <avr/io.h>

// chap 16 datasheet
#define TIME_FREQUENCY (16000000/256)
// 16 bits = 65565 max
// 16M/8/2 = 1000000 > 65565 too much
// 16M/16/2 = 125000 > 65565 too much
// 16M/256/2 = 31250 < 65565

int main (void) {
    // set the D2 led
    DDRB |= (1 << PB1);
    

    // p140, table 16-2
    // we want clear on Compare match with top
    // set when at bottom
    // toggle clear to top
    // top send itback at bottom, bottom set it etc..
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    

    // for mode 15 fast PWM
    // need to have 1 for: WGM13/12/11/10
    // 11 and 10 are in TCCR1A
    TCCR1B |= (1 << WGM12) | (1 << WGM13);
    
    //p 143, table 16-5 CS12 1 0 0 to have 256 as prescaler
    TCCR1B |= (1 << CS12);
    
    //set [TOP] p142 16-4 fast PWM
    ICR1 = TIME_FREQUENCY;
    // 1sec [TOGGLE]
    // 10% of time frequency
    OCR1A = TIME_FREQUENCY/10;
    while (1){}
}