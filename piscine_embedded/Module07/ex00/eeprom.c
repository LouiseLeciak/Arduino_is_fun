
#include "utils.h"

uint8_t ee_read(uint16_t addr) {
    // waiting if writing
    while(EECR & (1 << EEPE));

    // set addr
    EEAR = addr;

    // start reading
    EECR |= (1 << EERE);

    // return the data we get
    return EEDR;
}

void ee_write(uint16_t addr, uint8_t value){
    // waiting for the last write to end
    while( EECR & (1 << EEPE));

    // set addr and value
    EEAR = addr;
    EEDR = value;

    EECR |= (1 << EEMPE);
    EECR |=(1 << EEPE);
}