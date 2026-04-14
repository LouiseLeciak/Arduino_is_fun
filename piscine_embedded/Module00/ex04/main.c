#include <avr/io.h>
#include <util/delay.h>

#define BUTTONS ((1 << PD2) | (1 << PD4))
#define LEDS ((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4))


void display_counter(int counter) {
    // extracts the 4th bit
    int get_pb3 = (counter & (1 << 3));
    // move it to pb4
    int clean_fourth_bit = (get_pb3 << 1);
    int clean_bits = ((clean_fourth_bit) | (counter & 0b111));
    // do not modify the bits not affiliated to leds, so ~LED to ignore it
    PORTB = ((PORTB & ~LEDS) | clean_bits);
}

void set_up(){
    // and the buttons the input (0)
    DDRD &= ~BUTTONS;
    // i want my led to be the output (1)
    DDRB |= LEDS;
}
// 14.2.1
int main (void) {
    int counter = 0;

    set_up();
    while (1)
    {
        // if sw1 is used
        // PIND because PD2
        if (!(PIND & (1 << PD2)))
        {
            //delay to debounce
            // while to avoid the incrementation while holding the button
            _delay_ms(20.f);
            while (!(PIND & (1 << PD2)))
                ;
            _delay_ms(20.f);
            if (counter < 15)
                counter++;
        }

        // same with sw2
        // PIND because PD4
        else if (!(PIND & (1 << PD4)))
        {
            _delay_ms(20.f);
            while (!(PIND & (1 << PD4)))
                ;
            _delay_ms(20.f);
            if (counter > 0)
                counter--;
        }
        display_counter(counter);
    }
}
