#include"utils.h"
#include <util/delay.h>



int main(void) {
    uint8_t pos = 0;
    init_rgb();
    while (1){
        wheel(pos++);
        _delay_ms(10);
    }
        
}