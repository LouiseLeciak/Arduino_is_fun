
#include "utils.h"

// https://www.alldatasheet.com/html-pdf/310026/NXP/PCA9555/294/5/PCA9555.html

// SW3 -> IO0_0
// D11 -> IO0_1
// D10 -> IO0_2
// D9 -> IO0_3

// 1 input
// 0 output

// 2 = abged
// 0 1 2 3 4 5 6
// a b c d e f g
// 1 1 0 1 1 0 1

int main() {
    i2c_init();

    uint8_t digits = 0;
    uint8_t segment = 0;
    DDRD &= ~(1 << PD2);
    // IO1 = segments -> output
    // IO0 = digits -> output
    write_pca9555(CONFIG_PORT_0, 0x00);
    write_pca9555(CONFIG_PORT_1, 0x00);
    // pca9555_write_with_mask(OUTPUT_PORT_0, 0x00, D9_MASK);
    // pca9555_write_with_mask(OUTPUT_PORT_0, 0x00, D10_MASK);
    // pca9555_write_with_mask(OUTPUT_PORT_0, 0x00, D11_MASK);

    while (1) {
        digits = 0xFF;  // everyone off
        // 0 the one i dont want to touch, 1 the one i want to modify
        pca9555_write_with_mask(OUTPUT_PORT_0, digits, 0b11110000);
        digits &= ~(1 << 7);  // digit 4
        segment = display_2();
        write_pca9555(OUTPUT_PORT_1, segment);
        pca9555_write_with_mask(OUTPUT_PORT_0, digits, 0b11110000);

        // digit 3
        digits = 0xFF;
        pca9555_write_with_mask(OUTPUT_PORT_0, digits, 0b11110000);

        digits &= ~(1 << 6);  // set digit 3
        segment = display_4();
        write_pca9555(OUTPUT_PORT_1, segment);
        pca9555_write_with_mask(OUTPUT_PORT_0, digits, 0b11110000);
    }
}