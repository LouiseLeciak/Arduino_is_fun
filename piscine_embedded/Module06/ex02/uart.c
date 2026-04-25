#include "utils.h"

// screen /dev/ttyUSB0 115200
// ctrl a puis k puis y

void uart_init() {
    // set baudrate
    // p180 table 20-1
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)MYUBRR;
    // enable transmiter
    // p202 bit 4 Reveiver enable
    UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
    // 8n1 => 8 bits de data, no parite, 1 bit de stop
    // p203 20-11
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

    // p182 tab 20-1 for double speed to lower fail chance
    UCSR0A |= (1 << U2X0);
}

void uart_tx(char c) {
    // waiting for the buffer to be empty
    while (!(UCSR0A & (1 << UDRE0))) {
    }
    // put data (char) into buffer
    UDR0 = c;
}

char uart_rx(void) {
    // p189, wait for data to be received
    while (!(UCSR0A & (1 << RXC0))) {
    }
    // get and return received data from buffer
    return UDR0;
}

void uart_printstr(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        uart_tx(str[i]);
    }
}

void uart_printhex(uint8_t value) {
    char hex[] = "0123456789abcdef";
    char buf[3];

    uint8_t first = value / 16;
    uint8_t sec = value % 16;

    buf[0] = hex[first];
    buf[1] = hex[sec];
    buf[2] = '\0';

    uart_printstr(buf);
}

uint8_t atoi_hex(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

bool is_hex_char(char c) {
    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') ||
        (c >= 'a' && c <= 'f'))
        return true;
    return false;
}

// Reverses a string 'str' of length 'len'
static void reverse(char* str, uint16_t len) {
    uint16_t i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

void uart_printint(int val) {
    char buffer[10];
    int i = 0;

    // neg numbers
    if (val < 0) {
        uart_tx('-');
        val = -val;
    }

    // 0
    if (val == 0) {
        uart_tx('0');
        return;
    }

    // convert in string
    while (val > 0) {
        buffer[i++] = (val % 10) + '0';
        val /= 10;
    }

    // put it back in the right order
    for (int j = i - 1; j >= 0; j--) {
        uart_tx(buffer[j]);
    }
}

int ft_itoa(int val, char* buf) {
    int i = 0;

    // 0
    if (val == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return 1;
    }

    // convert in string
    while (val > 0) {
        buf[i++] = (val % 10) + '0';
        val /= 10;
    }
    buf[i] = '0';

    reverse(buf, i);

    return (i);
}

float ft_pow_10(int nb) {
    float val = 1;
    while (nb--){
        val *= 10;
    }
    return (val);
}

void float_to_array(float value, char* buf, int div) {
    int before = (int)value;
    // delete everything before the ,
    float after = value - before;

    int idx = ft_itoa(before, buf);
    buf[idx++] = '.';
    after = after * ft_pow_10(div);
    ft_itoa(after, buf + idx);
}

void uart_printfloat(float value) {
    char buf[128] = {};
    float_to_array(value, buf, 2);
}