
#include "utils.h"

void ADC_init() {
    // ADC multiplexer selection register
    // REFS0 to choose AVCC
    // ADLAR = i want to read 8 or 10 bits? p259 schema 24.9.3
    // for 8 with ADLAR = 1 only have to read ADCH
    ADMUX |= (1 << REFS0) | (1 << ADLAR);

    // enable the adc
    ADCSRA |= (1 << ADEN);
    // prescaler,p249 max 200kHz, closer = more precise
    // 16000000/64 = 250000 too much
    // 16000000/128 = 125000
    // p259 table 24-5
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint8_t ADC_read_8() {
    //p252 ADC input Channels
    // p259.9.2 start a conversion
    ADCSRA |= (1 << ADSC);
    // wait until the conversion is over
    while(ADCSRA & (1 << ADSC)){}

    //p259 24.9.3.2
    return ADCH;
}