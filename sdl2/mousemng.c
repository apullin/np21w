#include	"compiler.h"
#include	"mousemng.h"

MOUSEMNGSTAT	mousemngstat;

static SINT16	mouse_dx;
static SINT16	mouse_dy;
static UINT8	mouse_btn;		/* uPD8255A style: bit set = button UP */
static BOOL	mouse_captured;
static BOOL	mouse_enabled;	/* FALSE = mouse disabled via menu */

void mousemng_initialize(void) {
	mouse_dx = 0;
	mouse_dy = 0;
	mouse_btn = uPD8255A_LEFTBIT | uPD8255A_RIGHTBIT;	/* both up */
	mouse_captured = FALSE;
	mouse_enabled = TRUE;
}

UINT8 mousemng_getstat(SINT16 *x, SINT16 *y, int clear) {

	if (!mouse_enabled) {
		*x = 0;
		*y = 0;
		return uPD8255A_LEFTBIT | uPD8255A_RIGHTBIT;
	}
	*x = mouse_dx;
	*y = mouse_dy;
	if (clear) {
		mouse_dx = 0;
		mouse_dy = 0;
	}
	return mouse_btn;
}

void mousemng_onmotion(int dx, int dy) {
	mouse_dx += (SINT16)dx;
	mouse_dy += (SINT16)dy;
}

void mousemng_onbutton(int button, int pressed) {
	if (button == SDL_BUTTON_LEFT) {
		if (pressed) {
			mouse_btn &= ~uPD8255A_LEFTBIT;	/* clear bit = pressed */
		} else {
			mouse_btn |= uPD8255A_LEFTBIT;		/* set bit = released */
		}
	}
	else if (button == SDL_BUTTON_RIGHT) {
		if (pressed) {
			mouse_btn &= ~uPD8255A_RIGHTBIT;
		} else {
			mouse_btn |= uPD8255A_RIGHTBIT;
		}
	}
}

void mousemng_setcapture(BOOL capture) {
	if (!mouse_enabled) {
		capture = FALSE;
	}
	if (capture && !mouse_captured) {
		SDL_SetRelativeMouseMode(SDL_TRUE);
		mouse_captured = TRUE;
	}
	else if (!capture && mouse_captured) {
		SDL_SetRelativeMouseMode(SDL_FALSE);
		mouse_captured = FALSE;
	}
}

BOOL mousemng_getcapture(void) {
	return mouse_captured;
}

void mousemng_setenabled(BOOL enabled) {
	mouse_enabled = enabled;
	if (!enabled) {
		mousemng_setcapture(FALSE);
	}
}

BOOL mousemng_getenabled(void) {
	return mouse_enabled;
}

void mousemng_reset(void) {
	mouse_dx = 0;
	mouse_dy = 0;
}

void mousemng_updateautohidecursor(void) {
}

BOOL mousemng_getautohidecursor(void) {
	return mousemngstat.autohide;
}

void mousemng_setautohidecursor(BOOL autohide) {
	mousemngstat.autohide = autohide ? 1 : 0;
}

BOOL mousemng_getabspos(SINT16 *x, SINT16 *y) {
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	if (x) *x = (SINT16)mx;
	if (y) *y = (SINT16)my;
	return TRUE;
}
