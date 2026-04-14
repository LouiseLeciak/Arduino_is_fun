#include <avr/io.h>
#include <util/delay.h>

void turn_on_with_button(void) {
    DDRB |= (1 << PB0);
    // freeze all the bits, PD2 set to 0 (READ)
    DDRD &= ~(1 << PD2); // button, so start at 0
    // replace the (1<<PD2)

    bool previous = false; // was it on before ?
    bool on = false;

    while (1){
        bool current = (!(PIND & (1 << PD2)));
        if (!previous && current) { // if currentand different from previous
            if (on)
                PORTB &= ~(1 << PB0);// portb become 0, turn off
            else
                PORTB |= (1 << PB0);// portb to 1, turn on
            on = !on;// if on then off, if off then on
        }
        previous = current;// to remember if it was previously on or off;
        _delay_ms(30.f);
    }
}

int main (void)
{
    turn_on_with_button();
    return(0);
}