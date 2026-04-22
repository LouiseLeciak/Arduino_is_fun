#include <avr/io.h>
#include <util/delay.h>

#define LEDS ((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4))

volatile uint16_t tic = 0;
volatile uint16_t last = 0;
volatile uint8_t counter = 0;
volatile uint8_t sw2_pending = 0;
volatile uint16_t sw2_time = 0;

void display_counter(int counter) {
    // extracts the 4th bit
    int get_pb3 = (counter & (1 << 3));
    // move it to pb4
    int clean_fourth_bit = (get_pb3 << 1);
    int clean_bits = ((clean_fourth_bit) | (counter & 0b111));
    // do not modify the bits not affiliated to leds, so ~LED to ignore it
    PORTB = ((PORTB & ~LEDS) | clean_bits);
}

// when timer reach the top (OCR1A)
//  so every ~1ms
void __attribute__((signal)) TIMER0_COMPA_vect(void) {
    // ms counter
    tic++;

    // if we have detected a press AND it's been ~30ms or more
    // then it's a real press and not a bounce
    if (sw2_pending && (tic - sw2_time >= 30)) {
        // is the button still pressed
        if (!(PIND & (1 << PD4))) {
            if (counter > 0)
                counter--;

            display_counter(counter);
        }
        sw2_pending = 0;
    }
}


void set_up() {
    // i want my led to be the output (1)
    DDRB |= LEDS;

    // enable interrupt pin change p82
    PCICR |= (1 << PCIE2);
    // p82 in PCIE2 paraf PCINT[23:16] pins are enabled individually by the
    // PCMSK2 Register. enable PCINT for PD2 and PD4 p12 bottom right schema
    PCMSK2 |= (1 << PCINT18) | (1 << PCINT20);

    // enable interrupt
    SREG |= (1 << 7);

    // set up TIMER
    // p115 CTC mod
    TCCR0A |= (1 << WGM01);
    // prescaler
    TCCR0B |= (1 << CS02);
    // enable compare interrupt
    TIMSK0 |= (1 << OCIE0A);
    // ~1ms, OCR1A = TOP
    OCR0A = 62;

    // enable INT0, set the interrupt when SW1 is pressed
    EIMSK |= (1 << INT0);
    // // // mod
    // EICRA |= (1 << ISC00);
}

//when SW1 is pressed
__attribute__((signal)) void INT0_vect(void) {
    // if more than 50ms have passed then the delay is over and we can toggle
    // the led delay to debounce while to avoid the incrementation while holding
    // the button
    // INT0 is for sw1 so no need for the pin
    if (tic - last >= 50) {
        last = tic;
        if (counter < 15) 
            counter++;
        display_counter(counter);
    }
}
// PCINT2_vect because PD4 -> PCINT20 p12
// PCINT2 = Pin change Interrupt Request 2 p74
void __attribute__((signal)) PCINT2_vect(void) {
    uint8_t current_state = (PIND & (1 << PD4)) != 0;
    // check if pressed
    if (!current_state) {
        // detect a press and does nothing for now
        sw2_pending = 1;
        // register the time when we detect a press
        sw2_time = tic;
    }
}


// 14.2.1
int main(void) {
    set_up();
    while (1) {
    }
}
