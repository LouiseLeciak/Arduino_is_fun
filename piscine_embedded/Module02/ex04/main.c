#include <avr/io.h>
#include <util/delay.h>

#define LEN 50

int main(void)
{
    char username[LEN];
    char password[LEN];
    bool hide = false; // pour dire de print en * pour le password

    const char correct_user[] = "spectre";
    const char correct_pass[] = "password";

    uart_init();
    led_init();

    while (1)
    {
        // print enter your login
        // puis username:
        // recuperer la reponse
        // password:
        // recuperer la reponse
        // SI c'est le mm username ou password 
        // envoyer Hello username Shall we play a game 
        // faire un truc avec les leds
        // ELSE envoyer mauvaise combinaise de username mdp

    }
}