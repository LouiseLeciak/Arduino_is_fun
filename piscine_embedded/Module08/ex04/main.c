
#include "utils.h"

#define LEN 9
volatile int idx = 0;
char rgb_buff[LEN] = {0};

typedef enum State { INIT, WAIT_RGB, RGB, RAINBOW } states_t;

volatile states_t state = INIT;

void rgb_from_buffer(void) {
    uint8_t r = hex_to_bit(&rgb_buff[0]);  // RR so the 2 first
    uint8_t g = hex_to_bit(&rgb_buff[2]);  // GG
    uint8_t b = hex_to_bit(&rgb_buff[4]);  // BB
    // get the led D6 D7 or D8
    char led = rgb_buff[6];

    get_led(r, g, b, led);
}

int is_valid_rgb_cmd() {
    // RR GG BB DX
    if (ft_len(rgb_buff) != 8) {
        return 0;
    }

    // hex char for the 6 first
    for (int i = 0; i < 7; i++) {
        if (!is_hex_char(rgb_buff[i])) {
            return 0;
        }
    }

    if (rgb_buff[7] != 'D') {
        return 0;
    }
    // check if the last char is a valid led
    if (rgb_buff[8] != '6' && rgb_buff[8] != '7' && rgb_buff[8] != '8') {
        return 0;
    }
    return 1;
}

void __attribute__((signal, used)) USART_RX_vect(void) {
    // get the character
    char rx = uart_rx();

    // if backspace, delete last char and update what's on screen
    if (state == WAIT_RGB) {
        // x7F = delete in ascii
        if (rx == '\x7F' || rx == '\b') {
            if (idx > 0) {
                rgb_buff[--idx] = '\0';
                uart_tx('\b');
                uart_tx(' ');
                uart_tx('\b');
            }
            return;
        }
        // if enter then finish and change line
        else if (rx == '\r') {
            rgb_buff[idx++] = '\0';
            uart_printstr("\n\r");
            idx = 0;

            if (is_valid_rgb_cmd()) {
                state = RGB;
            } else {
                uart_printstr("error: invalid format\n\r");
                state = INIT;
            }
            return;
        }

        if (!is_hex_char(rx)) {
            return;

        } else if (rx < 20 || rx > 126) {
            return;
        }
        if (idx >= LEN - 1) {
            return;
        }
        if (idx < LEN - 1) {
            rgb_buff[idx] = rx;
            idx++;
        }
        uart_tx(rx);
    }
}

void get_led(uint8_t r, uint8_t g, uint8_t b, char led) {
    uint8_t l1[3] = {0};
    uint8_t l2[3] = {0};
    uint8_t l3[3] = {0};

    if (led == '6') {
        l1[0] = r;
        l1[1] = g;
        l1[2] = b;
    } else if (led == '7') {
        l2[0] = r;
        l2[1] = g;
        l2[2] = b;
    } else if (led == '8') {
        l3[0] = r;
        l3[1] = g;
        l3[2] = b;
    } else {
        uart_printstr("error: invalid led (6, 7, 8)");
        return;
    }

    set_three_leds(l1, l2, l3);
}


//! mode RAINBOW
//! ADAPTER WHEEL

int main() {
    uart_init();
    SPI_master_init();
    // to enable interrupt
    SREG |= (1 << 7);

    while (1) {
        if (state == INIT) {
            uart_printstr("Choose a color in this format #RRGGBB: #");
            state = WAIT_RGB;
        }
        if (state == WAIT_RGB) {
            continue;
        }
        if (state == RGB) {
            rgb_from_buffer();
            state = INIT;
        }
    }
}