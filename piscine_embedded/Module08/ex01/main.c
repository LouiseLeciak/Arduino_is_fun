
#include <avr/io.h>
#include <util/delay.h>

void SPI_master_init(void) {
    // p 172 tells to set MOSI
    // MOSI = M-User defined S-INput
    DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);

    // MISO
    DDRB &= ~(1 << DDB4);

    // still p172
    SPCR = (1 << SPE)      // enable spi
           | (1 << MSTR)   // master mod
           | (1 << SPR0);  // clock rate
}

// transmit byte per byte
void SPI_master_transmit(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1 << SPIF))) {
    };
}

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

int main() {
    SPI_master_init();

    while (1) {
        //red
        set_led_color(255, 0, 0);
        _delay_ms(1000);

        //green
        set_led_color(0, 255, 0);
        _delay_ms(1000);

        //blue 
        set_led_color(0, 0, 255);
        _delay_ms(1000);

        // yellow
        set_led_color(255, 255, 0);
        _delay_ms(1000);

        // cyan
        set_led_color(0, 255, 255);
        _delay_ms(1000);

        //magenta
        set_led_color(255, 0, 255);
        _delay_ms(1000);

        //white
        set_led_color(255, 255, 255);
        _delay_ms(1000);

    }
}