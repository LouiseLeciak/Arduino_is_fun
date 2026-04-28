
#include "utils.h"

void status_cmd(void, uint8_t slot_id) { uart_printstr("status hihi\n\r"); }

void config_reallocating(uint8_t* slot_id, nodeconfig_* config) {
    for (int i = 0; i < 4; i++) {
        if (config_write((*slot_id + i) % 4), config) {
            return;
        }
    }
    uart_printstr("CRITICAL EEPROM FAILURE.\n\r") return;
}

void set_id_cmd(char* arg, uint8_t* slot_id) {
    uart_printstr("DANS SET_ID\n\r");
    nodeconfig_ temp;
    config_read(slot_id, temp);
    if (!check_magic(temp) || !check_integrity(temp)) {
        init_config(&temp);
    }
    temp.node_id = arg;
    config_reallocating(slot_id, &temp);
    return;
}

void set_prio_cmd(char* arg, uint8_t slot_id) {
    uart_printstr("DANS SET_PRIO\n\r");
    nodeconfig_ temp;
    config_read(slot_id, temp);
    if (!check_magic(temp) || !check_integrity(temp)) {
        init_config(&temp);
    }
    temp.priority = arg;
    config_reallocating(slot_id, &temp);
    return;
}

void set_tag_cmd(char* arg, uint8_t slot_id) {
    uart_printstr("DANS SET_TAG\n\r");
        nodeconfig_ temp;
    config_read(slot_id, temp);
    if (!check_magic(temp) || !check_integrity(temp)) {
        init_config(&temp);
    }
    for (int i = 0; arg[i]; i++)
        temp.tag[i] = arg[i];
    config_reallocating(slot_id, &temp);
    return;
}

void facto_reset_cmd(void, uint8_t slot_id) {
    uart_printstr("facto reset\n\r");
    ee_write(&config, 10);
}

void check_command(char* cmd, char* arg, uint8_t* slot_id) {
    if (comp_str(cmd, "STATUS")) {
        if (arg[0]) uart_printstr("error: no arg needed for STATUS\n\r");
        status_cmd(slot_id);
    }

    else if (comp_str(cmd, "SET_ID")) {
        if (!is_nb_str(arg)) {
            uart_printstr("error: need a number for id\n\r");
            return;
        }
        set_id_cmd(arg, slot_id);
    }

    else if (comp_str(cmd, "SET_PRIO")) {
        if (!is_nb_str(arg)) {
            uart_printstr("error: need a number for prio\n\r");
            return;
        }
        set_prio_cmd(arg, slot_id);
    }

    else if (comp_str(cmd, "SET_TAG")) {
        if (!is_tag_correct(arg)) {
            uart_printstr("error: need alphnum or - _ for tag\n\r");
            return;
        }
        set_tag_cmd(arg, slot_id);
    }

    else if (comp_str(cmd, "FACTORY_RESET")) {
        if (arg[0]) uart_printstr("error: no arg needed for FACTORY_RESET\n\r");
        facto_reset_cmd(slot_id);
    }

    else
        uart_printstr("error: wrong input\n\r");
}

void parse_cmd(char* str, uint8_t* slot_id) {
    char cmd[CMD_S];
    char arg[ARG_S];

    int i = 0;
    int j = 0;
    while (str[i] && str[i] == ' ') {
        i++;
    }

    while (str[i] && str[i] != ' ') {
        cmd[j] = str[i];
        i++;
        j++;
    }
    cmd[j] = '\0';

    if (str[i]) i++;

    j = 0;
    while (str[i] && str[i] != ' ') {
        arg[j] = str[i];
        i++;
        j++;
    }
    arg[j] = '\0';
    if (str[i] != '\0') return;
    uart_printstr("cmd: ");
    uart_printstr(cmd);
    uart_printstr("\n\r");
    uart_printstr("arg: ");
    uart_printstr(arg);
    uart_printstr("\n\r");
    check_command(cmd, arg, slot_id);
}