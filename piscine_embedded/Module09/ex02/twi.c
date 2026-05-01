
#include "utils.h"

void i2c_init(void) {
    // TWI control register, TWI Enable Bit p240
    TWCR = (1 << TWEN);

    // prescaler = 1
    TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
    // p221/222 bit rate generator unit
    //((F_CPU / 100kHz)- 16) / 2 / prescaler
    //  TWI bit rate register
    TWBR = 72;
}

void i2c_stop(void) {
    // p225 7
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void i2c_start(void) {
    // p225
    //  TWINT = TWI interrupt flag p239
    //  TWI START condition bit
    //  enable the twi
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    // while TWINT = 0, wait
    while (!(TWCR & (1 << TWINT)));
}

void i2c_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)));
}

uint8_t i2c_read_nack(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
};
