#include <avr/io.h>

//timer 16bit so can count to 65535
#define PWM_TOP 1000
#define LEVEL (PWM_TOP/10) // 10 levels of brightness

volatile uint8_t duty_cycle_step = 0; // going from 0 to 1
volatile bool up = 0;
volatile bool down = 0;
// tick on ms to wait a little longer
volatile uint8_t tick = 0;


void __attribute__((signal)) __vector_14(void) {
    
    tick++;
    //F_CPU/1024 = 15625 => 1/15626= 0.000064
    // 256 * 0.000064 = 0.016 so ~16ms, too fast
    // i need it to be a little slower so i increment tick
    // 16 * 3 = ~48ms
    if (tick >= 3){
        // if already at max then decrement and vice versa
        if (duty_cycle_step == 10){
            down = 1;
            up = 0;
        }
        else if (duty_cycle_step == 0){
            down = 0;
            up = 1;
        }

        if (down == 1 && up == 0)
            duty_cycle_step--;
        else if (down == 0 && up == 1)
            duty_cycle_step++;
        // tick back to 0
        tick = 0;

        OCR1A = duty_cycle_step * LEVEL;
    }
}

int main (void) {
    // set the led
    DDRB |= (1 << PB1);

    // using 2 timers 
    // timer0 for interruption

    // CTC
    TCCR0A |= (1 << WGM01);
    //prescaler 64 p117
    TCCR0B |= (1 << CS02) | (1 << CS00);
    // top
    OCR0A = 250;
    // enable the interrupt
    //p118
    TIMSK0 |= (1 << OCIE0A);


    //timer1 for PWM

    // fast PWM with ICR1 and
    // COM1A! to clear on compare match
    //p140 & p141
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12);
    // no prescaler
    // p117 table 15-9
    TCCR1B |= (1 << CS10);
    // top of the counter
    ICR1 = PWM_TOP;
    // led off at first
    OCR1A = 0; 
    // enable interrupt
    SREG |= (1 << 7);
    while(1){}
}