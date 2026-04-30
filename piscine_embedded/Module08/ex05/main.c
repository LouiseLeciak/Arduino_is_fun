
#include "utils.h"

volatile uint8_t current_led = D6;
volatile uint8_t current_color = RED;
volatile uint8_t leds[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};


void change_current_color() {
    if (current_color == RED) {
        current_color = GREEN;
        return;
    }
    if (current_color == GREEN) {
        current_color = BLUE;
        return;
    }
    if (current_color == BLUE) {
        current_color = RED;
        return;
    }
}

void change_current_led() {
    if (current_led == D6) {
        current_led = D7;
        return;
    }
    if (current_led == D7) {
        current_led = D8;
        return;
    }
    if (current_led == D8) {
        current_led = D6;
        return;
    }
}

void set_three_leds() {
    // set 0 to clean
    for (int i = 0; i < 4; i++) {
        SPI_master_transmit(0x00);
    }

   for (int i = 0; i < 3; i++){
        SPI_master_transmit(0xFF);
        SPI_master_transmit(leds[i][2]);
        SPI_master_transmit(leds[i][1]);
        SPI_master_transmit(leds[i][0]);

   }

    // end frame
    for (int i = 0; i < 4; i++) {
        SPI_master_transmit(0x00);
    }
}


int main() {
    SPI_master_init();
    ADC_init();
    DDRD &= ~BUTTONS;
    while (1) {
        //? read adc
        uint8_t res_pot = ADC_read_8();
        _delay_ms(20);
        // if sw1 is used
        if (!(PIND & (1 << PD2))) {
            _delay_ms(20.f);
            while (!(PIND & (1 << PD2)));
            _delay_ms(20.f);
            leds[current_led][current_color] = res_pot;
            change_current_color();
        }
        // sw2
        else if (!(PIND & (1 << PD4))) {
            _delay_ms(20.f);
            while (!(PIND & (1 << PD4)));
            _delay_ms(20.f);
            change_current_led();
        }
        set_three_leds();
    }
}