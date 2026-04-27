
#include "utils.h"

uint8_t atoi_hex(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}


uint32_t atoi_hex_str(const char *str) {
    uint32_t res = 0;
    int i = 0;

    while (str[i] != '\0') {
        res = res * 16 + atoi_hex(str[i]);
        i++;
    }
    return res;
}

bool is_hex_char(char c) {
    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') ||
        (c >= 'a' && c <= 'f'))
        return true;
    return false;
}

int ft_len(char * str){
    int i = 0;
    while (str[i] != '\0'){
        i++;
    }
    return i;
}
