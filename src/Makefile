CLOCK = 8000000

#CFLAGS = -Wall \
#	-mmcu=atmega32u4 -DF_CPU=$(CLOCK) \
#	-I.

CFLAGS = \
	-mmcu=atmega32u4 -DF_CPU=$(CLOCK) \
	-Os -funsigned-char -funsigned-bitfields -ffunction-sections \
	-fpack-struct -fshort-enums -Wall -std=gnu99

LDFLAGS = -mmcu=atmega32u4 -Wl,--relax -Wl,--gc-sections

AVRDUDE = avrdude -P /dev/cuaU0 -c avr109 -p m32u4
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

MOUSE_SRC_C = main.c \
							adns.c \
							spi.c \
							usb_mouse.c

MOUSE_OBJ = $(MOUSE_SRC_A:%.S=%.o) $(MOUSE_SRC_C:%.c=%.o)

SERIAL_SRC_C = serial.c \
							 spi.c \
							 adns.c \
							 usb_serial.c

SERIAL_OBJ = $(SERIAL_SRC_A:%.S=%.o) $(SERIAL_SRC_C:%.c=%.o)

.SUFFIXES:
.SUFFIXES: .c .S .h .o .elf .hex .list

all: mouse.hex 

.S.o:
	$(CC) ${CFLAGS} -x assembler-with-cpp -c $< -o $@

.c.o:
	$(CC) ${CFLAGS} -c $< -o $@

.elf.list:
	$(OBJDUMP) -j .text -j .data -S $< > $@

.elf.hex:
	$(OBJCOPY) -j .text -j .data -O ihex $< $@
	
mouse.elf: $(MOUSE_OBJ)
	$(CC) -o mouse.elf $(MOUSE_OBJ) $(LDFLAGS)

serial.elf: $(SERIAL_OBJ)
	$(CC) -o serial.elf $(SERIAL_OBJ) $(LDFLAGS)

flash: mouse.hex
	$(AVRDUDE) -U flash:w:mouse.hex:i

flash-serial: serial.hex
	$(AVRDUDE) -U flash:w:serial.hex:i

clean: 
	rm -f *.elf *.hex *.list $(MOUSE_OBJ) $(SERIAL_OBJ)

fuse:
	$(AVRDUDE) -U lfuse:w:0xce:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

