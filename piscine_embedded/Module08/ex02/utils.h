

#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>
#include <util/delay.h>

// led
void set_led_color(uint8_t r, uint8_t g, uint8_t b) ;

// spi
void SPI_master_init(void);
void SPI_master_transmit(uint8_t data);

#endif