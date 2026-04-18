
#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>

#define RED PD5
#define GREEN PD6
#define BLUE PD3

void init_leds();

void turn_blue();
void turn_green();
void turn_red();
void turn_yellow();
void turn_cyan();
void turn_magenta();
void turn_white();

#endif