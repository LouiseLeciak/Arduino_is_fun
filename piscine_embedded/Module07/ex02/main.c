
#include "utils.h"

#define COL_RED "\x1b[31m"
#define COL_GREEN "\x1b[32m"
#define COL_YELLOW "\x1b[33m"
#define COL_BLUE "\x1b[34m"
#define COL_PURPLE "\x1b[35m"
#define COL_CYAN "\x1b[36m"
#define COL_RESET "\x1b[0m"
#define COL_GRAY "\x1b[38;5;248m"

#define COL_BOLD "\x1b[1m"

#define COL_BGBLACK "\x1b[47m"

#define ROW_SIZE (0x20)
#define BUFFER_SIZE (128)

uint16_t slot_address[] = {SLOT_0, SLOT_1, SLOT_2, SLOT_3};

#define tabsize(tab) (sizeof(tab) / sizeof(tab[0]))
#define member_size(type, member) (sizeof(((type*)0)->member))
#define setup_isField(field)                                       \
    static inline bool is_##field(uint16_t addr) {                 \
        for (uint8_t i = 0; i < tabsize(slot_address); i++) {      \
            if (addr >= slot_address[i] + offsetof(nodeconfig_, field) && \
                addr < slot_address[i] + offsetof(nodeconfig_, field) +   \
                           member_size(nodeconfig_, field))               \
                return true;                                       \
        }                                                          \
        return false;                                              \
    }

setup_isField(magic_nb);
setup_isField(tag);
setup_isField(node_id);
setup_isField(priority);
setup_isField(checksum);

char* get_color(uint16_t addr) {
    if (is_magic_nb(addr))
        return (COL_RED);
    else if (is_node_id(addr))
        return (COL_GREEN);
    else if (is_tag(addr))
        return (COL_BLUE);
    else if (is_priority(addr))
        return (COL_YELLOW);
    else if (is_checksum(addr))
        return (COL_PURPLE);
    else
        return (COL_GRAY);
}

void display_state(void) {
    // += 16 bcs 16 bytes
    // addr is a position in my EEPROM memory
    for (uint16_t addr = 0; addr < 0x100 /*EEPROM_SIZE*/; addr += 16) {
        // i is my position in my line
        uart_printhex_32((uint32_t)addr);
        uart_tx(' ');
        for (uint8_t i = 0; i < 16; i++) {
            // +i to read every bytes from the 16
            // uint8_t * __p) ptr to tell it where to read
            uint8_t value = ee_read(addr + i);
            uart_printstr(get_color(addr + i));
            uart_printhex(value);
            uart_printstr("\e[0m");
            uart_tx(' ');
        }
        
        uart_tx('|');
        for (uint8_t i = 0; i < 16; i++) {
            // +i to read every bytes from the 16
            // uint8_t * __p) ptr to tell it where to read
            uint8_t value = ee_read(addr + i);
            uart_printstr(get_color(addr + i));
            if (value >= 32 && value <= 126)
                uart_tx(value);
            else
                uart_tx('.');
            uart_printstr("\e[0m");
        }
        uart_tx('|');
        uart_printstr("\n\r");
    }
}

// ? https://www.nongnu.org/avr-libc/user-manual/group__avr__eeprom.html

void set_tag(t_tag tag, nodeconfig_* config) {
    tag.tag[TAG_LEN] = 0;
    config->tag = tag;
    return;
}

void init_config(nodeconfig_* config, uint8_t* slot_id) {
    uart_printstr("Setting up default value\r\n");
    config->magic_nb = MAGIC_NB;
    config->node_id = 12;
    set_tag((t_tag){"default_tag"}, config);
    config->priority = 0;
    config->checksum = integrity_calculate(config);
    config_write(*slot_id, config);
}

bool check_integrity(nodeconfig_* config) {
    uint16_t integrity = integrity_calculate(config);
    return (integrity == config->checksum);
}

bool check_magic(nodeconfig_* config) { return (config->magic_nb == MAGIC_NB); }

uint8_t test_slot() {
    nodeconfig_ dummy;
    for (int i = 0; i < 4; i++) {
        config_read(i, &dummy);
        if (check_magic(&dummy) && check_integrity(&dummy)) {
            return i;  // return the good slot
        }
    }
    return 0;  // if no good slot, then slot 0
}

int main() {
    uint8_t slot_id = test_slot();
    char line[16];
    uart_init();

    display_state();
    while (1) {
        uart_printstr("> ");
        uart_readline(line);
        parse_cmd(line, &slot_id);
    }
}