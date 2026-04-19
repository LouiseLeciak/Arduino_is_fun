
#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>

#define RED PD5
#define GREEN PD6
#define BLUE PD3

#define LEN 7
#define BAUD 115200
// p182 table 20-1
#define MYUBRR ((F_CPU / (8 * BAUD)) - 1)


void init_rgb();
void set_rgb(uint8_t r, uint8_t g, uint8_t b);
void uart_init();
void uart_tx(char c);
char uart_rx(void);
void uart_printstr(const char* str);
uint8_t atoi_hex(char c);
bool is_hex_char(char c);
void rgb_from_buffer(void);
uint8_t hex_to_bit(char *hex);

#endif