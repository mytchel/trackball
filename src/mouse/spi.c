#include <stdbool.h>
#include <stdarg.h>
#include <avr/io.h>
#include <util/delay.h>

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

	spi(addr | 0x80);

	spi(v);
	_delay_us(20);

	spi_end();
	_delay_us(100);
}

	uint8_t
spi_read(uint8_t addr)
{
	uint8_t v;

	spi_start();

	spi(addr & 0x7f);
	_delay_us(100);

	v = spi(0xff);
	_delay_us(1);

	spi_end();
	_delay_us(19);

	return v;
}

