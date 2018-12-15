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
	int scroll_delay, light, light_delay;
	int8_t l, m, r, s;
	int16_t dx, dy;

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

	scroll_delay = 0;
	light = 0;
	light_delay = 0;
	while (1) {
		/* Get buttons */	
		l = !((PINB >> 7) & 1);
		m = !((PINB >> 6) & 1);
		r = !((PINB >> 5) & 1);
		s = !((PINE >> 6) & 1);

		usb_mouse_buttons(l, m, r);

		if (adns_motion(&dx, &dy)) {
			light = 50;
			if (s) {
				dx /= 1;
				dy /= -1;

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
		
				scroll_delay = (scroll_delay + 1) % 6;
			} else {
				usb_mouse_move(dx, dy, 0, 0);
				scroll_delay = 0;
			}
		}

		for (int i = 0; i < 100; i++) {
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
	}
}

