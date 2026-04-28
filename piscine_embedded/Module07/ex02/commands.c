
#include "utils.h"

uint8_t test_slot_validity(uint8_t *slot_id) {
    nodeconfig_ dummy;

        config_read(*slot_id, &dummy);
        if (check_magic(&dummy)){
            return 1;  // return the good slot
        }
        else if (check_integrity(&dummy)){
            return 2;
        }
    return 0;  // if no good slot, then slot 0
}

void status_cmd(uint8_t *slot_id) {
    uint8_t res = test_slot_validity(slot_id);
    nodeconfig_ temp;
    // no valid spot
    if (res == 1) {
        uart_printstr("Node unconfigured.\n\r");
        return;
    }
    config_read(res, &temp);
    if (res == 2) {
        uart_printstr("CRITICAL: Data corruption detected!.\n\r");
        return;
    }
    print_status(temp, *slot_id);
}

void config_reallocating(uint8_t* slot_id, nodeconfig_* config) {
    for (int i = 0; i < 4; i++) {
        if (config_write((*slot_id + i) % 4, config)) {
            return;
        }
    }
    uart_printstr("CRITICAL EEPROM FAILURE.\n\r");
    return;
}

void set_id_cmd(char* arg, uint8_t* slot_id) {
    nodeconfig_ temp;
    config_read(*slot_id, &temp);
    if (!check_magic(&temp) || !check_integrity(&temp)) {
        init_config(&temp);
    }
    temp.node_id = atoi_int(arg);
    config_reallocating(slot_id, &temp);
    return;
}

void set_prio_cmd(char* arg, uint8_t* slot_id) {
    nodeconfig_ temp;
    config_read(*slot_id, &temp);
    if (!check_magic(&temp) || !check_integrity(&temp)) {
        init_config(&temp);
    }
    temp.priority = atoi_int(arg);
    config_reallocating(slot_id, &temp);
    return;
}

void set_tag_cmd(char* arg, uint8_t slot_id) {
    nodeconfig_ temp;
    config_read(slot_id, &temp);
    if (!check_magic(&temp) || !check_integrity(&temp)) {
        init_config(&temp);
    }
    for (int i = 0; arg[i]; i++) {
        temp.tag[i] = arg[i];
    }
    config_reallocating(&slot_id, &temp);
    return;
}


void facto_reset_cmd(uint8_t slot_id) {
    nodeconfig_ temp = {};
    temp.magic_nb = MAGIC_NB / 2;
    config_write(slot_id, &temp);
    return;
}