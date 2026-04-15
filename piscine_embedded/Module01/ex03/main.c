#include <avr/io.h>
#include <util/delay.h>

// chap 16 datasheet
#define TIME_FREQUENCY (16000000/256)
// 16 bits = 65565 max
// 16M/8/2 = 1000000 > 65565 too much
// 16M/16/2 = 125000 > 65565 too much
// 16M/256/2 = 31250 < 65565

int main (void) {
    int duty_cycle = 1;
    // set the D2 led
    DDRB |= (1 << PB1);
    // buttons set to 0
    DDRD &= ~((1 << PD2) | (1 << PD4));
    

    // bottom set,toggle clear
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    // for mode 15 fast PWM
    TCCR1B |= (1 << WGM12) | (1 << WGM13); 
    // set prescaler at 256
    TCCR1B |= (1 << CS12);
    //set [TOP p142 16-4 fast PWM
    ICR1 = TIME_FREQUENCY;
    // 1sec [TOGGLE]
    // the duty cycle can change
    OCR1A = TIME_FREQUENCY / 10 * duty_cycle;
    
    while (1)
    {
        // if sw1 is used
        // PIND because PD2
        if (!(PIND & (1 << PD2)))
        {
            //delay to debounce
            // while to avoid the incrementation while holding the button
            _delay_ms(20);
            while (!(PIND & (1 << PD2)))
            ;
            _delay_ms(20);
            if (duty_cycle < 10)
                duty_cycle ++;
            OCR1A = TIME_FREQUENCY / 10 * duty_cycle;
        }
        
        // same with sw2
        // PIND because PD4
        else if (!(PIND & (1 << PD4)))
        {
            _delay_ms(20);
            while (!(PIND & (1 << PD4)))
            ;
            _delay_ms(20);
            if (duty_cycle > 1)
                duty_cycle --;
            OCR1A = TIME_FREQUENCY / 10 * duty_cycle;
        }
    }
}