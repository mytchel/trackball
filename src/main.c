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
#include "usb_mouse.h"
#include "spi.h"

#include "adns9800_srom_A6.h"

void srom_download(void)
{
	/* Enable 3k download? */
	spi_write(0x39, 1 << 1);

	/* Enable srom */
	spi_write(0x13, 0xd1);
	/* Sleep for 1 frame (> 120 ns) */
	_delay_ms(1);
	/* Start srom download */
	spi_write(0x13, 0x18);

	/* Write data */
	spi_start();
	spi(0x64 | (1<<7));
	
	int i;
	for (i = 0; i < sizeof(SROMA6); i++) {
		spi(SROMA6[i]);
	}

	spi_end();
}

int main(void)
{
	int8_t l, m, r, s;

	DDRD |= (1<<2);

		/* Enable pull ups on buttons */
	PORTB |= (1 << 7) | (1 << 6) | (1 << 5);
	PORTE |= (1 << 6);

	usb_init();
	while (!usb_configured()) /* wait */ ;

	_delay_ms(500);

	/* Reset spi */
	spi_end();
	spi_start();
	spi_end();
	
	spi_init();

	/* Reset adns */
	spi_write(0x3a, 0x5a);
	_delay_ms(50);

	/* Read motion, x, y for some reason (datasheet) */
	spi_read(0x2);
	spi_read(0x3);
	spi_read(0x4);
	spi_read(0x5);
	spi_read(0x6);

	srom_download();

	_delay_ms(10);

	/* Enable laser */
	uint8_t lc = spi_read(0x20);
	spi_write(0x20, lc & 0xf0);

	/* Enable highest resolution */
	spi_write(0xf, 0x29);

	while (1) {
		uint8_t mot = spi_read(0x2);

		if (mot & (1 << 7)) {
			/* Motion */

			uint16_t xl = spi_read(0x3);
			uint16_t xh = spi_read(0x4);

			uint16_t yl = spi_read(0x5);
			uint16_t yh = spi_read(0x6);

			int16_t x = -((xh << 8) | xl);
			int16_t y = (yh << 8) | yl;

			usb_mouse_move(x, y, 0);
		}

		/* Get buttons */	
		l = !((PINB >> 7) & 1);
		m = !((PINB >> 6) & 1);
		r = !((PINB >> 5) & 1);
		s = !((PINE >> 6) & 1);

		usb_mouse_buttons(l, m, r);

		_delay_ms(20);
	}
}

