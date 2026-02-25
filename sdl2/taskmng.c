#include	"compiler.h"
#include	"inputmng.h"
#include	"taskmng.h"
#include	"sdlkbd.h"
#include	"vramhdl.h"
#include	"menubase.h"
#include	"sysmenu.h"
#include	"mousemng.h"


	BOOL	task_avail;


void sighandler(int signo) {

	(void)signo;
	task_avail = FALSE;
}


void taskmng_initialize(void) {

	task_avail = TRUE;
}

void taskmng_exit(void) {

	task_avail = FALSE;
}

void taskmng_rol(void) {

	SDL_Event	e;

	if ((!task_avail) || (!SDL_PollEvent(&e))) {
		return;
	}
	switch(e.type) {
		case SDL_MOUSEMOTION:
			if (menuvram == NULL) {
				mousemng_onmotion(e.motion.xrel, e.motion.yrel);
			}
			else {
				menubase_moving(e.motion.x, e.motion.y, 0);
			}
			break;

		case SDL_MOUSEBUTTONUP:
			switch(e.button.button) {
				case SDL_BUTTON_LEFT:
					if (menuvram != NULL)
					{
						menubase_moving(e.button.x, e.button.y, 2);
					}
#if defined(__IPHONEOS__)
					else if (SDL_IsTextInputActive())
					{
						SDL_StopTextInput();
					}
					else if (e.button.y >= 320)
					{
						SDL_StartTextInput();
					}
#endif
					else
					{
						mousemng_onbutton(SDL_BUTTON_LEFT, 0);
					}
					break;

				case SDL_BUTTON_RIGHT:
					if (menuvram == NULL) {
						mousemng_onbutton(SDL_BUTTON_RIGHT, 0);
					}
					break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			switch(e.button.button) {
				case SDL_BUTTON_LEFT:
					if (menuvram != NULL)
					{
						menubase_moving(e.button.x, e.button.y, 1);
					}
					else
					{
						/* Capture mouse on first click in emulation */
						if (!mousemng_getcapture()) {
							mousemng_setcapture(TRUE);
						}
						mousemng_onbutton(SDL_BUTTON_LEFT, 1);
					}
					break;

				case SDL_BUTTON_RIGHT:
					if (menuvram == NULL) {
						mousemng_onbutton(SDL_BUTTON_RIGHT, 1);
					}
					break;

				case SDL_BUTTON_MIDDLE:
					/* Middle-click: toggle capture / menu (like Windows) */
					if (menuvram != NULL) {
						menubase_close();
					}
					else if (mousemng_getcapture()) {
						mousemng_setcapture(FALSE);
					}
					else {
						mousemng_setcapture(FALSE);
						sysmenu_menuopen(0, 0, 0);
					}
					break;
			}
			break;

		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_F11) {
				/* F11: toggle menu (works if macOS isn't capturing it) */
				if (menuvram == NULL) {
					mousemng_setcapture(FALSE);
					sysmenu_menuopen(0, 0, 0);
				}
				else {
					menubase_close();
				}
			}
			else if (e.key.keysym.sym == SDLK_ESCAPE
				  && (e.key.keysym.mod & KMOD_GUI)) {
				/* Cmd+Escape: release capture / toggle menu */
				if (menuvram != NULL) {
					menubase_close();
				}
				else if (mousemng_getcapture()) {
					mousemng_setcapture(FALSE);
				}
				else {
					sysmenu_menuopen(0, 0, 0);
				}
			}
			else {
				/* Everything else (including bare Escape) → PC-98 */
				sdlkbd_keydown(e.key.keysym.sym);
			}
			break;

		case SDL_KEYUP:
			sdlkbd_keyup(e.key.keysym.sym);
			break;

		case SDL_QUIT:
			task_avail = FALSE;
			break;
	}
}

BOOL taskmng_sleep(UINT32 tick) {

	UINT32	base;

	base = GETTICK();
	while((task_avail) && ((GETTICK() - base) < tick)) {
		taskmng_rol();
		SDL_Delay(1);
	}
	return(task_avail);
}
