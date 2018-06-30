CLOCK = 16000000

CFLAGS = -Wall -O0 \
	-mmcu=atmega328p -DF_CPU=$(CLOCK) \
	-Iusbdrv -I.

LDFLAGS = -mmcu=atmega328p

AVRDUDE = avrdude -c usbtiny -p m328p -F
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

SRC_C = main.c \
				uart.c

OBJ = $(SRC_A:%.S=%.o) $(SRC_C:%.c=%.o)

.SUFFIXES:
.SUFFIXES: .c .S .h .o .elf .hex .list

all: out.hex out.list

.S.o:
	$(CC) ${CFLAGS} -x assembler-with-cpp -c $< -o $@

.c.o:
	$(CC) ${CFLAGS} -c $< -o $@

.elf.list:
	$(OBJDUMP) -j .text -j .data -S $< > $@

.elf.hex:
	$(OBJCOPY) -j .text -j .data -O ihex $< $@
	
out.elf: $(OBJ)
	$(CC) -o out.elf $(OBJ) $(LDFLAGS)

flash: out.hex
	$(AVRDUDE) -U flash:w:out.hex:i

clean: 
	rm -f *.elf *.hex *.list $(OBJ)

fuse:
	$(AVRDUDE) -U lfuse:w:0xce:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

