
#include "utils.h"

//“Temperature Measurement” (ADC)
// table “Temperature sensor characteristics”
// Temperature = (ADC_value - ADC_at_25°C) / slope + 25
// ou temp ≈ ADC - constante
void init_temp_sensor() {
    // 1000 = temperature sensor (ADC8)
    ADMUX |= (1 << MUX3);
    // [CHECK DATASHEET] 1.1V
    // ADMUX |= (1 << REFS1) | (1 << REFS0);
    ADMUX |= (1 << REFS0);
}



int main() {
    ADC_init();
    uart_init();

    while (1) {
        uint8_t res = ADC_read();

        uart_printint(res);
        uart_printstr("\r\n");

        _delay_ms(20);
    }
}