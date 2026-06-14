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
		expr.o		\
		z80.o		\
		z80_decode.o	\
		z80_dis.o

CFLAGS +=	`sdl2-config --cflags` -DENABLE_DISASSEM

LIBS	=	`sdl2-config --libs`

$(TARGET): $(OBJECTS) 
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

clean:
	rm -f $(TARGET) $(TARGET).exe $(OBJECTS) core

