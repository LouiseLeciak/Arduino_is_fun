
#include "utils.h"

int main() {
    ADC_init();
    SPI_master_init();
    uint8_t res = 0;
    uint8_t led1[3] = {0, 0, 0};
    uint8_t led2[3] = {0, 0, 0};
    uint8_t led3[3] = {0, 0, 0};

    while (1) {

        //reset leds
        led1[0] = led1[1] = led1[2] = 0;
        led2[0] = led2[1] = led2[2] = 0;
        led3[0] = led3[1] = led3[2] = 0;

        res = ADC_read_8();
        // 255 * 0.33 ~= 85
        if (res >= 85) {
            led1[0] = 255;
        }

        if (res >= 170) {
            led2[1] = 255;
        }

        if (res >= 250) {
            led3[2] = 255;
        }

        set_three_leds(led1, led2, led3);
        _delay_ms(30);
    }
}