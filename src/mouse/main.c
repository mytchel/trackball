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
	int16_t dx, dy, sx, sy;
	int8_t la, lb, l = 0;
	int8_t ma, mb, m = 0;
	int8_t ra, rb, r = 0;
	int8_t sa, sb, s = 0;

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
	sx = 0;
	sy = 0;
	
	while (1) {
		/* Get buttons */	
		la = !((PINB >> 7) & 1);
		ma = !((PINB >> 6) & 1);
		ra = !((PINB >> 5) & 1);
		sa = !((PINE >> 6) & 1);

		if (adns_motion(&dx, &dy)) {
			light = 50;
			if (s) {
				sx += dx;
				sy -= dy;

				if (scroll_delay == 0) {
					bool ix = false, iy = false;
					int i;
					for (i = 16; i >= 0; i--) {
						if (!ix && sx >= i * 4) {
							sx = i;
							ix = true;
						} else if (!ix && sx <= i * -4) {
							sx = -i;
							ix = true;
						}

						if (!iy && sy >= i * 4) {
							sy = i;
							iy = true;
						} else if (!iy && sy <= i * -4) {
							sy = -i;
							iy = true;
						}
					}

					if (!ix) sx = 0;
					if (!iy) sy = 0;

					usb_mouse_move(0, 0, (int8_t) sx, (int8_t) sy);
					sx = 0;
					sy = 0;
				}
			} else {
				usb_mouse_move(dx, dy, 0, 0);
			}
		}

		scroll_delay = (scroll_delay + 1) % 4;

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

		lb = !((PINB >> 7) & 1);
		if (la && lb) l = 1;
		else l = 0;
		mb = !((PINB >> 6) & 1);
		if (ma && mb) m = 1;
		else m = 0;
		rb = !((PINB >> 5) & 1);
		if (ra && rb) r = 1;
		else r = 0;
		sb = !((PINE >> 6) & 1);
		if (sa && sb) s = 1;
		else s = 0;

		usb_mouse_buttons(l, m, r);
	}
}

