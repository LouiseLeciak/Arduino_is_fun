#include <avr/io.h>
#include <util/delay.h>

// screen /dev/ttyUSB0 115200
// ctrl a puis k puis y

#define TIME_FREQUENCY F_CPU/1024
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
    UCSR0B = (1 << TXEN0);
    // 8n1 => 8 bits de data, no parite, 1 bit de stop
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


void uart_printstr(const char* str) {
    for (int i = 0; str[i] != '\0'; i++){
        uart_tx(str[i]);
    }
}
void timer_init() {
    // choose what to do with timer -> TCCR1A
    // toggle OC1A on Compare Match
    // p140, table 16-1
    TCCR1A |= (1 << COM1A0);
    // i want the timer to count
    // p 141, table 16-4, I want CTC so WGM12 has to be 1
    // CTC means i want the timer to count
    // if i check 16.11.2 i can see that wgm12 is in TCCR1B
    TCCR1B |= (1 << WGM12);
    
    //p 143, table 16-5 CS12 & CS10 1 0 1 to have 1024 as prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);
    //p 141 table 16-4 CTC -> [TOP] OCR1A
    OCR1A = TIME_FREQUENCY * 2;
    // the timer is calling the interrupt by itself    
    
    // tell the timer to trigger the interrupt
    // p103 15.2.2
    TIMSK1 |= (1 << OCIE1A);
}

// p74 12.4 11 bcs they start with 1 so 12-1
// __attribute__ tell to the compilo i give you more infos
// about this function
// used like volatile, look not used but is used
// when interrupt (this fc) is called, the compilo is going to
// save everything, state, register, when return restore all
// basically stop everything, does the function, then restore all like nothing happened
void __attribute__((signal, used)) __vector_11(void) {
    uart_printstr("Hello World !\n\r");
}

int main (void) {
    // p20 Bit-7 to enabled the interrupts
    SREG =(1 << 7);
    uart_init();
    timer_init();
    while (1){}
}