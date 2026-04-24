
#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>
#include <util/delay.h>

#define LEDS ((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4))

#define BAUD 115200
// p182 table 20-1
#define MYUBRR ((F_CPU / (8 * BAUD)) - 1)

#define SET_RV1 0
#define SET_LDR 1
#define SET_NTC 2


void uart_init();
void uart_tx(char c);
char uart_rx(void);
void uart_printstr(const char* str);
void uart_printhex(uint8_t value);
void uart_printint(int val);

void ADC_init();
uint8_t ADC_read();

void init_rgb();
void set_rgb(uint8_t r, uint8_t g, uint8_t b);
void wheel(uint8_t pos);

#endif