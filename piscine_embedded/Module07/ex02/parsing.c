
#include "utils.h"

void check_command(char* cmd, char* arg, uint8_t* slot_id) {
    if (comp_str(cmd, "STATUS")) {
        if (arg[0]) {
            uart_printstr("error: no arg needed for STATUS\n\r");
            return;
        }
        status_cmd(slot_id);
    }

    else if (comp_str(cmd, "GET_SLOT")) {
        if (arg[0]) {
            uart_printstr("error: no arg needed for GET_SLOT\n\r");
            return;
        }
        get_slot_cmd(slot_id);
    }

    else if (comp_str(cmd, "SET_SLOT")) {
        if (!is_nb_str(arg)) {
            uart_printstr("error: need a number for slot\n\r");
            return;
        }
        set_slot_cmd(arg, slot_id);
    }

    else if (comp_str(cmd, "KILL_SLOT")) {
        if (arg[0]) {
            uart_printstr("error: no arg needed for KILL_SLOT\n\r");
            return;
        }
        kill_slot_cmd(slot_id);
    }

    else if (comp_str(cmd, "W_FAIL")) {
        if (!is_nb_str(arg)) {
            uart_printstr("error: need a number for slot\n\r");
            return;
        }
        w_fail_cmd(arg);
    }

    else if (comp_str(cmd, "SET_ID")) {
        if (!is_nb_str(arg)) {
            uart_printstr("error: need a number for id\n\r");
            return;
        }
        set_id_cmd(arg, slot_id);
    }

    else if (comp_str(cmd, "SET_PRIO")) {
        if (!is_nb_neg(arg)) {
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
        if (arg[0]) {
            uart_printstr("error: no arg needed for FACTORY_RESET\n\r");
            return;
        }
        facto_reset_cmd(*slot_id);
    }

    else if (comp_str(cmd, "DISPLAY")) {
        if (arg[0]) {
            uart_printstr("error: no arg needed for FACTORY_RESET\n\r");
            return;
        }
        display_state();
    } else
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
    check_command(cmd, arg, slot_id);
}