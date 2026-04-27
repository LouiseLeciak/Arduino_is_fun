
#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>
#include <avr/eeprom.h>
#define BAUD 115200
// p182 table 20-1
#define MYUBRR ((F_CPU / (8 * BAUD)) - 1)


void uart_init();
void uart_tx(char c);
char uart_rx(void);
void uart_printstr(const char* str);
void uart_printhex(uint8_t value);
void uart_printhex_32(uint32_t value);
void uart_printint(int val);

uint8_t ee_read(uint16_t addr);
void ee_write(uint16_t addr, uint8_t value);

#endif