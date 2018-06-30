#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <avr/io.h>

void
uart_init(void)
{
	uint16_t ubrr;

	ubrr = ((((F_CPU / 16) + (9600 / 2)) / (9600)) - 1);
	UBRR0H = (uint8_t) (ubrr>>8);
	UBRR0L = (uint8_t) ubrr;

	UCSR0C = (1<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);
	UCSR0B = (1<<TXEN0);
}

void
uart_puts(char *s)
{
	while (*s) {
		UDR0 = *s++;
		while (!(UCSR0A & (1<<UDRE0)))
			;
	}
}

void
debug(const char *fmt, ...)
{
	char s[64];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(s, fmt, ap);
	va_end(ap);

	uart_puts(s);
}

