

#include "utils.h"

void SPI_master_init(void) {
    // p 172 tells to set MOSI
    // MOSI = M-User defined S-INput
    DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);

    // MISO
    DDRB &= ~(1 << DDB4);

    // still p172
    SPCR = (1 << SPE)      // enable spi
           | (1 << MSTR)   // master mod
           | (1 << SPR0);  // clock rate
}

// transmit byte per byte
void SPI_master_transmit(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1 << SPIF))) {
    };
}