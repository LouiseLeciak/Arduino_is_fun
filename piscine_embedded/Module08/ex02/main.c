
#include "utils.h"

void set_three_leds(uint8_t led1[3], uint8_t led2[3], uint8_t led3[3]) {
    // set 0 to clean
    for (int i = 0; i < 4; i++) {
        SPI_master_transmit(0x00);
    }

    // LED 1 D6
    SPI_master_transmit(0xFF);     // brigthness => first byte read
    SPI_master_transmit(led1[2]);  // blue=> second byte read
    SPI_master_transmit(led1[1]);
    SPI_master_transmit(led1[0]);

    // LED 2 D7
    SPI_master_transmit(0xFF);     // brigthness => first byte read
    SPI_master_transmit(led2[2]);  // blue=> second byte read
    SPI_master_transmit(led2[1]);
    SPI_master_transmit(led2[0]);

    // LED 3 D8
    SPI_master_transmit(0xFF);     // brigthness => first byte read
    SPI_master_transmit(led3[2]);  // blue=> second byte read
    SPI_master_transmit(led3[1]);
    SPI_master_transmit(led3[0]);

    // end frame
    for (int i = 0; i < 4; i++) {
        SPI_master_transmit(0x00);
    }
}

int main() {
    uint8_t led1[3] = {};
    uint8_t led2[3] = {};
    uint8_t led3[3] = {};
    SPI_master_init();

    while (1) {
        led1[0] = 255;
        led1[1] = 0;
        led1[2] = 0;

        led2[0] = 0;
        led2[1] = 0;
        led2[2] = 0;

        led3[0] = 0;
        led3[1] = 0;
        led3[2] = 0;

        set_three_leds(led1, led2, led3);
        _delay_ms(1000);

        led1[0] = 0;
        led1[1] = 0;
        led1[2] = 0;

        led2[0] = 0;
        led2[1] = 255;
        led2[2] = 0;

        led3[0] = 0;
        led3[1] = 0;
        led3[2] = 0;

        set_three_leds(led1, led2, led3);
        _delay_ms(1000);

        led1[0] = 0;
        led1[1] = 0;
        led1[2] = 0;

        led2[0] = 0;
        led2[1] = 0;
        led2[2] = 0;

        led3[0] = 0;
        led3[1] = 0;
        led3[2] = 255;

        set_three_leds(led1, led2, led3);
        _delay_ms(1000);
    }
}