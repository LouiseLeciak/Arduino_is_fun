
#include "utils.h"

uint8_t test_slot_validity(uint8_t* slot_id) {
    nodeconfig_ dummy;

    config_read(*slot_id, &dummy);
    if (!check_magic(&dummy)) {
        return 10;  // return the good slot
    }
    if (!check_integrity(&dummy)) {
        return 11;
    }
    return 0;  // if no good slot
}

void status_cmd(uint8_t* slot_id) {
    uint8_t res = test_slot_validity(slot_id);
    nodeconfig_ temp;
    // no valid spot
    if (res == 10) {
        uart_printstr("Node unconfigured.\n\r");
        return;
    }
    if (res == 11) {
        uart_printstr("CRITICAL: Data corruption detected!.\n\r");
        return;
    }
    config_read(*slot_id, &temp);
    print_status(temp, *slot_id);
}

void config_reallocating(uint8_t* slot_id, nodeconfig_* config) {
    for (int i = 0; i < 4; i++) {
        if (config_write((*slot_id + i) % 4, config)) {
            *slot_id = (*slot_id + i) % 4;
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
        init_config(&temp, slot_id);
    }
    temp.node_id = atoi_int(arg);
    temp.checksum = integrity_calculate(&temp);
    config_reallocating(slot_id, &temp);
    return;
}

void set_prio_cmd(char* arg, uint8_t* slot_id) {
    nodeconfig_ temp;
    config_read(*slot_id, &temp);
    if (!check_magic(&temp) || !check_integrity(&temp)) {
        init_config(&temp, slot_id);
    }
    temp.priority = atoi_int(arg);
    temp.checksum = integrity_calculate(&temp);
    config_reallocating(slot_id, &temp);
    return;
}

void set_tag_cmd(char* arg, uint8_t* slot_id) {
    nodeconfig_ temp;
    config_read(*slot_id, &temp);
    if (!check_magic(&temp) || !check_integrity(&temp)) {
        // uart_printstr("\n\rcheck magic res : ");
        // uart_printint(check_magic(&temp));
        // uart_printstr("\n\rcheck inte res : ");
        // uart_printint(check_integrity(&temp));
        // uart_printstr("\r\n");
        init_config(&temp, slot_id);
    }
    if (!is_tag_correct(arg))
    {
        uart_printstr("wrong tag format :(\r\n");
        return;
    }
    temp.tag = (t_tag){};
    for (uint8_t i=0; i < TAG_LEN && arg[i]; i++)\
        temp.tag.tag[i] = arg[i];
    temp.checksum = integrity_calculate(&temp);
    config_reallocating(slot_id, &temp);
    return;
}

void facto_reset_cmd(uint8_t slot_id) {
    nodeconfig_ temp = {};
    temp.magic_nb = MAGIC_NB -1;
    config_write(slot_id, &temp);
    return;
}

void set_slot_cmd(char *arg, uint8_t* slot_id)
{
    if (arg[0] <'0' || arg[0]>'3')
    {
        uart_printstr("slot must be between 0 and 3\r\n");
        return;
    }

    *slot_id = arg[0]-'0';
    uart_printstr("Changed slot to ");
    uart_printint(*slot_id);
    uart_printstr("\r\n");
}


void get_slot_cmd(uint8_t* slot_id)
{
    uart_printstr("Current slot is ");
    uart_printint(*slot_id);
    uart_printstr("\r\n");
}

void kill_slot_cmd(uint8_t* slot_id)
{
    nodeconfig_ temp;

    config_read(*slot_id, &temp);
    // bit flip exemple
    ((uint8_t *)&temp)[11] ^= 1;
    config_write(*slot_id, &temp);

    uart_printstr("Flipped a bit :D\r\n");
}