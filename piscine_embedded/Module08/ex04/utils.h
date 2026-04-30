

#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>
#include <util/delay.h>

#define BAUD 115200
// p182 table 20-1
#define MYUBRR ((F_CPU / (8 * BAUD)) - 1)
// led
void set_led_color(uint8_t r, uint8_t g, uint8_t b) ;
void set_three_leds(uint8_t led1[3], uint8_t led2[3], uint8_t led3[3]);
void wheel(uint8_t pos);
void get_led(uint8_t r, uint8_t g, uint8_t b, char led);

// spi
void SPI_master_init(void);
void SPI_master_transmit(uint8_t data);

//adc
void ADC_init();
uint8_t ADC_read_8();


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



/// UTILS ///
int ft_len(char* str);
bool comp_str(const char* str1, const char* str2);
uint32_t atoi_int(const char* str);
bool is_nb_neg(char* str);
uint8_t hex_to_bit(char *hex);


#endif