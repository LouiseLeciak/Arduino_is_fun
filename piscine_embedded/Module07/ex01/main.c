
#include "utils.h"

// https://www.nongnu.org/avr-libc/user-manual/group__avr__eeprom.html

// p 29 8.4 ATmega328p = 1Kb EEPROM= 1024 bytes
#define EEPROM_SIZE 1024
#define LEN 20

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
        if (idx >= LEN - 1) {
            uart_printstr("\n\r");
            return;
        }
        if (idx < LEN - 1) {
            buf[idx] = rx;
            idx++;
        }
        uart_tx(rx);
    }
}

uint8_t parse_line(char* line) {
    char addr_str[5] = {};  // max 4 hex + '\0'
    char val_str[3] = {};   // same idea

    int i = 0;
    int j = 0;

    // get the addr
    while (line[i] != ' ' && line[i] != '\0') {
        if (j >= 4) return 0;
        addr_str[j++] = line[i++];
    }
    addr_str[j] = '\0';

    // skip space
    while (line[i] == ' ') i++;

    // get value
    j = 0;
    while (line[i] != '\0') {
        if (j >= 2) return 0;
        val_str[j++] = line[i++];
    }
    val_str[j] = '\0';

    if (ft_len(addr_str) == 0 || ft_len(addr_str) > 4) return 0;

    if (ft_len(val_str) == 0 || ft_len(val_str) > 2) return 0;
    // convert
    addr = (uint16_t)atoi_hex_str(addr_str);
    value = (uint8_t)atoi_hex_str(val_str);

    if (addr >= EEPROM_SIZE) {
        uart_printstr("error: invalid addr\n\r");
        return 0;
    }

    return 1;
}

void display_red_state(volatile uint16_t red) {
    char ascii_version[17];
    // += 16 bcs 16 bytes
    // addr is a position in my EEPROM memory
    for (uint16_t addr = 0; addr < EEPROM_SIZE; addr += 16) {
        // i is my position in my line
        uart_printhex_32((uint32_t)addr);
        uart_tx(' ');
        for (uint8_t i = 0; i < 16; i++) {
            uint16_t current_addr = (addr + i);
            // +i to read every bytes from the 16
            // uint8_t * __p) ptr to tell it where to read
            uint8_t value = eeprom_read_byte((uint8_t*)current_addr);
            if (current_addr == red) {
                uart_printstr(RED);
                uart_printhex(value);
                uart_printstr(RED_END);
            } else {
                uart_printhex(value);
            }
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
    char line[16];
    uart_init();
    display_state();
    while (1) {
        uart_printstr("> ");
        uart_readline(line);

        if (!parse_line(line)) continue;

        uint8_t current_byte = ee_read(addr);
        if (current_byte != value) {
            // replace the byte if it's different from the value
            ee_write(addr, value);
            display_red_state(addr);
        }
    }
}