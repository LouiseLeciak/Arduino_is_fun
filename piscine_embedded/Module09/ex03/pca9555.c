#include "utils.h"

uint8_t read_pca9555(uint8_t reg) {
    i2c_start();
    // i tell the expender wich reg i want to interact write
    i2c_write((ADDR << 1) | 0);
    i2c_write(reg);

    // then i read
    i2c_start();
    i2c_write((ADDR << 1) | 1);
    uint8_t res = i2c_read_nack();
    i2c_stop();

    return res;
}

void write_pca9555(uint8_t reg, uint8_t data) {
    i2c_start();
    // addr + turn the write
    i2c_write((ADDR << 1) | 0);
    // reg of the pca9555
    i2c_write(reg);
    // my data
    i2c_write(data);
    i2c_stop();
}

// reg:
// 0x00 = Input P0
// 0x01 = Input P1
// 0x02 = Output P0
// 0x03 = Output P1
// 0x04 = Polarity P0
// 0x05 = Polarity P1
// 0x06 = Config P0
// 0x07 = Config P1
// void pca9555_write_with_mask(uint8_t reg, uint8_t data, uint8_t mask){
//     utin8_t val;

//     val = read_pca9555(reg);
//     write_pca9555(reg,)
// }