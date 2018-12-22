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
	int light, light_delay;
	int16_t dx, dy, sx, sy;
	int8_t l, m, r, s;

	DDRD |= (1<<2);
	DDRC |= (1<<7);

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

	light = 0;
	light_delay = 0;
	sx = 0;
	sy = 0;
	
	while (1) {
		l = 0;
		m = 0;
		r = 0;
		s = 0;
		for (int i = 0; i < 100; i++) {
			l += !((PINB >> 7) & 1);
			m += !((PINB >> 6) & 1);
			r += !((PINB >> 5) & 1);
			s += !((PINE >> 6) & 1);

			if (i >= light) {
				PORTC &= ~(1<<7);
			} else {
				PORTC |= (1<<7);
			}
			_delay_us(10);
		}

		if (light > 0) {
			if (light_delay++ == 10) {
				light--;
				light_delay = 0;
			}
		}

		l = l > 80;
		m = m > 80;
		r = r > 80;
		s = s > 80;

		usb_mouse_buttons(l, m, r);

		if (adns_motion(&dx, &dy)) {
			light = 50;

			if (s) {
				sx += dx;
				sy -= dy;
				dx = 0;
				dy = 0;

#define SCROLL_LIM 15

				if (sx < -SCROLL_LIM || SCROLL_LIM < sx) {
					dx = sx / SCROLL_LIM;//sx > 0 ? 1 : -1;
					sx = 0;
				}

				if (sy < -SCROLL_LIM || SCROLL_LIM < sy) {
					dy = sy / SCROLL_LIM;//sy > 0 ? 1 : -1;
					sy = 0;
				}

				usb_mouse_move(0, 0, dx, dy);
			} else {
				usb_mouse_move(dx, dy, 0, 0);
				sx = 0;
				sy = 0;
			}
		}
	}
}

