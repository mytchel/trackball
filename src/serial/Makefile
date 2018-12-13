CLOCK = 8000000

CFLAGS = -mmcu=atmega32u4 -I. -DF_CPU=8000000UL -Os -funsigned-char -funsigned-bitfields -ffunction-sections -fpack-struct -fshort-enums -Wall -std=gnu99 
#-MMD -MP -MF 

LDFLAGS = -mmcu=atmega32u4 -Wl,--relax -Wl,--gc-sections
#LDFLAGS = -Wl,--cref -Wl,--relax -Wl,--gc-sections

#CFLAGS = -Wall -Os \
#	-mmcu=atmega32u4 -DF_CPU=$(CLOCK) \
#	-I.

#LDFLAGS = -mmcu=atmega32u4

AVRDUDE = avrdude -P /dev/cuaU0 -c avr109 -p m32u4
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

SRC_C = example.c \
				spi.c \
				../mouse/adns.c \
				usb_serial.c

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

