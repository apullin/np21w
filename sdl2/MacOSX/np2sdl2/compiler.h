/**
 * @file	compiler.h
 * @brief	macOS SDL2 build configuration for np21w (PC-9821 IA-32 mode)
 */

#pragma once

#ifdef __OBJC__
#import <Cocoa/Cocoa.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <setjmp.h>
#include <limits.h>
#include <SDL.h>

/* SDL2 mutex header guard compat for sndcsec.h */
#ifdef SDL_mutex_h_
#define _SDL_mutex_h
#endif

#define	BYTESEX_LITTLE
#define	OSLANG_UTF8
#define	OSLINEBREAK_LF
#define RESOURCE_US

/* ---- basic types ---- */
typedef	signed int			SINT;
typedef	unsigned int		UINT;
typedef	signed char			SINT8;
typedef	unsigned char		UINT8;
typedef	signed short		SINT16;
typedef	unsigned short		UINT16;
typedef	signed int			SINT32;
typedef	unsigned int		UINT32;
typedef	int64_t				SINT64;
typedef	uint64_t			UINT64;

typedef	signed char			CHAR;
typedef	signed char			TCHAR;
typedef	unsigned char		BYTE;
typedef	uint32_t			DWORD;
typedef	uint16_t			WORD;

#ifndef	INLINE
#define	INLINE				static inline
#endif

#define	QWORD_CONST(v)		((UINT64)(v))
#define	SQWORD_CONST(v)		((SINT64)(v))

#define	INTPTR				intptr_t
#define	INT_PTR				intptr_t

#define	BRESULT				UINT
#define	OEMCHAR				char
#define	OEMTEXT(string)		string
#define	OEMSPRINTF			sprintf
#define	OEMSTRLEN			strlen

typedef	signed int			INT;
typedef	signed int			INT32;
typedef	signed short		INT16;
typedef	signed char			INT8;

#define SIZE_VGA
#if !defined(SIZE_VGA)
#define	RGB16		UINT32
#define	SIZE_QVGA
#endif

#if !defined(OBJC_BOOL_DEFINED)
typedef	unsigned char	BOOL;
#endif

#ifndef	TRUE
#define	TRUE	1
#endif

#ifndef	FALSE
#define	FALSE	0
#endif

#ifndef	MAX_PATH
#define	MAX_PATH	256
#endif

#ifndef __cplusplus
#ifndef	max
#define	max(a,b)	(((a) > (b)) ? (a) : (b))
#endif
#ifndef	min
#define	min(a,b)	(((a) < (b)) ? (a) : (b))
#endif
#endif	/* __cplusplus */

#ifndef	ZeroMemory
#define	ZeroMemory(d,n)		memset((d), 0, (n))
#endif
#ifndef	CopyMemory
#define	CopyMemory(d,s,n)	memcpy((d), (s), (n))
#endif
#ifndef	FillMemory
#define	FillMemory(a, b, c)	memset((a), (c), (b))
#endif

#ifndef	roundup
#define	roundup(x, y)	((((x)+((y)-1))/(y))*(y))
#endif

#ifndef	NELEMENTS
#define	NELEMENTS(a)	((int)(sizeof(a) / sizeof(a[0])))
#endif

/* ---- architecture detection ---- */
#if defined(__x86_64__) || defined(__amd64__)
#define	NP2_CPU_ARCH_AMD64
#define	NP2_CPU_ARCH_IA32
#define	GCC_CPU_ARCH_IA32
#define	GCC_CPU_ARCH_AMD64
#undef	MEMOPTIMIZE
#define LOADINTELDWORD(a)	(*((const UINT32 *)(a)))
#define LOADINTELWORD(a)	(*((const UINT16 *)(a)))
#define STOREINTELDWORD(a, b)	*(UINT32 *)(a) = (b)
#define STOREINTELWORD(a, b)	*(UINT16 *)(a) = (b)
#elif defined(__i386__)
#define	NP2_CPU_ARCH_IA32
#define	GCC_CPU_ARCH_IA32
#undef	MEMOPTIMIZE
#define LOADINTELDWORD(a)	(*((const UINT32 *)(a)))
#define LOADINTELWORD(a)	(*((const UINT16 *)(a)))
#define STOREINTELDWORD(a, b)	*(UINT32 *)(a) = (b)
#define STOREINTELWORD(a, b)	*(UINT16 *)(a) = (b)
#elif defined(__aarch64__)
/* ARM64 is little-endian, unaligned access OK on Apple Silicon */
#define	MEMOPTIMIZE		2
#define LOADINTELDWORD(a)	(*((const UINT32 *)(a)))
#define LOADINTELWORD(a)	(*((const UINT16 *)(a)))
#define STOREINTELDWORD(a, b)	*(UINT32 *)(a) = (b)
#define STOREINTELWORD(a, b)	*(UINT16 *)(a) = (b)
#else
#define	MEMOPTIMIZE		1
#endif

/* ---- calling conventions (no-op on non-MSVC) ---- */
#ifndef	FASTCALL
#define	FASTCALL
#endif
#define	CPUCALL		FASTCALL
#define	MEMCALL		FASTCALL
#define	DMACCALL	FASTCALL
#define	IOOUTCALL	FASTCALL
#define	IOINPCALL	FASTCALL
#define	SOUNDCALL	FASTCALL
#define	VRAMCALL	FASTCALL
#define	SCRNCALL	FASTCALL
#define	VERMOUTHCL	FASTCALL
#define	PARTSCALL	FASTCALL

/* ---- setjmp (for CPU panic recovery) ---- */
#define	sigjmp_buf				jmp_buf
#define	sigsetjmp(env, mask)	setjmp(env)
#define	siglongjmp(env, val)	longjmp(env, val)

/* ---- Windows API compatibility ---- */
#define	msgbox(title, msg)	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, msg, NULL)
#ifndef INFINITE
#define INFINITE	0xFFFFFFFF
#endif
#define GetTickCount()	SDL_GetTicks()

/* Windows file attribute constants */
#ifndef FILE_ATTRIBUTE_READONLY
#define FILE_ATTRIBUTE_READONLY		0x01
#define FILE_ATTRIBUTE_HIDDEN		0x02
#define FILE_ATTRIBUTE_SYSTEM		0x04
#define FILE_ATTRIBUTE_DIRECTORY	0x10
#define FILE_ATTRIBUTE_ARCHIVE		0x20
#define FILE_ATTRIBUTE_NORMAL		0x80
#endif

#define	_tcsrchr	strrchr
#define	_tcsncat	strncat
#define	_sntprintf	snprintf

/* ---- common includes ---- */
#include "common.h"
#include "milstr.h"
#include "_memory.h"
#include "rect.h"
#include "lstarray.h"
#include "trace.h"

/* ---- tick / assert / string ---- */
#define	GETTICK()			SDL_GetTicks()
#define	GETRAND()			random()
#ifdef	DEBUG
#include <assert.h>
#define	__ASSERT(s)			assert(s)
#else
#define	__ASSERT(s)
#endif
#define	SPRINTF				sprintf
#define	STRLEN				strlen

#define	_T(x)		x
#ifndef TEXT
#define	TEXT(x)		x
#endif
#define	_tcscpy		strcpy
#define	_tcsicmp	strcasecmp
#define	_tcsnicmp	strncasecmp
#define	_tcslen		strlen
#define	_tcscat		strcat
#define	_tcscmp		strcmp
#define	_tcsncmp	strncmp

#ifndef _countof
#define	_countof(a)	(sizeof(a) / sizeof((a)[0]))
#endif

/* ---- sound ---- */
#define	VERMOUTH_LIB
#define SOUND_CRITICAL
#define	SOUNDRESERVE	100

/* ---- text / encoding ---- */
#define	SUPPORT_UTF8
#define	SUPPORT_EUC
#define	SUPPORT_ANK

/* ---- display ---- */
#define	SUPPORT_16BPP
#define	SUPPORT_24BPP
#define	SUPPORT_32BPP
#define	SUPPORT_NORMALDISP
#define	SCREEN_BPP		16

/* ---- CPU / PC-9821 ---- */
#define	CPUCORE_IA32
#define	USE_FPU
#define	SUPPORT_FPU_DOSBOX2
#define	USE_MMX
#define	USE_3DNOW
#define	USE_SSE
#define	USE_SSE2
#define	USE_SSE3
#define	USE_SSSE3
#define	USE_SSE4_1
#define	USE_SSE4_2
#define	USE_SSE4A
#define	USE_TSC
#define	USE_VME
#define	IA32_PAGING_EACHSIZE
#define	IA32_REBOOT_ON_PANIC

/* ---- CPU dispatch optimizations (match Windows np21w build) ---- */
#define	USE_CPU_MODRMPREFETCH
#define	USE_CPU_INLINEINST
#define	USE_CPU_DIRECTREG
#define	USE_CPU_EIPMASK
#define	USE_FASTPAGING
#define	BIOS_IO_EMULATION
#define	SUPPORT_PC9821
#define	SUPPORT_CRT15KHZ
#define	SUPPORT_CRT31KHZ
#define	SUPPORT_PC9801_119

/* ---- peripherals ---- */
#define	SUPPORT_PEGC
#define	SUPPORT_PC9861K
#define	SUPPORT_HOSTDRV
#define	SUPPORT_SWSEEKSND
#define	SUPPORT_SASI
#define	SUPPORT_SCSI
#define	SUPPORT_IDEIO

/* ---- features ---- */
#define SUPPORT_EXTERNALCHIP
#define SUPPORT_ARC
#define SUPPORT_ZLIB
#define	SUPPORT_RESUME
#define	SUPPORT_STATSAVE	10
#define	SUPPORT_S98
#define	SUPPORT_KEYDISP
#define	SUPPORT_SOFTKBD		0
#define	SUPPORT_MEMDBG32
#define	SUPPORT_HRTIMER
#define	SUPPORT_FMGEN
#define	SUPPORT_SOUND_SB16
#define	SUPPORT_JOYSTICK

/* ---- disk / file ---- */
#define	SUPPORT_LARGE_HDD
typedef	SINT64	FILEPOS;
typedef	SINT64	FILELEN;
#define	NHD_MAXSIZE		8000
#define	NHD_MAXSIZE2	((UINT32)0xffffffff/1024/2)
#define	NHD_MAXSIZE28	130558

#define	SUPPORT_LARGE_MEMORY
#define	MEMORY_MAXSIZE		4000
#define	CPU_MULTIPLE_MAX	2048

#define	REG8		UINT8
#define	REG16		UINT16
