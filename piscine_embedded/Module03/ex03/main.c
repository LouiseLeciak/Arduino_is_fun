#include "utils.h"

char rgb_buff[LEN] = {0};
volatile int idx = 0;

typedef enum State{
    INIT,
    WAIT_RGB,
    RGB
}states_t;

volatile states_t state = INIT;

void rgb_from_buffer(void) {

    uint8_t r = hex_to_bit(&rgb_buff[0]); // RR so the 2 first 
    uint8_t g = hex_to_bit(&rgb_buff[2]); // GG
    uint8_t b = hex_to_bit(&rgb_buff[4]); // BB

    set_rgb(r, g, b);
}

void __attribute__((signal, used)) USART_RX_vect(void){
    // get the character
    char rx = uart_rx();

    // if backspace, delete last char and update what's on screen    
    if (state == WAIT_RGB) {
        // x7F = delete in ascii
        if (rx == '\x7F' || rx == '\b'){
            if (idx > 0) {
                rgb_buff[--idx] = '\0';
                uart_tx('\b'); // cursor goes back from one
                uart_tx(' '); // write a space to replace the char
                uart_tx('\b');// put the cursor back again to replace the space
            }
            return; 
        }
        // if enter then finish and change line
        else if (rx == '\r' ){
            rgb_buff[idx++] = '\0';
            uart_printstr("\n\r");
            idx = 0;
            state = RGB;
            return;
        }
        
        if (!is_hex_char(rx))
            return;
            else if (rx < 20 || rx > 126)
            return;
        if (idx >= LEN -1)
            return;
        if (idx < LEN - 1){
            rgb_buff[idx] = rx;
            idx++;
        }
    uart_tx(rx);
    }
}

// #FF0000 = red
// #00FF00 = green
// #0000FF = blue
// #FFFFFF = white
// #000000 = black/turn off
// #FFFF00 = yellow
// #2BFAFA = cyan
// #FF00FF = magenta

int main(void)
{

    SREG =(1 << 7);
    uart_init();
    init_rgb();

    while (1)
    {
        if (state == INIT){
            uart_printstr("Choose a color in this format #RRGGBB: #");
            state = WAIT_RGB;
        }
        if (state == WAIT_RGB){
            continue;
        }
        if (state == RGB){
            rgb_from_buffer();
            state = INIT;
        }
    }
}