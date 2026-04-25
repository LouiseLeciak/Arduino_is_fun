
#include "utils.h"

void i2c_init(void) {
    // TWI control register, TWI Enable Bit p240
    TWCR |= (1 << TWEN);

    // prescaler = 1
    TWSR &= ~(TWPS1 << 1) & ~(TWPS0 << 1);

    // p221/222 bit rate generator unit
    //((F_CPU / 100kHz)- 16) / 2 * prescaler
    //  TWI bit rate register
    TWBR = 72;
}

void i2c_start(uint8_t state) {
    // p225
    //  TWINT = TWI interrupt flag p239
    //  TWI START condition bit
    //  enable the twi
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    // while TWINT = 0, wait
    while (!(TWCR & (1 << TWINT)));

    if (TW_STATUS != TW_START) uart_printstr("start error\n\r");

    // where i'm sending data
    // start transmission with AHT20
    // p225 (SLA_W)
    if (state == WRITE)
        TWDR = AHT20_ADDR_W;
    else if (state == READ)
        TWDR = AHT20_ADDR_R;
    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)));

    // https://www.nongnu.org/avr-libc/user-manual/group__util__twi.html
    //  p227
    // TW_STATUS = (TWSR & 0xf8)
    if (state == WRITE) {
        if (TW_STATUS != TW_MT_SLA_ACK)
            uart_printstr("master transmitter ack error\n\r");
    } else if (state == READ) {
        if (TW_STATUS != TW_MR_SLA_ACK)
            uart_printstr("master transmitter ack error\n\r");
    }
}

void i2c_stop(void) {
    // p225 7
    TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}
