
#include "utils.h"

// 2 = abged
// 0 1 2 3 4 5 6
// a b c d e f g
// 1 1 0 1 1 0 1

uint8_t display_0() {
    uint8_t seg = 0x00;
    seg |= (1 << 0);  // a
    seg |= (1 << 1);  // b
    seg |= (1 << 2);  // c
    seg |= (1 << 3);  // d
    seg |= (1 << 4);  // e
    seg |= (1 << 5);  // f
    return seg;
}

uint8_t display_1() {
    uint8_t seg = 0x00;
    seg |= (1 << 1);  // b
    seg |= (1 << 2);  // c
    return seg;
}

uint8_t display_2() {
    uint8_t seg = 0x00;
    seg |= (1 << 0);  // a
    seg |= (1 << 1);  // b
    seg |= (1 << 3);  // d
    seg |= (1 << 4);  // e
    seg |= (1 << 6);  // g
    return seg;
}

uint8_t display_3() {
    uint8_t seg = 0x00;

    seg |= (1 << 0);  // a
    seg |= (1 << 1);  // b
    seg |= (1 << 2);  // c
    seg |= (1 << 3);  // d
    seg |= (1 << 6);  // g
    return seg;
}

uint8_t display_4() {
    uint8_t seg = 0x00;

    seg |= (1 << 1);  // b
    seg |= (1 << 2);  // c
    seg |= (1 << 5);  // f
    seg |= (1 << 6);  // g
    return seg;
}

uint8_t display_5() {
    uint8_t seg = 0x00;

    seg |= (1 << 0);  // a
    seg |= (1 << 2);  // c
    seg |= (1 << 3);  // d
    seg |= (1 << 5);  // f
    seg |= (1 << 6);  // g
    return seg;
}

uint8_t display_6() {
    uint8_t seg = 0x00;

    seg |= (1 << 0);  // a
    seg |= (1 << 2);  // c
    seg |= (1 << 3);  // d
    seg |= (1 << 4);  // e
    seg |= (1 << 5);  // f
    seg |= (1 << 6);  // g
    return seg;
}

uint8_t display_7() {
    uint8_t seg = 0x00;

    seg |= (1 << 0);  // a
    seg |= (1 << 1);  // b
    seg |= (1 << 2);  // c
    return seg;
}

uint8_t display_8() {
    uint8_t seg = 0x00;

    seg |= (1 << 0);  // a
    seg |= (1 << 1);  // b
    seg |= (1 << 2);  // c
    seg |= (1 << 3);  // d
    seg |= (1 << 4);  // e
    seg |= (1 << 5);  // f
    seg |= (1 << 6);  // g
    return seg;
}

uint8_t display_9() {
    uint8_t seg = 0x00;

    seg |= (1 << 0);  // a
    seg |= (1 << 1);  // b
    seg |= (1 << 2);  // c
    seg |= (1 << 3);  // d
    seg |= (1 << 5);  // f
    seg |= (1 << 6);  // g
    return seg;
}
