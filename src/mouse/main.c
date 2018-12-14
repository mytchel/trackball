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

#include "usb_mouse.h"
#include "spi.h"

#include "adns.h"

int main(void)
{
	int8_t l, m, r, s;
	int16_t dx, dy;

	DDRD |= (1<<2);

	/* Enable pull ups on buttons */
	PORTB |= (1 << 7) | (1 << 6) | (1 << 5);
	PORTE |= (1 << 6);

	if (!adns_init()) {
		while (1)
			;
	}

	usb_init();
	while (!usb_configured()) /* wait */ ;

	_delay_ms(500);

	while (1) {
		/* Get buttons */	
		l = !((PINB >> 7) & 1);
		m = !((PINB >> 6) & 1);
		r = !((PINB >> 5) & 1);
		s = !((PINE >> 6) & 1);

		usb_mouse_buttons(l, m, r);

		if (adns_motion(&dx, &dy)) {
			dx = dx / 4;
			dy = -dy / 4;

			if (dx > 127) dx = 127;
			if (dx < -127) dx = -127;
			if (dy > 127) dy = 127;
			if (dy < -127) dy = -127;

			if (s) {
				usb_mouse_move(0, 0, dx, dy);
			} else {
				usb_mouse_move(dx, dy, 0, 0);
			}
		}
	}
}

