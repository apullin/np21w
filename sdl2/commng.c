#include	"compiler.h"
#include	"np2.h"
#include	"commng.h"
#include	"cmver.h"
#include	"cmjasts.h"


// ---- non connect

static UINT ncread(COMMNG self, UINT8 *data) {
	(void)self;
	(void)data;
	return(0);
}

static UINT ncwrite(COMMNG self, UINT8 data) {
	(void)self;
	(void)data;
	return(0);
}

static UINT ncwriteretry(COMMNG self) {
	(void)self;
	return(0);
}

static void ncbeginblock(COMMNG self) {
	(void)self;
}

static void ncendblock(COMMNG self) {
	(void)self;
}

static UINT nclastwritesuccess(COMMNG self) {
	(void)self;
	return(1);
}

static UINT8 ncgetstat(COMMNG self) {
	(void)self;
	return(0xf0);
}

static INTPTR ncmsg(COMMNG self, UINT msg, INTPTR param) {
	(void)self;
	(void)msg;
	(void)param;
	return(0);
}

static void ncrelease(COMMNG self) {
	(void)self;
}

static const _COMMNG com_nc = {
	COMCONNECT_OFF, ncread, ncwrite, ncwriteretry,
	ncbeginblock, ncendblock, nclastwritesuccess,
	ncgetstat, ncmsg, ncrelease
};


// ----

void commng_initialize(void) {
	cmvermouth_initialize();
}

COMMNG commng_create(UINT device, BOOL force) {

	COMMNG	ret;

	(void)force;
	ret = NULL;
	if (device == COMCREATE_MPU98II) {
		ret = cmvermouth_create();
	}
	else if (device == COMCREATE_PRINTER) {
		if (np2oscfg.jastsnd) {
			ret = cmjasts_create();
		}
	}
	if (ret == NULL) {
		ret = (COMMNG)&com_nc;
	}
	return(ret);
}

void commng_destroy(COMMNG hdl) {
	if (hdl) {
		hdl->release(hdl);
	}
}
