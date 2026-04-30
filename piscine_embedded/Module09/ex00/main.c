
#include "utils.h"

uint8_t read_pca9555(uint8_t reg) {
    i2c_start();
    // i tell the expender wich reg i want to interact write
    i2c_write((ADDR << 1) | 0);
    i2c_write(reg);

    // then i read
    i2c_start();
    i2c_write((ADDR << 1) | 1);
    i2c_stop();

    return i2c_read_nack();
}

void write_pca9555(uint8_t reg, uint8_t data) {
    i2c_start();
    // addr + turn the write
    i2c_write((ADDR << 1) | 0);
    // reg of the pca9555
    i2c_write(reg);
    // my data
    i2c_write(data);
    i2c_stop();
}

// 0x00 = Input P0
// 0x01 = Input P1
// 0x02 = Output P0
// 0x03 = Output P1
// 0x04 = Polarity P0
// 0x05 = Polarity P1
// 0x06 = Config P0
// 0x07 = Config P1

int main() {
    i2c_init();

    // every pin are 0 so output
    write_pca9555(CONFIG_PORT, 0x00);

    while (1) {
        // led on (D9 = 1)
        write_pca9555(OUTPUT_PORT, ~D9_MASK);
        _delay_ms(500);

        // led off
        // schema -> invert logo, means ff turn of and 00 turn on
        write_pca9555(OUTPUT_PORT, 0xFF);
        _delay_ms(500);
    }
}
