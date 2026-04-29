
#include "utils.h"

uint8_t atoi_hex(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

uint32_t atoi_hex_str(const char* str) {
    uint32_t res = 0;
    int i = 0;

    while (str[i] != '\0') {
        res = res * 16 + atoi_hex(str[i]);
        i++;
    }
    return res;
}

uint32_t atoi_int(const char* str) {
    uint32_t res = 0;
    int i = 0;

    while (str[i] != '\0') {
        res = res * 10 + atoi_hex(str[i]);
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

int ft_len(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

bool comp_str(const char* str1, const char* str2) {
    int i = 0;

    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return false;
        }
        i++;
    }
    return str1[i] == str2[i];
}


bool is_nb(char c) {
    if (c >= '0' && c <= '9')
        return true;
    else
        return false;
}

bool is_nb_str(char* str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] >= '0' && str[i] <= '9')
            continue;
        else
            return false;
    }
    return true;
}

bool is_nb_neg(char* str) {
    if (!str || !str[0]) return false;

    int i = 0;

    if (str[0] == '-') {
        i = 1;
        // if there is only a -
        if (!str[1]) return false;
    }

    for (; str[i]; i++) {
        if (str[i] < '0' || str[i] > '9') return false;
    }

    return true;
}

bool is_alnum(char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || is_nb(c))
        return true;
    else
        return false;
}

bool is_tag_correct(char* str) {
    if (ft_len(str) > 33) {
        uart_printstr("error: tag too long\n\r");
    }
    for (int i = 0; str[i]; i++) {
        if (is_alnum(str[i]) || is_nb(str[i]) || str[i] == '-' || str[i] == '_')
            continue;
        else
            return false;
    }
    return true;
}

uint16_t integrity_calculate(uint8_t* data, int count) {
    uint16_t sum1 = 0;
    uint16_t sum2 = 0;
    int index;

    for (index = 0; index < count; ++index) {
        sum1 = (sum1 + data[index]) % 255;
        sum2 = (sum2 + sum1) % 255;
    }

    return (sum2 << 8) | sum1;
}

uint8_t hex_to_bit(char *hex) {
    // * 16 bcs HEX is base 16
    return atoi_hex(hex[0]) * 16 + atoi_hex(hex[1]);
}