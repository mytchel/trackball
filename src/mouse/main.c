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
	int16_t dx, dy, sdx, sdy;

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

	int scroll_delay = 0;
	sdx = 0;
	sdy = 0;

	while (1) {
		/* Get buttons */	
		l = !((PINB >> 7) & 1);
		m = !((PINB >> 6) & 1);
		r = !((PINB >> 5) & 1);
		s = !((PINE >> 6) & 1);

		usb_mouse_buttons(l, m, r);

		if (adns_motion(&dx, &dy)) {
			dx = dx;
			dy = -dy;

			if (s) {
				int16_t t = 1;
				int8_t v = 1;

				sdx = sdy = 0;

				for (int i = 1; i < 5; i++) {
					if (dx > t*i) sdx = v*i;
					if (dx < -t*i) sdx = -v*i;
					if (dy > t*i) sdy = -v*i;
					if (dy < -t*i) sdy = v*i;
				}
/*
				dx /= 1;
				dy /= -1;

				sdx += dx;
				sdy += dy;

				if (sdx > 127) sdx = 127;
				if (sdx < -127) sdx = -127;
				if (sdy > 127) sdy = 127;
				if (sdy < -127) sdy = -127;
*/
				if (scroll_delay == 0) {
					usb_mouse_move(0, 0, (int8_t) sdx, (int8_t) sdy);
					usb_mouse_move(0, 0, 0, 0);
					sdx = 0;
					sdy = 0;
				}

				scroll_delay = (scroll_delay + 1) % 4;
			} else {
				usb_mouse_move(dx, dy, 0, 0);
				scroll_delay = 0;
			}
		}

		_delay_ms(10);
	}
}

