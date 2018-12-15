/* Mouse example with debug channel, for Teensy USB Development Board
 * http://www.pjrc.com/teensy/usb_mouse.html
 * Copyright (c) 2009 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdarg.h>
#include <stdio.h>

#include "spi.h"

#include "adns.h"

//#include "adns_srom.h"
#include "adns9800_srom_A5.h"

bool
srom_download(void)
{
	/* Enable 3k download? */
	spi_write(0x39, 1 << 1);

	/* Enable srom */
	spi_write(0x13, 0x1d);
	/* Sleep for 1 frame (> 120 ns) */
	_delay_ms(10);
	/* Start srom download */
	spi_write(0x13, 0x18);

	/* Write data */
	spi_start();
	spi(0x62 | (1<<7));
	_delay_us(15);
	
	int i;
	char c;
	for (i = 0; i < sizeof(ADNS_SROM); i++) {
		c = pgm_read_byte(ADNS_SROM + i);
		spi(c);
		_delay_us(15);
	}

	spi_end();
	
	_delay_ms(10);

	return (spi_read(0x2a) == ADNS_SROM_ID);
}

bool
adns_init(void)
{
	uint8_t c;

	spi_init();

	/* Reset spi */
	spi_end();
	spi_start();
	spi_end();

	/* Reset adns */
	spi_write(0x3a, 0x5a);
	_delay_ms(50);

	/* Read motion, x, y for some reason (datasheet) */
	spi_read(0x2);
	spi_read(0x3);
	spi_read(0x4);
	spi_read(0x5);
	spi_read(0x6);

	/* check product */
	if (spi_read(0x0) != 0x33)
		return false;

	/* check revision */
	if (spi_read(0x1) != 0x03)
		return false;

	if (!srom_download())
		return false;

	/* Enable laser */

	c = spi_read(0x20);
	c = c & 0xf0;
//	c = (c & 0xf0) | 0x4;
	spi_write(0x20, c);

	/* lift decection threshold */
//	spi_write(0x2e, 0x1f);

	/* Enable axis snap */
//	spi_write(0x42, 0x80);

	/* enable seperate resolutions */
	c = spi_read(0x10);
	spi_write(0x10, c | (1<<2));

	spi_write(0x0f, 0x4); /* x resolution */
	spi_write(0x2f, 0x4); /* y resolution */

	return true;
}

bool
adns_motion(int16_t *dx, int16_t *dy)
{
	uint8_t mot = spi_read(0x2);

	if (!(mot & (1 << 7))) {
		*dx = 0;
		*dy = 0;
		return false;
	}

	/* Motion */

	uint16_t xl = spi_read(0x3);
	uint16_t xh = spi_read(0x4);

	uint16_t yl = spi_read(0x5);
	uint16_t yh = spi_read(0x6);

	uint16_t x = (xh << 8) | xl;
	uint16_t y = (yh << 8) | yl;

	*dx = x;
	*dy = y;

	return true;
}

