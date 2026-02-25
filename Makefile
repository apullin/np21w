# np21w macOS SDL2 Makefile (IA-32 / PC-9821 build)

CC      = cc
CXX     = c++
OBJC    = cc

SDL_CFLAGS  := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)

CFLAGS   = -O2 -fsigned-char -fno-strict-aliasing $(SDL_CFLAGS)
CXXFLAGS = -O2 -fsigned-char -fno-strict-aliasing -std=c++11 $(SDL_CFLAGS)
OBJCFLAGS = -O2 -fsigned-char -fno-strict-aliasing $(SDL_CFLAGS) -fobjc-arc

LDFLAGS  = $(SDL_LDFLAGS) -framework Cocoa -framework IOKit -lz -lc++

DEFS = -DSUPPORT_VPCVHD -DSUPPORT_KAI_IMAGES \
       -DSUPPORT_SOUND_SB16 -DSUPPORT_SMPU98

INCLUDES = -I. \
           -Isdl2 \
           -Isdl2/MacOSX/np2sdl2 \
           -Isdl2/MacOSX/np2sdl2/misc \
           -Icommon \
           -Icbus \
           -Igeneric \
           -Ii386c \
           -Ii386c/ia32 \
           -Ii386c/ia32/instructions \
           -Ii386c/ia32/instructions/fpu \
           -Ii386c/ia32/instructions/mmx \
           -Ii386c/ia32/instructions/sse \
           -Ii386c/ia32/instructions/sse2 \
           -Ii386c/ia32/instructions/sse3 \
           -Ii386c/ia32/instructions/ssse3 \
           -Ii386c/ia32/instructions/sse4 \
           -Ii386c/ia32/instructions/sse4a \
           -Iio \
           -Imem \
           -Inetwork \
           -Isound \
           -Isound/fmgen \
           -Isound/getsnd \
           -Isound/vermouth \
           -Isound/mame \
           -Ivram \
           -Ibios \
           -Ifdd \
           -Ifont \
           -Ilio \
           -Iembed \
           -Iembed/menu \
           -Iembed/menubase \
           -Idiskimage \
           -Icodecnv \
           -Izlib

BUILDDIR = build
TARGET   = np21w

# ---- SDL2 platform layer ----
SDL2_C_SRCS = \
	sdl2/np2.c \
	sdl2/dosio.c \
	sdl2/scrnmng.c \
	sdl2/taskmng.c \
	sdl2/inputmng.c \
	sdl2/fontmng.c \
	sdl2/sdlkbd.c \
	sdl2/commng.c \
	sdl2/joymng.c \
	sdl2/mousemng.c \
	sdl2/sysmenu.c \
	sdl2/sysmng.c \
	sdl2/timemng.c \
	sdl2/ini.c \
	sdl2/trace.c

SDL2_CXX_SRCS = \
	sdl2/soundmng.cpp \
	sdl2/MacOSX/np2sdl2/misc/threadbase.cpp \
	sdl2/MacOSX/np2sdl2/misc/tty.cpp \
	sdl2/MacOSX/np2sdl2/misc/usbdev.cpp

SDL2_OBJC_SRCS = \
	sdl2/MacOSX/np2sdl2/main.m

# ---- core emulation ----
CORE_SRCS = \
	pccore.c \
	nevent.c \
	calendar.c \
	timing.c \
	statsave.c \
	keystat.c \
	debugsub386.c

# ---- common ----
COMMON_SRCS = \
	common/strres.c \
	common/milstr.c \
	common/_memory.c \
	common/textfile.c \
	common/profile.c \
	common/rect.c \
	common/lstarray.c \
	common/bmpdata.c \
	common/mimpidef.c \
	common/parts.c \
	common/wavefile.c \
	common/arc.c \
	common/arcunzip.c \
	common/resize.c

# ---- codec conversion ----
CODECNV_SRCS = \
	codecnv/eucsjis.c \
	codecnv/eucucs2.c \
	codecnv/sjiseuc.c \
	codecnv/sjisucs2.c \
	codecnv/tcswap16.c \
	codecnv/tcswap32.c \
	codecnv/textcnv.c \
	codecnv/ucs2sjis.c \
	codecnv/ucs2utf8.c \
	codecnv/utf8ucs2.c

# ---- BIOS ----
BIOS_SRCS = \
	bios/bios.c \
	bios/bios09.c \
	bios/bios0c.c \
	bios/bios12.c \
	bios/bios13.c \
	bios/bios18.c \
	bios/bios19.c \
	bios/bios1a.c \
	bios/bios1b.c \
	bios/bios1c.c \
	bios/bios1f.c \
	bios/sxsibios.c

# ---- C-BUS ----
CBUS_SRCS = \
	cbus/cbuscore.c \
	cbus/pc9861k.c \
	cbus/mpu98ii.c \
	cbus/amd98.c \
	cbus/board118.c \
	cbus/board14.c \
	cbus/board26k.c \
	cbus/board86.c \
	cbus/boardpx.c \
	cbus/boardso.c \
	cbus/boardspb.c \
	cbus/boardx2.c \
	cbus/cs4231io.c \
	cbus/pcm86io.c \
	cbus/sasiio.c \
	cbus/scsiio.c \
	cbus/scsicmd.c \
	cbus/ideio.c \
	cbus/atapicmd.c \
	cbus/boardsb16.c \
	cbus/ct1741io.c \
	cbus/ct1745io.c \
	cbus/smpu98.c

# ---- FDD / disk ----
FDD_SRCS = \
	fdd/diskdrv.c \
	fdd/newdisk.c \
	fdd/fdd_mtr.c \
	fdd/sxsi.c \
	fdd/sxsihdd.c \
	fdd/sxsicd.c

# ---- font ----
FONT_SRCS = \
	font/font.c \
	font/fontdata.c \
	font/fontmake.c \
	font/fontpc88.c \
	font/fontpc98.c \
	font/fontv98.c \
	font/fontfm7.c \
	font/fontx1.c \
	font/fontx68k.c

# ---- generic ----
GENERIC_SRCS = \
	generic/cmndraw.c \
	generic/keydisp.c \
	generic/cmjasts.c \
	generic/cmver.c \
	generic/hostdrv.c \
	generic/hostdrvs.c \
	generic/unasm.c \
	generic/np2info.c \
	generic/memdbg32.c \
	generic/softkbd.c

# ---- I/O ----
IO_SRCS = \
	io/iocore.c \
	io/artic.c \
	io/cgrom.c \
	io/cpuio.c \
	io/crtc.c \
	io/dipsw.c \
	io/dmac.c \
	io/egc.c \
	io/emsio.c \
	io/fdc.c \
	io/fdd320.c \
	io/gdc.c \
	io/gdc_sub.c \
	io/gdc_pset.c \
	io/mouseif.c \
	io/nmiio.c \
	io/np2sysp.c \
	io/pic.c \
	io/pit.c \
	io/printif.c \
	io/serial.c \
	io/sysport.c \
	io/upd4990.c \
	io/necio.c \
	io/epsonio.c \
	io/pcidev.c \
	io/pegc.c

# ---- LIO ----
LIO_SRCS = \
	lio/lio.c \
	lio/gscreen.c \
	lio/gpset.c \
	lio/gline.c \
	lio/gcircle.c \
	lio/gput1.c

# ---- memory ----
MEM_SRCS = \
	mem/dmav30.c \
	mem/dmax86.c \
	mem/memegc.c \
	mem/memems.c \
	mem/memepp.c \
	mem/memtram.c \
	mem/memvga.c \
	mem/memvram.c

# ---- sound (C) ----
SOUND_C_SRCS = \
	sound/sound.c \
	sound/soundrom.c \
	sound/s98.c \
	sound/adpcmc.c \
	sound/adpcmg.c \
	sound/beepc.c \
	sound/beepg.c \
	sound/cs4231c.c \
	sound/cs4231g.c \
	sound/fmboard.c \
	sound/oplgenc.c \
	sound/oplgeng.c \
	sound/opngenc.c \
	sound/opngeng.c \
	sound/opntimer.c \
	sound/pcm86c.c \
	sound/pcm86g.c \
	sound/pcmmix.c \
	sound/psggenc.c \
	sound/psggeng.c \
	sound/rhythmc.c \
	sound/ct1741c.c \
	sound/ct1741g.c \
	sound/sndcsec.c \
	sound/tms3631c.c \
	sound/tms3631g.c \
	sound/getsnd/getsnd.c \
	sound/getsnd/getsmix.c \
	sound/getsnd/getmp3.c \
	sound/getsnd/getogg.c \
	sound/getsnd/getwave.c \
	sound/vermouth/midiout.c \
	sound/vermouth/midimod.c \
	sound/vermouth/midinst.c \
	sound/vermouth/midvoice.c \
	sound/vermouth/midtable.c \
	sound/mame/fmopl.c \
	sound/mame/ymdeltat.c \
	sound/mame/ymf262.c

# ---- sound (C++ fmgen) ----
SOUND_CXX_SRCS = \
	sound/fmgen/fmgen_file.cpp \
	sound/fmgen/fmgen_fmgen.cpp \
	sound/fmgen/fmgen_fmgwrap.cpp \
	sound/fmgen/fmgen_fmtimer.cpp \
	sound/fmgen/fmgen_opm.cpp \
	sound/fmgen/fmgen_opna.cpp \
	sound/fmgen/fmgen_psg.cpp

# ---- external chip (C++) ----
EXTCHIP_CXX_SRCS = \
	sdl2/ext/opl3.cpp \
	sdl2/ext/opna.cpp \
	sdl2/ext/externalchipmanager.cpp \
	sdl2/ext/externalopl3.cpp \
	sdl2/ext/externalopm.cpp \
	sdl2/ext/externalopna.cpp \
	sdl2/ext/externalpsg.cpp \
	sdl2/ext/c86ctl/c86ctlif.cpp \
	sdl2/ext/c86ctl/c86ctlc86box.cpp \
	sdl2/ext/c86ctl/c86ctlgimic.cpp \
	sdl2/ext/c86ctl/c86ctlrealchipbase.cpp \
	sdl2/ext/scci/scciif.cpp \
	sdl2/ext/scci/sccisoundinterface.cpp \
	sdl2/ext/scci/sccisoundinterfacemanager.cpp \
	sdl2/ext/scci/sccisoundchip.cpp \
	sdl2/ext/scci/sccispfmlight.cpp

# ---- VRAM ----
VRAM_SRCS = \
	vram/vram.c \
	vram/scrndraw.c \
	vram/sdraw.c \
	vram/dispsync.c \
	vram/palettes.c \
	vram/maketext.c \
	vram/maketgrp.c \
	vram/makegrph.c \
	vram/makegrex.c \
	vram/scrnsave.c

# ---- disk image ----
DISKIMAGE_SRCS = \
	diskimage/cd/cdd_ccd.c \
	diskimage/cd/cdd_cue.c \
	diskimage/cd/cdd_iso.c \
	diskimage/cd/cdd_mds.c \
	diskimage/cd/cdd_nrg.c \
	diskimage/fd/fdd_bkdsk.c \
	diskimage/fd/fdd_d88.c \
	diskimage/fd/fdd_dcp.c \
	diskimage/fd/fdd_nfd.c \
	diskimage/fd/fdd_vfdd.c \
	diskimage/fd/fdd_xdf.c \
	diskimage/cddfile.c \
	diskimage/fddfile.c \
	diskimage/img_strres.c

# ---- network ----
# Network support requires Windows TAP driver - disabled on macOS
NETWORK_SRCS =

# ---- IA-32 CPU ----
IA32_SRCS = \
	i386c/cpucore.c \
	i386c/cpumem.c \
	i386c/ia32/interface.c \
	i386c/ia32/ia32.c \
	i386c/ia32/cpu.c \
	i386c/ia32/cpu_io.c \
	i386c/ia32/cpu_mem.c \
	i386c/ia32/ctrlxfer.c \
	i386c/ia32/debug.c \
	i386c/ia32/disasm.c \
	i386c/ia32/exception.c \
	i386c/ia32/groups.c \
	i386c/ia32/inst_table.c \
	i386c/ia32/paging.c \
	i386c/ia32/resolve.c \
	i386c/ia32/segments.c \
	i386c/ia32/task.c \
	i386c/ia32/instructions/bin_arith.c \
	i386c/ia32/instructions/bit_byte.c \
	i386c/ia32/instructions/ctrl_trans.c \
	i386c/ia32/instructions/data_trans.c \
	i386c/ia32/instructions/dec_arith.c \
	i386c/ia32/instructions/flag_ctrl.c \
	i386c/ia32/instructions/fpu.c \
	i386c/ia32/instructions/logic_arith.c \
	i386c/ia32/instructions/misc_inst.c \
	i386c/ia32/instructions/seg_reg.c \
	i386c/ia32/instructions/shift_rotate.c \
	i386c/ia32/instructions/string_inst.c \
	i386c/ia32/instructions/system_inst.c \
	i386c/ia32/instructions/fpu/fpemul_dosbox.c \
	i386c/ia32/instructions/fpu/fpemul_dosbox2.c \
	i386c/ia32/instructions/fpu/fpdummy.c \
	i386c/ia32/instructions/mmx/mmx.c \
	i386c/ia32/instructions/mmx/3dnow.c \
	i386c/ia32/instructions/sse/sse.c \
	i386c/ia32/instructions/sse2/sse2.c \
	i386c/ia32/instructions/sse3/sse3.c \
	i386c/ia32/instructions/ssse3/ssse3.c \
	i386c/ia32/instructions/sse4/sse4_1.c \
	i386c/ia32/instructions/sse4/sse4_2.c \
	i386c/ia32/instructions/sse4a/sse4a.c

# ---- embed (menu system) ----
EMBED_SRCS = \
	embed/vramhdl.c \
	embed/vrammix.c \
	embed/menu/dlgcfg.c \
	embed/menu/dlgscr.c \
	embed/menu/dlgabout.c \
	embed/menu/filesel.c \
	embed/menu/menustr.c \
	embed/menubase/menubase.c \
	embed/menubase/menudlg.c \
	embed/menubase/menuicon.c \
	embed/menubase/menumbox.c \
	embed/menubase/menures.c \
	embed/menubase/menusys.c \
	embed/menubase/menuvram.c

# ---- zlib (bundled subset + system -lz for deflate/compress) ----
ZLIB_SRCS = \
	zlib/adler32.c \
	zlib/crc32.c \
	zlib/inffast.c \
	zlib/inflate.c \
	zlib/inftrees.c \
	zlib/zutil.c

# ---- collect all sources ----
ALL_C_SRCS = $(SDL2_C_SRCS) $(CORE_SRCS) $(COMMON_SRCS) $(CODECNV_SRCS) \
             $(BIOS_SRCS) $(CBUS_SRCS) $(FDD_SRCS) $(FONT_SRCS) \
             $(GENERIC_SRCS) $(IO_SRCS) $(LIO_SRCS) $(MEM_SRCS) \
             $(SOUND_C_SRCS) $(VRAM_SRCS) $(DISKIMAGE_SRCS) $(NETWORK_SRCS) \
             $(IA32_SRCS) $(EMBED_SRCS) $(ZLIB_SRCS)

ALL_CXX_SRCS = $(SDL2_CXX_SRCS) $(SOUND_CXX_SRCS) $(EXTCHIP_CXX_SRCS)

ALL_OBJC_SRCS = $(SDL2_OBJC_SRCS)

# ---- object files ----
C_OBJS   = $(ALL_C_SRCS:%.c=$(BUILDDIR)/%.o)
CXX_OBJS = $(ALL_CXX_SRCS:%.cpp=$(BUILDDIR)/%.o)
OBJC_OBJS = $(ALL_OBJC_SRCS:%.m=$(BUILDDIR)/%.o)

ALL_OBJS = $(C_OBJS) $(CXX_OBJS) $(OBJC_OBJS)

# ---- rules ----
.PHONY: all clean test

all: $(TARGET)

test: tests/audio_test.c
	$(CC) -O2 -o audio_test $<
	./audio_test

$(TARGET): $(ALL_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(BUILDDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(BUILDDIR)/%.o: %.m
	@mkdir -p $(dir $@)
	$(OBJC) $(OBJCFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(TARGET)
