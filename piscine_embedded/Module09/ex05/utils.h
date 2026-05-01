


#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#define BAUD 115200
// p182 table 20-1
#define MYUBRR ((F_CPU / (8 * BAUD)) - 1)

#define ADDR 0b0100000

// 0x00 = Input P0
// 0x01 = Input P1
// 0x02 = Output P0
// 0x03 = Output P1
// 0x04 = Polarity P0
// 0x05 = Polarity P1
// 0x06 = Config P0
// 0x07 = Config P1

#define INPUT_PORT_0 0x00
#define INPUT_PORT_1 0x01
#define OUTPUT_PORT_0 0x02
#define OUTPUT_PORT_1 0x03
#define CONFIG_PORT_0 0x06
#define CONFIG_PORT_1 0x07

#define OFF 0xFF

// bcs IO0_3
#define D9_MASK (1 << 3)
#define D10_MASK (1 << 2)
#define D11_MASK (1 << 1)
#define SW3_MASK (1 << 0)

typedef struct {
    uint8_t digit[4];
} global_state_seg;

void set_digit(uint8_t digit, uint8_t segments);
void display_state();

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

uint8_t read_pca9555(uint8_t reg);
void write_pca9555(uint8_t reg, uint8_t data);
void pca9555_write_with_mask(uint8_t reg, uint8_t data, uint8_t mask);

uint8_t display_0();
uint8_t display_1();
uint8_t display_2();
uint8_t display_3();
uint8_t display_4();
uint8_t display_5();
uint8_t display_6();
uint8_t display_7();
uint8_t display_8();
uint8_t display_9();

#endif
