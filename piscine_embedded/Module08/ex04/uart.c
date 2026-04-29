#include "utils.h"

// screen /dev/ttyUSB0 115200
// ctrl a puis k puis y

void uart_init() {
    // set baudrate
    // p180 table 20-1
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)MYUBRR;
    // enable transmiter
    // p202 bit 4 Reveiver enable
    UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
    // 8n1 => 8 bits de data, no parite, 1 bit de stop
    // p203 20-11
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

    // p182 tab 20-1 for double speed to lower fail chance
    UCSR0A |= (1 << U2X0);
}

void uart_tx(char c) {
    // waiting for the buffer to be empty
    while (!(UCSR0A & (1 << UDRE0))) {
    }
    // put data (char) into buffer
    UDR0 = c;
}

char uart_rx(void) {
    // p189, wait for data to be received
    while (!(UCSR0A & (1 << RXC0))) {
    }
    // get and return received data from buffer
    return UDR0;
}

void uart_printstr(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        uart_tx(str[i]);
    }
}

void uart_printhex(uint8_t value) {
    char hex[] = "0123456789abcdef";
    char buf[3];

    uint8_t first = value / 16;
    uint8_t sec = value % 16;

    buf[0] = hex[first];
    buf[1] = hex[sec];
    buf[2] = '\0';

    uart_printstr(buf);
}

void uart_printint(int val) {
    char buffer[10];
    int i = 0;

    // neg numbers
    if (val < 0) {
        uart_tx('-');
        val = -val;
    }

    // 0
    if (val == 0) {
        uart_tx('0');
        return;
    }

    // convert in string
    while (val > 0) {
        buffer[i++] = (val % 10) + '0';
        val /= 10;
    }

    // put it back in the right order
    for (int j = i - 1; j >= 0; j--) {
        uart_tx(buffer[j]);
    }
}

void uart_printhex_32(uint32_t value) {
    char hex[] = "0123456789abcdef";
    char buf[9];

    uint8_t i1 = (value / 268435456) % 16;  // 16^7
    uint8_t i2 = (value / 16777216) % 16;   // 16^6
    uint8_t i3 = (value / 1048576) % 16;    // 16^5
    uint8_t i4 = (value / 65536) % 16;      // 16^4
    uint8_t i5 = (value / 4096) % 16;       // 16^3
    uint8_t i6 = (value / 256) % 16;        // 16^2
    uint8_t i7 = (value / 16) % 16;         // 16^1
    uint8_t i8 = value % 16;                // 16^0

    buf[0] = hex[i1];
    buf[1] = hex[i2];
    buf[2] = hex[i3];
    buf[3] = hex[i4];
    buf[4] = hex[i5];
    buf[5] = hex[i6];
    buf[6] = hex[i7];
    buf[7] = hex[i8];
    buf[8] = '\0';

    uart_printstr(buf);
}

void display_state(void) {
    char ascii_version[17];
    // += 16 bcs 16 bytes
    // addr is a position in my EEPROM memory
    for (uint16_t addr = 0; addr < EEPROM_SIZE; addr += 16) {
        // i is my position in my line
        uart_printhex_32((uint32_t)addr);
        uart_tx(' ');
        for (uint8_t i = 0; i < 16; i++) {
            // +i to read every bytes from the 16
            // uint8_t * __p) ptr to tell it where to read
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

        // if (!is_hex_char(rx) && rx != ' ') {
        //     // uart_printstr("error: not hex");
        //     continue;
        // }
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

void print_status(nodeconfig_ temp, uint8_t res) {
    uart_printstr("Node ID: ");
    uart_printint(temp.node_id);
    uart_printstr("\n\rPriority: ");
    uart_printint(temp.priority);
    uart_printstr("\n\rSlot: ");
    uart_printint(res);
    uart_printstr("\n\rTag: \"");
    uart_printstr(temp.tag);
    uart_printstr("\"\n\r");
}

