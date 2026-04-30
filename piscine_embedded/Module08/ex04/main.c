
#include "utils.h"

#define LEN 9
#define LEN_RAIN 12
volatile int idx = 0;
char rgb_buff[LEN] = {0};
char rain_buff[12] = {0};
uint8_t rainbow_pos = 0;

volatile bool rainbow = false;

typedef enum State { INIT, WAIT_RGB, RGB, RAINBOW } states_t;
volatile int need_prompt = 0;

volatile states_t state = INIT;

void rgb_from_buffer(void) {
    uint8_t r = hex_to_bit(&rgb_buff[0]);  // RR so the 2 first
    uint8_t g = hex_to_bit(&rgb_buff[2]);  // GG
    uint8_t b = hex_to_bit(&rgb_buff[4]);  // BB
    // get the led D6 D7 or D8
    char led = rgb_buff[7];

    get_led(r, g, b, led);
}

int is_valid_rgb_cmd() {
    // RR GG BB DX
    if (ft_len(rgb_buff) != 8) {
        return 0;
    }

    // hex char for the 6 first
    for (int i = 0; i < 6; i++) {
        if (!is_hex_char(rgb_buff[i])) {
            return 0;
        }
    }

    if (rgb_buff[6] != 'D') {
        return 0;
    }
    // check if the last char is a valid led
    if (rgb_buff[7] != '6' && rgb_buff[7] != '7' && rgb_buff[7] != '8') {
        return 0;
    }
    return 1;
}

int check_rainbow() { return (comp_str(rain_buff, "FULLRAINBOW")); }

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

void __attribute__((signal, used)) USART_RX_vect(void) {
    // get the character
    char rx = uart_rx();

    if (state == WAIT_RGB || need_prompt == 1) {
        if (idx == 1 && rx == 'U' && rgb_buff[0] == 'F') {
            rain_buff[0] = 'F';
            rain_buff[idx++] = rx;
            rainbow = true;
            uart_tx(rx);

            return;
        }

        // x7F = delete in ascii
        if (rx == '\x7F' || rx == '\b') {
            if (idx > 0) {
                if (!rainbow)
                    rgb_buff[--idx] = '\0';
                else
                    rain_buff[--idx] = '\0';
                uart_tx('\b');
                uart_tx(' ');
                uart_tx('\b');
            }
            return;
        }
        // if enter then finish and change line
        else if (rx == '\r') {
            if (rainbow)
                rain_buff[idx] = '\0';
            else
                rgb_buff[idx] = '\0';
            uart_printstr("\n\r");
            idx = 0;

            if (rainbow && check_rainbow()) {
                state = RAINBOW;
            } else if (is_valid_rgb_cmd()) {
                state = RGB;
            } else {
                uart_printstr("error: invalid format\n\r");
                state = INIT;
            }
            rainbow = false;

            return;
        }

        if (rainbow == true) {
            if (rx < 'A' || rx > 'Z') {
                return;
            }
        }
        if (!is_hex_char(rx) && !rainbow) {
            return;

        } else if (rx < 20 || rx > 126) {
            return;
        }
        if (idx >= LEN - 1 && !rainbow) {
            return;
        }
        if (idx < LEN - 1 && !rainbow) {
            rgb_buff[idx] = rx;
            idx++;
        }
        if (rainbow) {
            if (idx >= LEN_RAIN - 1) {
                return;
            }
            rain_buff[idx++] = rx;
        }
        uart_tx(rx);
    }
}

int main() {
    uart_init();
    SPI_master_init();
    int i = 0;
    // to enable interrupt
    SREG |= (1 << 7);

    while (1) {
        if (state == INIT) {
            need_prompt = 0;
            i = 0;
            uart_printstr(
                "Choose a color in this format #RRGGBBDX or #FULLRAINBOW: #");
            state = WAIT_RGB;
        }
        if (state == WAIT_RGB) {
            need_prompt = 0;
            i = 0;
            continue;
        }
        if (state == RGB) {
            need_prompt = 0;
            i = 0;
            rgb_from_buffer();
            state = INIT;
        }
        if (state == RAINBOW) {
            need_prompt = 1;
            i++;
            wheel(rainbow_pos++);
            _delay_ms(20);
            if (i == 1)
                uart_printstr(
                    "Choose a color in this format #RRGGBBDX or #FULLRAINBOW: "
                    "#");
            continue;
        }
    }
}