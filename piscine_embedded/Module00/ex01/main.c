#include "/tmp/avr-libc/include/avr/io.h"

void turn_on_d1(void){
    DDRB |= (1 << PB0);
    PORTB |= (1 << PB0);
}
// DDR: Data Direction Register, I'm using it to specify if I want to read (0)
// or write (1), so fort the port PB0 I want the 0 to become a 1 to write
// PORT: is a data register for exiting data.
/*
PB7 PB6 PB5 PB4 PB3 PB2 PB1 PB0
 0   0   0   0   0   0   0   0
 |   |   |   |   |   |   |   |
 V   V   V   V   V   V   V   V
 0   0   0   0   0   0   0   1
*/


int main(void)
{
    turn_on_d1();
    return (0);
}
