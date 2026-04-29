
#ifndef UTILS_H
#define UTILS_H

#include <avr/eeprom.h>
#include <avr/io.h>
#define BAUD 115200
// p182 table 20-1
#define MYUBRR ((F_CPU / (8 * BAUD)) - 1)

#define EEPROM_SIZE 1024
#define RED "\x1b[31m"
#define RED_END "\x1b[0m"
#define LEN 100
#define TAG_LEN 32
#define CMD_S 16
#define ARG_S 32
#define MAGIC_NB 0xABCD

typedef struct {char tag[TAG_LEN+1];} t_tag;

typedef struct{
    uint16_t magic_nb;
    uint32_t node_id;
    int16_t priority;
    t_tag tag; // max length 32 + 1 '\0'
    uint16_t checksum;
} nodeconfig_;

typedef enum{
    SLOT_0 = 0x00,
    SLOT_1 = 0x40,
    SLOT_2 = 0x80,
    SLOT_3 = 0xc0,
} slots_;

/// UART ///
void uart_init();
void uart_tx(char c);
char uart_rx(void);
void uart_printstr(const char* str);
void uart_printhex(uint8_t value);
void uart_printhex_32(uint32_t value);
void uart_printint(int val);
void uart_readline(char* buf);
uint8_t atoi_hex(char c);
bool is_hex_char(char c);
uint32_t atoi_hex_str(const char* str);
void display_state(void);
void print_status(nodeconfig_ temp, uint8_t res);

/// UTILS ///
int ft_len(char* str);
bool comp_str(const char* str1, const char* str2);
uint32_t atoi_int(const char* str);
bool is_nb_neg(char* str);
void w_fail_cmd(char* arg);

/// EEPROM ///
uint8_t ee_read(uint16_t addr);
bool ee_write(uint16_t addr, uint8_t value);
bool config_write(uint8_t slot_id, nodeconfig_* config);
void config_read(uint8_t slot_id, nodeconfig_* config);
bool check_magic(nodeconfig_* config);
bool check_integrity(nodeconfig_* config);
void init_config(nodeconfig_* config, uint8_t *slot_id);

/// PARSING ///
uint8_t parse_line(char* line);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
void parse_cmd(char* str, uint8_t *slot_id);
void check_command(char* cmd, char* arg, uint8_t *slot_id);
bool is_tag_correct(char* str);
bool is_nb_str(char* str);
// void set_tag(char *tag, nodeconfig_ config);
uint16_t integrity_calculate(nodeconfig_ *config);

/// COMMANDS ///
uint8_t test_slot_validity(uint8_t *slot_id);
void status_cmd(uint8_t *slot_id);
void config_reallocating(uint8_t* slot_id, nodeconfig_* config);
void set_id_cmd(char* arg, uint8_t* slot_id);
void set_prio_cmd(char* arg, uint8_t *slot_id);
void set_tag_cmd(char* arg, uint8_t *slot_id);
void facto_reset_cmd(uint8_t slot_id);
void set_slot_cmd(char *arg, uint8_t* slot_id);
void get_slot_cmd(uint8_t* slot_id);
void kill_slot_cmd(uint8_t* slot_id);

void set_tag(t_tag tag, nodeconfig_* config);

#endif