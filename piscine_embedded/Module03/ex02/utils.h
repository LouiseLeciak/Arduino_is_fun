
#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>

#define RED PD5
#define GREEN PD6
#define BLUE PD3

void init_rgb();
void set_rgb(uint8_t r, uint8_t g, uint8_t b);
void wheel(uint8_t pos);



#endif