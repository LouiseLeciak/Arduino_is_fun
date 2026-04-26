#include <avr/io.h>

// chap 16 datasheet
#define TIME_FREQUENCY 16000000/256
// 16 bits = 65565 max
// 16M/8/2 = 1000000 > 65565 too much
// 16M/16/2 = 125000 > 65565 too much
// 16M/256/2 = 31250 < 65565

int main (void) {
    // set the D2 led
    DDRB |= (1 << PB1);
    
    // choose what to do with timer -> TCCR1A
    // toggle OC1A on Compare Match
    // p140, table 16-1
    TCCR1A |= (1 << COM1A0);
    
    // i want the timer to count
    // p 141, table 16-4, I want CTC so WGM12 has to be 1
    // CTC means i want the timer to count
    // if i check 16.11.2 i can see that wgm12 is in TCCR1B
    TCCR1B |= (1 << WGM12);
    
    //p 143, table 16-5 CS12 1 0 0 to have 256 as prescaler
    TCCR1B |= (1 << CS12);
    
    //p 141 table 16-4 CTC -> [TOP] OCR1A
    // /a because we want 0,5s
    OCR1A = TIME_FREQUENCY/2;
    while (1){}
}