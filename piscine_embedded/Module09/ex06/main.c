
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
    ADC_init();

    write_pca9555(CONFIG_PORT_0, 0x00);
    write_pca9555(CONFIG_PORT_1, 0x00);

    uint16_t count = 0;
    uint8_t d0 = 0;
    uint8_t d1 = 0;
    uint8_t d2 = 0;
    uint8_t d3 = 0;

    uint8_t nb_pool[10] = {display_0(), display_1(), display_2(), display_3(),
                           display_4(), display_5(), display_6(), display_7(),
                           display_8(), display_9()};
    while (1) {
        count = ADC_read_10();

        d3 = count % 10;
        d2 = (count / 10) % 10;
        d1 = (count / 100) % 10;
        d0 = (count / 1000) % 10;

        set_digit(0, nb_pool[d0]);
        set_digit(1, nb_pool[d1]);
        set_digit(2, nb_pool[d2]);
        set_digit(3, nb_pool[d3]);

        display_state();
    }
}