
#include "utils.h"

// https://www.nongnu.org/avr-libc/user-manual/group__avr__eeprom.html

//p 29 8.4 ATmega328p = 1Kb EEPROM= 1024 bytes
#define EEPROM_SIZE 1024

void display_state(void) {
    char ascii_version[17];
    // += 16 bcs 16 bytes
    //addr is a position in my EEPROM memory
    for (uint16_t addr = 0; addr < EEPROM_SIZE; addr += 16){
        // i is my position in my line
        uart_printhex_32((uint32_t)addr);
        uart_tx(' ');
        for (uint8_t i = 0; i < 16; i++){
            // +i to read every bytes from the 16
            // ptr to tell it where to read
            uint8_t value = ee_read(addr + i);
            uart_printhex(value);
            uart_tx(' ');
            // for ascii, check if printable char
            if (value >= 32 && value <= 126)
                ascii_version[i] = value;
            else
                ascii_version[i] = '.';
        }
        // print the ascii part
        ascii_version[16] = '\0';
        uart_tx('|');
        uart_printstr(ascii_version);
        uart_tx('|');

        uart_printstr("\n\r");
    }
}

int main() {
    uart_init();
    display_state();
    while (1) {}
}