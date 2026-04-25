
#include "utils.h"

// https://www.nongnu.org/avr-libc/user-manual/group__util__twi.html  STATUS
// https://datasheet4u.com/pdf/1551700/AHT20.pdf AHT20
// p12 AHT20 7.4 Sensor reading process

// to write the content of the TWDR register and send it to the temp sensor
// p225 5-2 -> 7-1
// Master Transmitter
void i2c_write(unsigned char data) {
    // Load data into TWDR register
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);

    // wait for the TWINT flag set
    while (!(TWCR & (1 << TWINT))){}

    if (TW_STATUS != TW_MT_DATA_ACK) {
        uart_printstr("error MT_DATA_ACK\n\r");
        i2c_stop();
    }
}

void print_hex_value(char c) { 
    char hex[] = "0123456789ABCDEF";
    char buf[3];

    unsigned char value = (unsigned char)c;

    buf[0] = hex[value / 16];
    buf[1] = hex[value % 16];
    buf[2] = '\0';

    uart_printstr(buf);
}

// Master Receiver Mode (p.229)
void i2c_read(void) {
    // we want 7 for 7 bytes p13
    for (int i = 0; i <= 5; i++) {
        TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
        while (!(TWCR & (1 << TWINT))) {
        }
        print_hex_value(TWDR);
        uart_tx(' ');
    }

    TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT))) {
    }

    print_hex_value(TWDR);
    uart_printstr("\r\n");
}

int main() {
    uart_init();
    i2c_init();
    while (1) {
        i2c_start(WRITE);
        _delay_ms(10);
        // cf 7.4 p12 AHT20
        i2c_write(0xAC);
        i2c_write(0x33);
        i2c_write(0x00);
        i2c_stop();
        _delay_ms(80);
        i2c_start(READ);
        i2c_read();
        i2c_stop();
        _delay_ms(100);
    }
}