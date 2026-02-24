#include	"compiler.h"
#include	"mousemng.h"

MOUSEMNGSTAT	mousemngstat;

UINT8 mousemng_getstat(SINT16 *x, SINT16 *y, int clear) {

	*x = 0;
	*y = 0;
	(void)clear;
	return(0xa0);
}

void mousemng_reset(void) {
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

