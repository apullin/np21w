
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

UINT8 mousemng_getstat(SINT16 *x, SINT16 *y, int clear);
void mousemng_reset(void);
void mousemng_updateautohidecursor(void);
BOOL mousemng_getautohidecursor(void);
void mousemng_setautohidecursor(BOOL autohide);
BOOL mousemng_getabspos(SINT16 *x, SINT16 *y);

#ifdef __cplusplus
}
#endif
