#include "utils.h"

// https://www.f-legrand.fr/scidoc/docmml/sciphys/arduino/adcrapide/adcrapide.html
// https://ece-classes.usc.edu/ee459/library/documents/ADC.pdf

// 0x00 = 0V
// 0xff = 5V
// 0x80 ~= 2.5V

void ADC_init() {
    // ADC multiplexer selection register
    // REFS0 to choose AVCC

    ADMUX |= (1 << REFS0);

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
    // start a conversion
    ADCSRA |= (1 << ADSC);
    // ADLAR = i want to read 8 or 10 bits? p259 schema 24.9.3
    // for 8 with ADLAR = 1 only have to read ADCH
    ADMUX |= (1 << ADLAR);
    // wait until the conversion is over
    while(ADCSRA & (1 << ADSC)){}

    //p259 24.9.3.2
    return ADCH;
}

void set_ADC(uint8_t adc) {
    // p 257 24.9.1
    // on schema RV1 = ADC0, LDR = ADC1, NTC = ADC2
    // p258 table 24-4
    if (adc == SET_RV1){
        ADMUX &= ~(1 << MUX0);
        ADMUX &= ~(1 << MUX1);
        ADMUX &= ~(1 << MUX2);
        ADMUX &= ~(1 << MUX3);
    }
    if (adc == SET_LDR){
        ADMUX |= (1 << MUX0);
        ADMUX &= ~(1 << MUX1);
        ADMUX &= ~(1 << MUX2);
        ADMUX &= ~(1 << MUX3);
    }
    if (adc == SET_NTC){
        ADMUX &= ~(1 << MUX0);
        ADMUX |= (1 << MUX1);
        ADMUX &= ~(1 << MUX2);
        ADMUX &= ~(1 << MUX3);
    }
}