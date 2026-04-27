
#include "utils.h"

// https://www.nongnu.org/avr-libc/user-manual/group__avr__eeprom.html

// p 29 8.4 ATmega328p = 1Kb EEPROM= 1024 bytes
#define EEPROM_SIZE 1024

volatile uint16_t addr;
volatile uint8_t value;

void uart_readline(char* buf) {
    int idx = 0;

    while (1) {
        char rx = uart_rx();
        // x7F = delete in ascii
        if (rx == '\x7F' || rx == '\b') {
            if (idx > 0) {
                buf[--idx] = '\0';
                uart_tx('\b');
                uart_tx(' ');
                uart_tx('\b');
            }
            continue;
        }

        // if enter then finish and change line
        else if (rx == '\r') {
            buf[idx++] = '\0';
            uart_printstr("\n\r");            
            return;
        }

        if (!is_hex_char(rx) && rx != ' ') {
            // uart_printstr("error: not hex");
            continue;
        }
        buf[idx++] = rx;
        uart_tx(rx);
    }
}


void parse_line(char *line) {
    char addr_str[8];
    char val_str[4];

    int i = 0;
    int j = 0;

    // get the addr
    while (line[i] != ' ' && line[i] != '\0') {
        addr_str[j++] = line[i++];
    }
    addr_str[j] = '\0';

    // skip space
    while (line[i] == ' ') i++;

    // get value
    j = 0;
    while (line[i] != '\0') {
        val_str[j++] = line[i++];
    }
    val_str[j] = '\0';

    // convert
    addr = (uint16_t) atoi_hex_str(addr_str);
    value = (uint8_t) atoi_hex_str(val_str);
}


int main() {
    char line[16];
    uart_init();
    display_state();
    while (1) {
        uart_printstr("> ");
        uart_readline(line);
        parse_line(line);
        if ( addr >= EEPROM_SIZE){
            uart_printstr("error: invalid addr\n\r");
            continue;
        }
        uint8_t current_byte = eeprom_read_byte((uint8_t*)addr);
        if (current_byte != value) {
            // replace the byte if it's different from the value
            eeprom_write_byte((uint8_t*)addr, value);
            display_state();
        }
    }
}