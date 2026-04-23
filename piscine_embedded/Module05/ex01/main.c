
#include "utils.h"

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


int main() {
    uint8_t res_r = 0;
    uint8_t res_l = 0;
    uint8_t res_n = 0;

    ADC_init();
    uart_init();

    while (1) {
        set_ADC(SET_RV1);
        res_r = ADC_read();
        set_ADC(SET_LDR);
        res_l = ADC_read();
        set_ADC(SET_NTC);
        res_n = ADC_read();

        uart_printhex(res_r);
        uart_printstr(", ");
        uart_printhex(res_l);
        uart_printstr(" ,");
        uart_printhex(res_n);
        uart_printstr("\n\r");
        _delay_ms(20);
    }
}