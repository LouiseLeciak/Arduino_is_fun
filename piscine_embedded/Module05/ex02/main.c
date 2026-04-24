
#include "utils.h"


uint16_t ADC_read_10() {
    //p252 ADC input Channels
    // start a conversion
    ADCSRA |= (1 << ADSC);
    // ADLAR = i want to read 8 or 10 bits? p259 schema 24.9.3
    // for 8 with ADLAR = 1 only have to read ADCH
    // ADMUX |= (1 << ADLAR);
    // wait until the conversion is over
    while(ADCSRA & (1 << ADSC)){}

    //p259 24.9.3.2
    return ADC;
}


int main() {
    uint16_t res_r = 0;
    uint16_t res_l = 0;
    uint16_t res_n = 0;

    ADC_init();
    uart_init();

    while (1) {
        set_ADC(SET_RV1);
        res_r = ADC_read_10();
        set_ADC(SET_LDR);
        res_l = ADC_read_10();
        set_ADC(SET_NTC);
        res_n = ADC_read_10();

        uart_printhex_10(res_r);
        uart_printstr(", ");
        uart_printhex_10(res_l);
        uart_printstr(" ,");
        uart_printhex_10(res_n);
        uart_printstr("\n\r");
        _delay_ms(20);
    }
}