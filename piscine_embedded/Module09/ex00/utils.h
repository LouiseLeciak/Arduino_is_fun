


#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>


#include <stdbool.h>


#define BAUD 115200
// p182 table 20-1
#define MYUBRR ((F_CPU / (8 * BAUD)) - 1)

#define ADDR 0b0100000
// // #define W ((ADDR << 1) | 0)
// #define R ((ADDR << 1) | 1)
#define CONFIG_PORT 0x06
#define OUTPUT_PORT 0x02
// bcs IO0_3
#define D9_MASK (1 << 3)

void uart_init();
void uart_tx(char c);
char uart_rx(void);
void uart_printstr(const char* str);
void uart_printhex(uint8_t value);
void uart_printint(int val);

void i2c_init(void);
void i2c_write(uint8_t data);
void i2c_start();
void i2c_stop(void);
uint8_t i2c_read_nack(void);

#endif
