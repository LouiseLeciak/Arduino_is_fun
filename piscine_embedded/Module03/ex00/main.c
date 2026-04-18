#include"utils.h"
#include <util/delay.h>

void rotation(void) {
    turn_red();
    _delay_ms(1000);
    turn_green();
    _delay_ms(1000);
    turn_blue();
    _delay_ms(1000);
}

int main(void) {
    init_leds();
    while (1)
        rotation();
}