
#include "utils.h"

//“Temperature Measurement” (ADC) p256 24-8
// table “Temperature sensor characteristics”
// Temperature = (ADC_value - ADC_at_25°C) / slope + 25
// ou temp ≈ ADC - constante
void init_temp_sensor() {
    // 1000 = temperature sensor (ADC8)
    // p256 24.8
    ADMUX |= (1 << MUX3);
    // voltage reference selection p257
    // 1.1v : The internal 1.1V voltage reference must also be selected for the ADC voltage
    // reference source in the temperature sensor measurement p 256
    ADMUX |= (1 << REFS0) | (1 <<REFS1);
}

// from datasheet p257
// T = { [(ADCH << 8) | ADCL] - TOS} / k
// from discord
// temp = (ADC - 314) / 1.22;

int main() {
    ADC_init();
    uart_init();

    while (1) {
        uint16_t res = ADC_read_10();
        int temp = (res - 324.31) / 1.22;
        uart_printint(temp);
        uart_printstr("\r\n");

        _delay_ms(20);
    }
}