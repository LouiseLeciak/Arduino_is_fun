
#include "utils.h"

// https://www.alldatasheet.com/html-pdf/310026/NXP/PCA9555/294/5/PCA9555.html

void display_counter(uint8_t count) {
    uint8_t leds = OFF;

    // if count = 5 so 101 then d9 on d10 off and d11 on
    if (count & 0x01){ // 0x01= 0001
        leds &= ~D9_MASK;
    }
    if (count & 0x02){// 0x02 = 0010
        leds &= ~D10_MASK;
    }
    if (count & 0x04){// 0x04 = 0100
        leds &= ~D11_MASK;
    }

    write_pca9555(OUTPUT_PORT, leds); // send all the leds at once
}

// SW3 -> IO0_0
// D11 -> IO0_1
// D10 -> IO0_2
// D9 -> IO0_3

// 1 input
// 0 output

int main() {
    i2c_init();

    uint8_t input = 0;
    uint8_t counter = 0;
    uint8_t last = SW3_MASK;
    uint8_t current = 0;

    // SW3 inout, leds output
    write_pca9555(CONFIG_PORT, SW3_MASK);

    while (1) {
        input = read_pca9555(INPUT_PORT); // read my input( so watch mu button)
        current = input & SW3_MASK;

        // if i detect a press
        if (last && !current){
            counter++;
            if (counter > 7){
                counter = 0;
            }
        }
        last = current;

        display_counter(counter);

        _delay_ms(30);
    }
}