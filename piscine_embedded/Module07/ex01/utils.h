
#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>
#include <avr/eeprom.h>
#define BAUD 115200
// p182 table 20-1
#define MYUBRR ((F_CPU / (8 * BAUD)) - 1)

#define EEPROM_SIZE 1024
#define RED "\x1b[31m"
#define RED_END "\x1b[0m"


void uart_init();
void uart_tx(char c);
char uart_rx(void);
void uart_printstr(const char* str);
void uart_printhex(uint8_t value);
void uart_printhex_32(uint32_t value);
void uart_printint(int val);
void uart_readline(char *buf);
uint8_t atoi_hex(char c);
bool is_hex_char(char c);
uint32_t atoi_hex_str(const char* str);
void display_state(void);

int ft_len(char * str);

#endif