
#include "utils.h"

volatile global_state_seg seg_state;

void set_digit(uint8_t digit, uint8_t segments){
    seg_state.digit[digit] = segments;
}

void display_state(){
    for (int i =0; i < 4; i++){
        pca9555_write_with_mask(OUTPUT_PORT_0, 0xFF, 0b11110000);// off led
        write_pca9555(OUTPUT_PORT_1, seg_state.digit[i]);
        pca9555_write_with_mask(OUTPUT_PORT_0, ~(1 << (i + 4)), 0b11110000);
    }
}