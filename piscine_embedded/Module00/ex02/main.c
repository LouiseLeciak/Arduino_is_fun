#include <avr/io.h>
#include <util/delay.h>

void turn_on_with_button(void) {
    DDRB |= (1 << PB0);
    // freeze all the bits, PD2 set to 0 (READ)
    DDRD &= ~(1 << PD2); // button, so start at 0
    // replace the (1<<PD2)


    // 14.2.4 reading the pin value
    // PINX: Port Input Pin, register of input data
    while (1){
        if (!(PIND & (1 << PD2))) { // if the button is not used and PD2 is 0
            PORTB |= (1 << PB0);// portb to 1, turn on
        }
        else
            PORTB &= ~(1 << PB0);// portb become 0, turn off
    }
}

int main (void)
{
    turn_on_with_button();
    return(0);
}