
#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>



void uart_init();
void uart_tx(char c);
char uart_rx(void);
void uart_printstr(const char* str);


#endif