
#include "utils.h"
#include <stdlib.h>


volatile uint32_t temp_data = 0;
volatile uint32_t hum_data = 0;
volatile float temp_stock = 0;
volatile float hum_stock = 0;

// https://www.nongnu.org/avr-libc/user-manual/group__util__twi.html  STATUS
// https://datasheet4u.com/pdf/1551700/AHT20.pdf AHT20
// p12 AHT20 7.4 Sensor reading process

void get_data(uint8_t data, uint8_t current_byte){
    uint8_t little_temperature = 0;
    uint8_t little_humidity = 0;

    if (current_byte == 1){
        hum_data = data;
    }
    else if (current_byte == 2){
        // shift 8 bits
        hum_data <<= 8;
        hum_data |= data;
    }
    // 0xF0 -> select 4 first bits
    // 0x0F 4 last
    else if (current_byte == 3){
        little_humidity = ((data & 0xF0) >> 4); // >>4 because we do not need the last 4
        hum_data <<= 4;
        hum_data |= little_humidity;
        little_temperature = (data & 0x0F);
        temp_data = little_temperature;
    }
    else if (current_byte == 4){
        temp_data <<= 8;
        temp_data |= data;
    }
    else if (current_byte == 5) {
        temp_data <<= 8;
        temp_data |= data;
    }
}

void convert(void) {
    float humidity = 0;
    float temperature = 0;

    // formule p 13
    // Relative humidity [%] = (Srh / 2^20) * 100%
    humidity = ((float)hum_data / (1UL << 20) * 100);
    hum_stock += humidity;

    // formule p 13
    // Temperature [C] = (ST / 2^20) * 200 - 50
    temperature = (((float)temp_data / (1UL << 20)) * 200 - 50);
    temp_stock += temperature;
    
}


int main() {
    int i = 0;
    char buff_t[8] = {};
    char buff_h[8] = {};

    uart_init();
    i2c_init();
    while (1) {
        i2c_start(WRITE);
        _delay_ms(10);
        // cf 7.4 p12 AHT20
        i2c_write(0xAC);
        i2c_write(0x33);
        i2c_write(0x00);
        i2c_stop();
        _delay_ms(80);
        i2c_start(READ);
        i2c_read();
        convert();
        i++;
        if (i == 3){
            i = 0;
            //https://www.programmingelectronics.com/dtostrf/
            dtostrf(temp_stock/3, 0, 1, buff_t);
            dtostrf(hum_stock/3, 0, 1, buff_h);
            uart_printstr("Temperature: ");
            uart_printstr(buff_t);
            uart_printstr("C, Humidity: ");
            uart_printstr(buff_h);
            uart_printstr("%\n\r");
            temp_stock = 0;
            hum_stock = 0;
        }
        i2c_stop();
        _delay_ms(100);
    }
}