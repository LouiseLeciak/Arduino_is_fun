#include <avr/io.h>
#include <util/delay.h>

// screen /dev/ttyUSB0 115200
// ctrl a puis k puis y

#define LEN 20
#define BAUD 115200
// p182 table 20-1
#define MYUBRR ((F_CPU / (8 * BAUD)) - 1)
#define LEDS ((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4))
#define LED_XOR ((1 << PB0) | (0 << PB1) | (1 << PB2) | (0 << PB4))

char user_buff[LEN] = {0};
char pass_buff[LEN] = {0};
volatile int idx = 0;


typedef enum State{
    INIT,
    ASK_USER,
    ASK_PASS,
    WAITING_USER,
    WAITING_PASS,
    MATCHING,
    BAD_COMB,
    CHECK_AUTH,
    END,
}states_t;
volatile states_t state = INIT;

void invalid_password(void) {
  for (uint8_t i = 0; i < 5; i++) {
    PORTB &= ~LEDS;
    _delay_ms(200);
    PORTB |= LEDS;
    _delay_ms(200);
  }
}

void valid_password(void) {
  for (uint8_t i = 0; i < 5; i++) {
    PORTB |= LED_XOR;
    _delay_ms(200);
    PORTB ^= LEDS;
  }
}

void uart_init() {
    // set baudrate
    // p180 table 20-1
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char) MYUBRR;
    // enable transmiter
    //p202 bit 4 Reveiver enable
    UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
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

char uart_rx(void) {
    //p189, wait for data to be received
    while (!(UCSR0A & (1 << RXC0))){}
    // get and return received data from buffer
    return UDR0;
}

void led_init(void){
    // i want my led to be the output (1)
    DDRB |= LEDS;
}

void uart_printstr(const char* str) {
    for (int i = 0; str[i] != '\0'; i++){
        uart_tx(str[i]);
    }
}

bool ft_strcmp(const char *user, char *input) {
    int i = 0;

    while(user[i] != '\0' || input[i] != '\0'){
        if (user[i] != input[i]) {
            return false;
        }
        i++;
    }
    return true;
}

void __attribute__((signal, used)) __vector_18(void) {
    // get the character
    char rx = uart_rx();

    // if backspace, delete last char and update what's on screen    
    if (state == WAITING_USER) {
        // x7F = delete in ascii
        if (rx == '\x7F' || rx == '\b'){
            if (idx > 0) {
            user_buff[--idx] = '\0';
            uart_tx('\b'); // cursor goes back from one
            uart_tx(' '); // write a space to replace the char
            uart_tx('\b');// put the cursor back again to replace the space
            }
        return; 
        }
    // if enter then finish and change line
        else if (rx == '\r' ){
            user_buff[idx++] = '\0';
            uart_printstr("\n\r");
            idx = 0;
            state = ASK_PASS;
            return;
        }
        
        else if (rx < 20 || rx > 126)
            return;
        if (idx >= LEN -1)
            return;
        if (idx < LEN - 1){
            user_buff[idx] = rx;
            idx++;
        }
        uart_tx(rx);
    }

    //same with pass
    if (state == WAITING_PASS) {
        if (rx == '\x7F' || rx == '\b'){
            if (idx > 0) {
                pass_buff[--idx] = '\0';
                uart_tx('\b');
                uart_tx(' ');
                uart_tx('\b');
            }
            return;
        }
        else if (rx == '\r'){
            pass_buff[idx++] = '\0';
            uart_printstr("\n\r");
            idx = 0;
            state = CHECK_AUTH;
            return;
        }
        else if (rx < 20 || rx > 126)
            return;
        if (idx >= LEN -1)
            return;
        if (idx < LEN - 1){
            pass_buff[idx] = rx;
            idx++;
        }
        uart_tx('*');
    }
}

int main(void)
{
    // for interrupts
    SREG =(1 << 7);

    const char correct_user[] = "grenouille";
    const char correct_pass[] = "super";

    uart_init();
    led_init();

    while (1){
        switch (state) {
            case INIT:
                state = ASK_USER;
                break;
            case ASK_USER:
                // print enter your login
                uart_printstr("Enter your login:\n\r");
                // username:
                uart_printstr("     username: ");
                state = WAITING_USER;
                break;
            case WAITING_USER:
                break;
            case ASK_PASS:
                uart_printstr("     password: ");
                state = WAITING_PASS;
                break;
            case WAITING_PASS:
                break;
            case MATCHING:
                uart_printstr("Hello ");
                uart_printstr(user_buff);
                uart_printstr("!\n\r");
                uart_printstr("Shall we play a game?\n\r");
                state = END;
                break;
            case BAD_COMB:
                uart_printstr("Bad combinaison username/password\n\r");
                state = INIT;
                break;
            case CHECK_AUTH:
                if (ft_strcmp(correct_user, user_buff) && ft_strcmp(correct_pass, pass_buff))
                {
                    state = MATCHING;
                    valid_password();
                    PORTB &= ~LEDS;
                }
                else
                {
                    state = BAD_COMB;
                    invalid_password();
                    PORTB &= ~LEDS;
                }
                break;
            case END:
                break;
                
        }
    }
}