#include <avr/io.h>

void my_timer(){
    for (long int i = 0; i < 150; i++){
        // volatile to avoid the compilo to delete this loop
        // even if if it does 'nothing'
        for (volatile unsigned int j = 0; j < F_CPU/6000; j++)
            ;
    }
}

int main (void){
    DDRB |= (1 << 0);

    // toggle using XOR because it can be 0 1 or 0 0 
    //https://qiriro.com/bme6163/static_files/notes/L3/Newbie%27s%20Guide%20to%20AVR%20Timers.pdf
    while (1){
        PORTB ^= (1 << 0);
        my_timer();
    }
}