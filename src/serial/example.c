/* Simple example for Teensy USB Development Board
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2008 PJRC.COM, LLC
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
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "usb_serial.h"

#include "../mouse/spi.h"
#include "../mouse/adns.h"

void send_str(const char *s);
uint8_t recv_str(char *buf, uint8_t size);
void parse_and_execute_command(const char *buf, uint8_t num);

int main(void)
{
	char buf[32];
	uint8_t n;

	adns_init();

	usb_init();
	while (!usb_configured()) /* wait */ ;
	_delay_ms(1000);

	while (1) {
		while (!(usb_serial_get_control() & USB_SERIAL_DTR)) /* wait */ ;

		// discard anything that was received prior.  Sometimes the
		// operating system or other software will send a modem
		// "AT command", which can still be buffered.
		usb_serial_flush_input();

		send_str(PSTR("\r\nTeensy USB Serial.\r\n"));

		// and then listen for commands and process them
		while (1) {
			send_str(PSTR("> "));
			n = recv_str(buf, sizeof(buf));
			if (n == 255) break;
			send_str(PSTR("\r\n"));
			parse_and_execute_command(buf, n);
		}
	}
}

// Send a string to the USB serial port.  The string must be in
// flash memory, using PSTR
//
void send_str(const char *s)
{
	char c;
	while (1) {
		c = pgm_read_byte(s++);
		if (!c) break;
		usb_serial_putchar(c);
	}
}

// Receive a string from the USB serial port.  The string is stored
// in the buffer and this function will not exceed the buffer size.
// A carriage return or newline completes the string, and is not
// stored into the buffer.
// The return value is the number of characters received, or 255 if
// the virtual serial connection was closed while waiting.
//
uint8_t recv_str(char *buf, uint8_t size)
{
	int16_t r;
	uint8_t count=0;

	while (count < size) {
		r = usb_serial_getchar();
		if (r != -1) {
			if (r == '\r' || r == '\n') return count;
			if (r >= ' ' && r <= '~') {
				*buf++ = r;
				usb_serial_putchar(r);
				count++;
			}
		} else {
			if (!usb_configured() ||
			  !(usb_serial_get_control() & USB_SERIAL_DTR)) {
				// user no longer connected
				return 255;
			}
			// just a normal timeout, keep waiting
		}
	}
	return count;
}

// parse a user command and execute it, or print an error message
//
void parse_and_execute_command(const char *buf, uint8_t num)
{
	char s[32];

	sprintf(s, "product id: 0x%x\r\n", spi_read(0x0));
	usb_serial_write((uint8_t *) s, strlen(s));
	sprintf(s, "revision id: 0x%x\r\n", spi_read(0x1));
	usb_serial_write((uint8_t *) s, strlen(s));

	sprintf(s, "srom id: 0x%x\r\n", spi_read(0x2a));
	usb_serial_write((uint8_t *) s, strlen(s));

	sprintf(s, "squall: 0x%x\r\n", ((uint16_t) spi_read(0x07)) << 2);
	usb_serial_write((uint8_t *) s, strlen(s));

	sprintf(s, "lift dectection: 0x%x\r\n", spi_read(0x2e));
	usb_serial_write((uint8_t *) s, strlen(s));

	sprintf(s, "c2: 0x%x\r\n", spi_read(0x10));
	usb_serial_write((uint8_t *) s, strlen(s));

	sprintf(s, "rpt: 0x%x\r\n", spi_read(0x10) & (1<<2));
	usb_serial_write((uint8_t *) s, strlen(s));

	sprintf(s, "dx res: 0x%x\r\n", spi_read(0x0f));
	usb_serial_write((uint8_t *) s, strlen(s));

	sprintf(s, "dy res: 0x%x\r\n", spi_read(0x2f));
	usb_serial_write((uint8_t *) s, strlen(s));

	sprintf(s, "snap: 0x%x\r\n", spi_read(0x42));
	usb_serial_write((uint8_t *) s, strlen(s));



	int16_t dx, dy;
	bool mot = adns_motion(&dx, &dy);

	sprintf(s, "mot: %i\r\n", mot);
	usb_serial_write((uint8_t *) s, strlen(s));

	sprintf(s, "dx : %i\r\n", dx);
	usb_serial_write((uint8_t *) s, strlen(s));
	sprintf(s, "dy : %i\r\n", dy);
	usb_serial_write((uint8_t *) s, strlen(s));

}

