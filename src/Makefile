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

CFLAGS	=	-g `sdl2-config --cflags` -Igfx-bitmap -DENABLE_DISASSEM


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
		font.c	        \
		audio.c	        \
		expr.c		\
		joystick.c	\
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
		font.o	        \
		audio.o	        \
		expr.o		\
		joystick.o	\
		z80.o		\
		z80_decode.o	\
		z80_dis.o	\
		gfx-bitmap.o

LIBS	=	`sdl2-config --libs`

$(TARGET): $(OBJECTS) 
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

depend.mak: $(SOURCE) *.h
	$(CC) $(CFLAGS) -MM $(SOURCE) > depend.mak

kbbmp.c: ../gfx/keyb.gfxbmp
	cfile8 -h template -c template ../gfx/keyb.gfxbmp kbbmp.c keyboard_bitmap

../gfx/keyb.gfxbmp: ../gfx/keyb.png
	cd ../gfx ; $(MAKE)

clean:
	rm -f $(TARGET) $(TARGET).exe $(OBJECTS) core depend.mak

gfx-bitmap.o: gfx-bitmap/gfx-bitmap.c gfx-bitmap/gfx-bitmap.h
	$(CC) -c -o gfx-bitmap.o gfx-bitmap/gfx-bitmap.c

include depend.mak
