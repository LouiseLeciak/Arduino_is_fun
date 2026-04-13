NAME	= main
PROG	= arduino
MICROCONTROLLER	= atmega328p
CC	= avr-gcc
OBJCOPY	= avr-objcopy
DUDE	= avrdude
FILES	= main.c
FLAGS	= -Wall -Wextra -Werror -mmcu=$(MICROCONTROLLER) -I$(AVR_LIBC_DIR)/include
BAUD_RATE	= 115200
PORT	= /dev/ttyUSB0
AVR_LIBC_DIR=/tmp/avr-libc

# how fast is the CPU
F_CPU	= 16000000 # pdf -> quartz 16Mhz

all:	$(AVR_LIBC_DIR)/.CACHETAG hex flash

$(AVR_LIBC_DIR)/.CACHETAG:
	rm -rf $(AVR_LIBC_DIR)
	git clone --depth=1 https://github.com/avrdudes/avr-libc/ $(AVR_LIBC_DIR)
	cd $(AVR_LIBC_DIR) && ./bootstrap
	cd $(AVR_LIBC_DIR) && ./configure --build=`./config.guess` --host=avr
	cd $(AVR_LIBC_DIR) && make -j
	touch $(AVR_LIBC_DIR)/.CACHETAG

hex:
	$(CC) $(FLAGS) -DF_CPU=$(F_CPU) -o $(NAME).bin $(FILES)
	$(OBJCOPY) -O ihex $(NAME).bin $(NAME).hex

flash:
	$(DUDE) -p $(MICROCONTROLLER) -c $(PROG) -b $(BAUD_RATE) -P $(PORT) -U flash:w:main.hex:i

clean:
	rm -f main.hex main.bin

.PHONY: all hex flash clean