

#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>
#include <util/delay.h>

// led
void set_led_color(uint8_t r, uint8_t g, uint8_t b) ;
void set_three_leds(uint8_t led1[3], uint8_t led2[3], uint8_t led3[3]);

// spi
void SPI_master_init(void);
void SPI_master_transmit(uint8_t data);

//adc
void ADC_init();
uint8_t ADC_read_8();

#endif