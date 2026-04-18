#include <avr/io.h>
#include <util/delay.h>

// screen /dev/ttyUSB0 115200
// ctrl a puis k puis y

#define BAUD 115200
// p182 table 20-1
#define MYUBRR ((F_CPU / (8 * BAUD)) - 1)

// p184 20.5 USART initialization
void uart_init() {
    // set baudrate
    // p180 table 20-1
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char) MYUBRR;
    // enable transmiter
    //p202 bit 4 Reveiver enable
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    // 8n1 => 8 bits de data, no parite, 1 bit de stop
    // p202 20.11.4
    // p203 20-11
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

    //p182 tab 20-1 for double speed to lower fail chance
    UCSR0A |= (1 << U2X0);

}

void uart_tx(char c) {
    // waiting for the buffer to be empty
    while (!(UCSR0A & (1 << UDRE0))){}
    // put data (char) into buffer
    UDR0 = c;
}

char uart_rx(void) {
    //p189, wait for data to be received
    //p200 20.11.2
    while (!(UCSR0A & (1 << RXC0))){}
    // get and return received data from buffer
    return UDR0;
}

int main (void) {
    uart_init();
    while (1){
        uart_tx(uart_rx());
    }
}