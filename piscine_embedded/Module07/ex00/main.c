
#include "utils.h"

// https://www.nongnu.org/avr-libc/user-manual/group__avr__eeprom.html

void display_state(void) {
    // += 16 bcs
    for (uint16_t addr = 0; addr< size de mon eeprom; addr += 16){
        j'affiche adresse ?
        eeprom_read_byte(); 16 bits ?
        afficher en hex;
        afficher ascii;
    }
}

int main() {

    while (1) {}
}