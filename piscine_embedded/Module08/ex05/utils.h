

#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>
#include <util/delay.h>

#define RED 0
#define GREEN 1
#define BLUE 2

#define D6 0
#define D7 1
#define D8 2

#define BUTTONS ((1 << PD2) | (1 << PD4))

extern volatile uint8_t leds[3][3];

// led
void set_led_color(uint8_t r, uint8_t g, uint8_t b) ;
void wheel(uint8_t pos);
void get_led(uint8_t r, uint8_t g, uint8_t b, char led);

// spi
void SPI_master_init(void);
void SPI_master_transmit(uint8_t data);

//adc
void ADC_init();
uint8_t ADC_read_8();



#endif