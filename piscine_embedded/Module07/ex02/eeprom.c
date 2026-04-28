
#include "utils.h"

// p29 EEPROM Data Memory
uint8_t ee_read(uint16_t addr) {
    // waiting if writing
    while (EECR & (1 << EEPE));

    // set addr
    EEAR = addr;

    // start reading
    EECR |= (1 << EERE);

    // return the data we get
    return EEDR;
}

bool ee_write(uint16_t addr, uint8_t value) {
    if (ee_read(addr) == value) return true;
    // waiting for the last write to end
    while (EECR & (1 << EEPE));

    // set addr and value
    EEAR = addr;
    EEDR = value;

    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
    // read if the write succeed
    if (ee_read(addr) == value)
        return true;
    else
        return false;
}

// en fonction du slot number, l'enum get la bonne adresse
bool config_write(uint8_t slot_id, nodeconfig_* config) {
    uint16_t addr = 0;
    if (slot_id == 0) {
        addr = SLOT_0;
    } else if (slot_id == 1) {
        addr = SLOT_1;
    } else if (slot_id == 2) {
        addr = SLOT_2;
    } else if (slot_id == 3) {
        addr = SLOT_3;
    }

    for (uint16_t i = 0; i < sizeof(*config); i++) {
        // if write didnt get well
        if (!ee_write(addr + i, ((uint8_t*)config)[i]))  // print byte i in struct
            return false;
    }
    return true;
}

// read a config in eeprom
void config_read(uint8_t slot_id, nodeconfig_* config) {
    uint16_t addr = 0;
    if (slot_id == 0) {
        addr = SLOT_0;
    } else if (slot_id == 1) {
        addr = SLOT_1;
    } else if (slot_id == 2) {
        addr = SLOT_2;
    } else if (slot_id == 3) {
        addr = SLOT_3;
    }

    for (uint16_t i = 0; i < sizeof(*config); i++) {
        ((uint8_t*)config)[i] = ee_read(addr + i);  // get byte i instruct
    }
}