#include <avr/io.h>
#include <util/delay.h>

// p12 top left PD2(INT0) (PD2 = button SW1)
// External Interrupt request

volatile uint16_t last = 0;
volatile uint16_t tic = 0;

//when timer reach the top (OCR1A)
// so every ~1ms
__attribute__((signal))void TIMER1_COMPA_vect(void) {
	tic++;
}

// interrupt when SW1 is pressed
__attribute__((signal))void INT0_vect(void) {
    // if more than 20ms have passed then the delay is over and we can toggle the led
	if (tic - last > 20)
	{
        // ^ to toggle the led
		PORTB ^= (1 << PB0);
		last = tic;
	}
}

int main(void){

    // TIMER
    // CTC mod
    TCCR1B |= (1 << WGM12);
    // prescaler
    TCCR1B |= (1 << CS12);
    // enable compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    
    DDRB |= (1 << PB0);
    //p81 13.2.2
    // enable INT0, set the interrupt when sw1 is pressed
    EIMSK |= (1 << INT0);
    // ~1ms, OCR1A = TOP
    // 16MHz/256 = 62500, 62500/62 ~= 1000Hz
	OCR1A = 62;
    //p80 13.2.1 & 13-2
    // The rising edge of INT0 generates an interrupt request
    EICRA |= (1 << ISC00) | (1 << ISC01);
    
    SREG |= (1 << 7);
    while(1){}
}