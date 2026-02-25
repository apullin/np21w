
enum {
	uPD8255A_LEFTBIT	= 0x80,
	uPD8255A_RIGHTBIT	= 0x20
};

typedef struct
{
	UINT32	autohide;
} MOUSEMNGSTAT;

#ifdef __cplusplus
extern "C" {
#endif

extern MOUSEMNGSTAT	mousemngstat;

void mousemng_initialize(void);
UINT8 mousemng_getstat(SINT16 *x, SINT16 *y, int clear);
void mousemng_onmotion(int dx, int dy);
void mousemng_onbutton(int button, int pressed);
void mousemng_setcapture(BOOL capture);
BOOL mousemng_getcapture(void);
void mousemng_setenabled(BOOL enabled);
BOOL mousemng_getenabled(void);
void mousemng_reset(void);
void mousemng_updateautohidecursor(void);
BOOL mousemng_getautohidecursor(void);
void mousemng_setautohidecursor(BOOL autohide);
BOOL mousemng_getabspos(SINT16 *x, SINT16 *y);

#ifdef __cplusplus
}
#endif
