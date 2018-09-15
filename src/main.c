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

int main(void)
{
	int8_t x, y;

	DDRD |= (1<<2);
	PORTD &= ~(1<<2);
	_delay_ms(10);
	PORTD |= (1<<2);
	_delay_ms(10);

	spi_init();

	usb_init();
	while (!usb_configured()) /* wait */ ;

	_delay_ms(500);

	while (1) {
		x = (int8_t) spi_read(0x3);
		y = (int8_t) spi_read(0x4);
		usb_mouse_move(x, y, 0);
		_delay_ms(20);
		
		// This sequence creates a right click
		//usb_mouse_buttons(0, 0, 1);
		//_delay_ms(10);
		//usb_mouse_buttons(0, 0, 0);
	}
}


int8_t PROGMEM circle[] = {
16, -1,
15, -4,
14, -7,
13, -9,
11, -11,
9, -13,
7, -14,
4, -15,
1, -16,
-1, -16,
-4, -15,
-7, -14,
-9, -13,
-11, -11,
-13, -9,
-14, -7,
-15, -4,
-16, -1,
-16, 1,
-15, 4,
-14, 7,
-13, 9,
-11, 11,
-9, 13,
-7, 14,
-4, 15,
-1, 16,
1, 16,
4, 15,
7, 14,
9, 13,
11, 11,
13, 9,
14, 7,
15, 4,
16, 1
};
