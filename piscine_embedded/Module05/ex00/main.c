#include "utils.h"

int main(void) {
    #include <avr/io.h>
#include <util/delay.h>

void ADC_init() {
    ADMUX = (1 << REFS0) | (1 << ADLAR); // AVCC + left adjust
    ADCSRA = (1 << ADEN)                 // enable ADC
           | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler 128
}

uint8_t ADC_read() {
    ADCSRA |= (1 << ADSC); // start conversion
    while (ADCSRA & (1 << ADSC)); // wait
    return ADCH; // 8 bits
}

int main() {
    ADC_init();

    while (1) {
        uint8_t val = ADC_read();

        // display with uart

        _delay_ms(20);
    }
}
}