# espec - ZX Spectrum emulator
# 
# Copyright (C) 2003  Ian Cowburn (ianc@noddybox.demon.co.uk)
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# 
# -------------------------------------------------------------------------
# 
# $Id: Makefile,v 1.12 2006-09-11 00:35:48 ianc Exp $
# 


# This CFLAGS assumes that gcc is being used.
# Simply comment out if not, and replace as needed.
# Other CFLAGS lines *are* required.
#
CFLAGS	=	-g -O2

TARGET	=	espec

SOURCE	=	main.c		\
		spec.c		\
		snap.c		\
		config.c	\
		gfx.c		\
		gui.c		\
		memmenu.c	\
		util.c		\
		kbbmp.c		\
		exit.c		\
		tape.c		\
		symtochar.c	\
		font.c	        \
		expr.c		\
		z80.c		\
		z80_decode.c	\
		z80_dis.c

OBJECTS	=	main.o		\
		spec.o		\
		snap.o		\
		config.o	\
		gfx.o		\
		gui.o		\
		memmenu.o	\
		util.o		\
		kbbmp.o		\
		exit.o		\
		tape.o		\
		symtochar.o	\
		font.o	        \
		expr.o		\
		z80.o		\
		z80_decode.o	\
		z80_dis.o

CFLAGS +=	`sdl2-config --cflags` -DENABLE_DISASSEM

LIBS	=	`sdl2-config --libs`

$(TARGET): $(OBJECTS) 
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

depend:
	makedepend $(SOURCE)

clean:
	rm -f $(TARGET) $(TARGET).exe $(OBJECTS) core Makefile.bak

# DO NOT DELETE THIS LINE -- make depend depends on it.

main.o: z80.h spec.h gfx.h gui.h memmenu.h config.h kbbmp.h exit.h tape.h
main.o: util.h
spec.o: spec.h z80.h snap.h gfx.h gui.h config.h exit.h tape.h util.h
snap.o: snap.h z80.h util.h
config.o: exit.h config.h
gfx.o: gfx.h exit.h config.h util.h font.h
gui.o: gui.h gfx.h exit.h symtochar.h util.h
memmenu.o: memmenu.h z80.h spec.h gfx.h gui.h util.h expr.h
util.o: util.h exit.h
kbbmp.o: kbbmp.h
exit.o: exit.h
tape.o: tape.h util.h gui.h config.h
symtochar.o: symtochar.h
font.o: font.h
z80.o: z80.h z80_private.h
z80_decode.o: z80.h z80_private.h
