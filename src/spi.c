#include <stdbool.h>
#include <stdarg.h>
#include <avr/io.h>

void
spi_init(void)
{
	/* Set MOSI, SCK as output */
	DDRB |= (1<<1)|(1<<2);

	/* Pin 1 for SS */
	DDRD |= (1<<3);

	/* Default SS must be high even though 
		 we can not use it on this board. */
	DDRB |= (1<<0);
	PORTB |= (1<<0);

	SPCR = (1<<MSTR)|(1<<CPOL)|(1<<CPHA)
		|(1<<SPR1)|(1<<SPR0)
		|(1<<SPE);
}

	uint8_t
spi(uint8_t c)
{
	SPDR = c;

	while (!(SPSR & (1<<SPIF)))
		;

	return SPDR;
}

void
spi_start(void)
{
	PORTD &= ~(1<<3);
}

void
spi_end(void)
{
	PORTD |= (1<<3);
}

	void
spi_write(uint8_t addr, uint8_t v)
{
	spi_start();
	spi(addr | (1<<7));
	spi(v);
	spi_end();
}

	uint8_t
spi_read(uint8_t addr)
{
	uint8_t v;

	spi_start();
	spi(addr);
	v = spi(0xff);
	spi_end();

	return v;
}

