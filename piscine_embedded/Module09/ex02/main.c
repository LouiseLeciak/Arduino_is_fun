
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
    uint8_t segment = 0x00;
    // IO1 = segments -> output
    // IO0 = digits -> output
    write_pca9555(CONFIG_PORT_0, 0x00);
    write_pca9555(CONFIG_PORT_1, 0x00);
    while (1) {
        // activate only the CA4
        digits = 0xFF;        // everyone off
        digits &= ~(1 << 7);  // set only the last one IO0_7
        write_pca9555(OUTPUT_PORT_0, digits);

        // display 2
        segment |= (1 << 0);  // a
        segment |= (1 << 1);  // b
        segment |= (1 << 3);  // d
        segment |= (1 << 4);  // e
        segment |= (1 << 6);  // g

        write_pca9555(OUTPUT_PORT_1, segment);
    }
}
