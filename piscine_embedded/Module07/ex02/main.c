
#include "utils.h"

// ? https://www.nongnu.org/avr-libc/user-manual/group__avr__eeprom.html

void set_tag(char* tag, nodeconfig_ config) {
    if (ft_len(tag) > 32) {
        uart_printstr("error: invalid tag\n\r");
        return;
    }
    for (int i = 0; tag[i]; i++) {
        config.tag[i] = tag[i];
    }
    (void)config;
    return;
}

void init_config(nodeconfig_* config) {
    config.magic_nb = MAGIC_NB;
    config.node_id = 12;
    set_tag("default_tag", config);
    config.priority = 0;
    config.checksum = integrity_calculate(
        (uint8_t*)&config, sizeof(config) - sizeof(config.checksum));
}

bool check_integrity(nodeconfig_* config) {
    uint16_t integrity = integrity_calculate(
        (uint8_t*)config, sizeof(*config) - sizeof(config->checksum));
    return (integrity == config->checksum);
}

bool check_magic(nodeconfig_* config) { return (config->magic_nb == MAGIC_NB); }

uint8_t test_slot() {
    nodeconfig_ dummy;
    for (int i = 0; i < 4;i++){
        config_read(i, &dummy);
        if (check_magic(&dummy) && check_integrity(&dummy)){
            return i;// return the good slot
        }
    }
    return 0;// if no good slot, then slot 0
}

int main() {
    nodeconfig_ config = {};
    uint8_t slot_id = test_slot();
    char line[16];
    uart_init();
    while (1) {
        uart_printstr("> ");
        uart_readline(line);
        parse_cmd(line, &slot_id);
    }
}