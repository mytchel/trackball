#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void
uart_init(void);

void
debug(const char *fmt, ...);

void
spi_init(void)
{
	/* Set MOSI, SCK as output */
	DDRB |= (1<<5)|(1<<3);
	/* Enable spi, master, clk high, sample rising. */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<CPOL)|(1<<CPHA);
}

uint8_t
spi_tranc(uint8_t c)
{
	SPDR = c;

	while (!(SPSR & (1<<SPIF)))
		;
	
	return SPDR;
}

void
spi_write(uint8_t addr, uint8_t v)
{
	spi_tranc(addr | (1<<7));
	spi_tranc(v);
}

uint8_t
spi_read(uint8_t addr)
{
	spi_tranc(addr);
	return spi_tranc(0xff);
}

int __attribute__((noreturn)) main(void)
{
	uart_init();

	debug("start\r\n");

	spi_init();

	DDRB |= (1<<1);

	debug("reset\r\n");
	PORTB &= ~(1<<1);
	_delay_ms(10);
	PORTB |= (1<<1);

	_delay_ms(500);

	debug("read spi\r\n");
	debug("product id 0x%x\r\n", spi_read(0));
	debug("revision id 0x%x\r\n", spi_read(1));

	_delay_ms(500);

	main();
	while (true) {
		_delay_ms(100);
	}
}

