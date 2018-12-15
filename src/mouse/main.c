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
	int scroll_delay = 0;
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
		scroll_delay = (scroll_delay + 1) % 4;

		/* Get buttons */	
		l = !((PINB >> 7) & 1);
		m = !((PINB >> 6) & 1);
		r = !((PINB >> 5) & 1);
		s = !((PINE >> 6) & 1);

		usb_mouse_buttons(l, m, r);

		if (adns_motion(&dx, &dy)) {
			if (s) {
				dx /= 2;
				dy /= -2;

				if (dx > 127) 
					dx = 127;
				if (dx < -127) 
					dx = -127;
				if (dy > 127) 
					dy = 127;
				if (dy < -127) 
					dy = -127;

				if (scroll_delay == 0) {
					usb_mouse_move(0, 0, (int8_t) dx, (int8_t) dy);
				}
			} else {
				usb_mouse_move(dx, dy, 0, 0);
				scroll_delay = 0;
			}
		}

		_delay_ms(10);
	}
}

