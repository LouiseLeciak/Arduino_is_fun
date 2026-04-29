
#include "utils.h"

// https://docs.buspirate.com/docs/devices/apa102-sk9822/
void set_led_color(uint8_t r, uint8_t g, uint8_t b) {
    // set 0 to clean
    for (int i = 0; i < 4; i++) {
        SPI_master_transmit(0x00);
    }

    // light the leds
    SPI_master_transmit(0xFF);  // brigthness => first byte read
    SPI_master_transmit(b);     // blue=> second byte read
    SPI_master_transmit(g);
    SPI_master_transmit(r);

    // end frame
    for (int i = 0; i < 4; i++) {
        SPI_master_transmit(0x00);
    }
}

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

void wheel(uint8_t pos) {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    pos = 255 - pos;

    if (pos < 85) {
        r = 255 - pos * 3;
        g = 0;
        b = pos * 3;
    } else if (pos < 170) {
        pos -= 85;
        r = 0;
        g = pos * 3;
        b = 255 - pos * 3;
    } else {
        pos -= 170;
        r = pos * 3;
        g = 255 - pos * 3;
        b = 0;
    }

    uint8_t l1[3] = {r, g, b};
    uint8_t l2[3] = {r, g, b};
    uint8_t l3[3] = {r, g, b};

    set_three_leds(l1, l2, l3);
}