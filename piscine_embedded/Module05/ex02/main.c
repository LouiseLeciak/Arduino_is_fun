
#include "utils.h"

uint16_t ADC_read_10() {
    // p252 ADC input Channels
    //  start a conversion
    ADCSRA |= (1 << ADSC);
    // ADLAR = i want to read 8 or 10 bits? p259 schema 24.9.3
    // for 8 with ADLAR = 1 only have to read ADCH
    // ADMUX |= (1 << ADLAR);
    // wait until the conversion is over
    while (ADCSRA & (1 << ADSC)) {
    }

    // p259 24.9.3.2
    return ADC;
}

void uart_printdec(uint16_t value) {
    char buffer[5];
    int i = 0;

    // if 0
    if (value == 0) {
        uart_tx('0');
        return;
    }

    // get numbers, reverse
    while (value > 0) {
        buffer[i++] = (value % 10) + '0';
        value /= 10;
    }

    // print it in the right order
    for (int j = i - 1; j >= 0; j--) {
        char c[2];
        c[0] = buffer[j];
        c[1] = '\0';
        uart_printstr(c);
    }
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

        uart_printdec(res_r);
        uart_printstr(", ");
        uart_printdec(res_l);
        uart_printstr(" ,");
        uart_printdec(res_n);
        uart_printstr("\n\r");
        _delay_ms(20);
    }
}