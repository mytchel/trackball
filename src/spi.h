#include <stdbool.h>
#include <stdarg.h>
#include <avr/io.h>

void
spi_init(void);

void
spi_start(void);

void
spi_end(void);

uint8_t
spi(uint8_t c);

void
spi_write(uint8_t addr, uint8_t v);

uint8_t
spi_read(uint8_t addr);

