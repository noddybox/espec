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
CFLAGS	=	-g -Wall -Werror -pedantic -ansi -O2
#CFLAGS	=	-g -Wall -Werror -pedantic -ansi -O2 -pg

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
		expr.o		\
		z80.o		\
		z80_decode.o	\
		z80_dis.o

CFLAGS +=	`sdl-config --cflags` -DENABLE_DISASSEM

LIBS	=	`sdl-config --libs`

$(TARGET): $(OBJECTS) 
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

clean:
	rm -f $(TARGET) $(TARGET).exe $(OBJECTS) core

depend:
	makedepend  -- $(CFLAGS) -- $(SOURCE)
	if test -e Makefile ; then rm -f Makefile.bak ; fi

# DO NOT DELETE THIS LINE -- make depend depends on it

main.o: /usr/include/stdlib.h /usr/include/sys/cdefs.h
main.o: /usr/include/sys/_null.h /usr/include/sys/_types.h
main.o: /usr/include/machine/_types.h /usr/include/stdio.h
main.o: /usr/include/string.h /usr/include/strings.h
main.o: /usr/local/include/SDL11/SDL.h /usr/local/include/SDL11/SDL_main.h
main.o: /usr/local/include/SDL11/SDL_types.h
main.o: /usr/local/include/SDL11/SDL_getenv.h
main.o: /usr/local/include/SDL11/SDL_error.h
main.o: /usr/local/include/SDL11/begin_code.h
main.o: /usr/local/include/SDL11/close_code.h
main.o: /usr/local/include/SDL11/SDL_rwops.h
main.o: /usr/local/include/SDL11/SDL_timer.h
main.o: /usr/local/include/SDL11/SDL_audio.h
main.o: /usr/local/include/SDL11/SDL_byteorder.h
main.o: /usr/local/include/SDL11/SDL_cdrom.h
main.o: /usr/local/include/SDL11/SDL_joystick.h
main.o: /usr/local/include/SDL11/SDL_events.h
main.o: /usr/local/include/SDL11/SDL_active.h
main.o: /usr/local/include/SDL11/SDL_keyboard.h
main.o: /usr/local/include/SDL11/SDL_keysym.h
main.o: /usr/local/include/SDL11/SDL_mouse.h
main.o: /usr/local/include/SDL11/SDL_video.h
main.o: /usr/local/include/SDL11/SDL_mutex.h
main.o: /usr/local/include/SDL11/SDL_quit.h
main.o: /usr/local/include/SDL11/SDL_version.h z80.h spec.h gfx.h gui.h
main.o: memmenu.h config.h kbbmp.h exit.h util.h
spec.o: /usr/include/stdlib.h /usr/include/sys/cdefs.h
spec.o: /usr/include/sys/_null.h /usr/include/sys/_types.h
spec.o: /usr/include/machine/_types.h /usr/include/stdio.h
spec.o: /usr/include/string.h /usr/include/strings.h spec.h z80.h
spec.o: /usr/local/include/SDL11/SDL.h /usr/local/include/SDL11/SDL_main.h
spec.o: /usr/local/include/SDL11/SDL_types.h
spec.o: /usr/local/include/SDL11/SDL_getenv.h
spec.o: /usr/local/include/SDL11/SDL_error.h
spec.o: /usr/local/include/SDL11/begin_code.h
spec.o: /usr/local/include/SDL11/close_code.h
spec.o: /usr/local/include/SDL11/SDL_rwops.h
spec.o: /usr/local/include/SDL11/SDL_timer.h
spec.o: /usr/local/include/SDL11/SDL_audio.h
spec.o: /usr/local/include/SDL11/SDL_byteorder.h
spec.o: /usr/local/include/SDL11/SDL_cdrom.h
spec.o: /usr/local/include/SDL11/SDL_joystick.h
spec.o: /usr/local/include/SDL11/SDL_events.h
spec.o: /usr/local/include/SDL11/SDL_active.h
spec.o: /usr/local/include/SDL11/SDL_keyboard.h
spec.o: /usr/local/include/SDL11/SDL_keysym.h
spec.o: /usr/local/include/SDL11/SDL_mouse.h
spec.o: /usr/local/include/SDL11/SDL_video.h
spec.o: /usr/local/include/SDL11/SDL_mutex.h
spec.o: /usr/local/include/SDL11/SDL_quit.h
spec.o: /usr/local/include/SDL11/SDL_version.h snap.h gfx.h gui.h config.h
spec.o: exit.h util.h
snap.o: snap.h /usr/include/stdlib.h /usr/include/sys/cdefs.h
snap.o: /usr/include/sys/_null.h /usr/include/sys/_types.h
snap.o: /usr/include/machine/_types.h /usr/include/stdio.h z80.h util.h
config.o: /usr/include/stdlib.h /usr/include/sys/cdefs.h
config.o: /usr/include/sys/_null.h /usr/include/sys/_types.h
config.o: /usr/include/machine/_types.h /usr/include/stdio.h
config.o: /usr/include/string.h /usr/include/strings.h exit.h config.h
gfx.o: /usr/include/stdlib.h /usr/include/sys/cdefs.h
gfx.o: /usr/include/sys/_null.h /usr/include/sys/_types.h
gfx.o: /usr/include/machine/_types.h /usr/include/stdio.h
gfx.o: /usr/include/string.h /usr/include/strings.h /usr/include/stdarg.h
gfx.o: gfx.h /usr/local/include/SDL11/SDL.h
gfx.o: /usr/local/include/SDL11/SDL_main.h
gfx.o: /usr/local/include/SDL11/SDL_types.h
gfx.o: /usr/local/include/SDL11/SDL_getenv.h
gfx.o: /usr/local/include/SDL11/SDL_error.h
gfx.o: /usr/local/include/SDL11/begin_code.h
gfx.o: /usr/local/include/SDL11/close_code.h
gfx.o: /usr/local/include/SDL11/SDL_rwops.h
gfx.o: /usr/local/include/SDL11/SDL_timer.h
gfx.o: /usr/local/include/SDL11/SDL_audio.h
gfx.o: /usr/local/include/SDL11/SDL_byteorder.h
gfx.o: /usr/local/include/SDL11/SDL_cdrom.h
gfx.o: /usr/local/include/SDL11/SDL_joystick.h
gfx.o: /usr/local/include/SDL11/SDL_events.h
gfx.o: /usr/local/include/SDL11/SDL_active.h
gfx.o: /usr/local/include/SDL11/SDL_keyboard.h
gfx.o: /usr/local/include/SDL11/SDL_keysym.h
gfx.o: /usr/local/include/SDL11/SDL_mouse.h
gfx.o: /usr/local/include/SDL11/SDL_video.h
gfx.o: /usr/local/include/SDL11/SDL_mutex.h
gfx.o: /usr/local/include/SDL11/SDL_quit.h
gfx.o: /usr/local/include/SDL11/SDL_version.h exit.h config.h font.h
gui.o: /usr/include/stdlib.h /usr/include/sys/cdefs.h
gui.o: /usr/include/sys/_null.h /usr/include/sys/_types.h
gui.o: /usr/include/machine/_types.h /usr/include/stdio.h
gui.o: /usr/include/string.h /usr/include/strings.h /usr/include/stdarg.h
gui.o: /usr/include/ctype.h /usr/include/_ctype.h /usr/include/runetype.h
gui.o: /usr/include/unistd.h /usr/include/sys/types.h
gui.o: /usr/include/machine/endian.h /usr/include/sys/_pthreadtypes.h
gui.o: /usr/include/sys/select.h /usr/include/sys/_sigset.h
gui.o: /usr/include/sys/_timeval.h /usr/include/sys/timespec.h
gui.o: /usr/include/sys/unistd.h /usr/include/dirent.h
gui.o: /usr/include/sys/dirent.h /usr/include/sys/stat.h
gui.o: /usr/include/sys/time.h /usr/include/time.h /usr/include/sys/param.h
gui.o: /usr/include/sys/syslimits.h /usr/include/sys/signal.h
gui.o: /usr/include/machine/signal.h /usr/include/machine/trap.h
gui.o: /usr/include/machine/param.h /usr/include/sys/limits.h
gui.o: /usr/include/machine/_limits.h gui.h gfx.h
gui.o: /usr/local/include/SDL11/SDL.h /usr/local/include/SDL11/SDL_main.h
gui.o: /usr/local/include/SDL11/SDL_types.h
gui.o: /usr/local/include/SDL11/SDL_getenv.h
gui.o: /usr/local/include/SDL11/SDL_error.h
gui.o: /usr/local/include/SDL11/begin_code.h
gui.o: /usr/local/include/SDL11/close_code.h
gui.o: /usr/local/include/SDL11/SDL_rwops.h
gui.o: /usr/local/include/SDL11/SDL_timer.h
gui.o: /usr/local/include/SDL11/SDL_audio.h
gui.o: /usr/local/include/SDL11/SDL_byteorder.h
gui.o: /usr/local/include/SDL11/SDL_cdrom.h
gui.o: /usr/local/include/SDL11/SDL_joystick.h
gui.o: /usr/local/include/SDL11/SDL_events.h
gui.o: /usr/local/include/SDL11/SDL_active.h
gui.o: /usr/local/include/SDL11/SDL_keyboard.h
gui.o: /usr/local/include/SDL11/SDL_keysym.h
gui.o: /usr/local/include/SDL11/SDL_mouse.h
gui.o: /usr/local/include/SDL11/SDL_video.h
gui.o: /usr/local/include/SDL11/SDL_mutex.h
gui.o: /usr/local/include/SDL11/SDL_quit.h
gui.o: /usr/local/include/SDL11/SDL_version.h exit.h util.h
memmenu.o: /usr/include/stdlib.h /usr/include/sys/cdefs.h
memmenu.o: /usr/include/sys/_null.h /usr/include/sys/_types.h
memmenu.o: /usr/include/machine/_types.h /usr/include/stdio.h
memmenu.o: /usr/include/string.h /usr/include/strings.h /usr/include/ctype.h
memmenu.o: /usr/include/_ctype.h /usr/include/runetype.h memmenu.h z80.h
memmenu.o: spec.h /usr/local/include/SDL11/SDL.h
memmenu.o: /usr/local/include/SDL11/SDL_main.h
memmenu.o: /usr/local/include/SDL11/SDL_types.h
memmenu.o: /usr/local/include/SDL11/SDL_getenv.h
memmenu.o: /usr/local/include/SDL11/SDL_error.h
memmenu.o: /usr/local/include/SDL11/begin_code.h
memmenu.o: /usr/local/include/SDL11/close_code.h
memmenu.o: /usr/local/include/SDL11/SDL_rwops.h
memmenu.o: /usr/local/include/SDL11/SDL_timer.h
memmenu.o: /usr/local/include/SDL11/SDL_audio.h
memmenu.o: /usr/local/include/SDL11/SDL_byteorder.h
memmenu.o: /usr/local/include/SDL11/SDL_cdrom.h
memmenu.o: /usr/local/include/SDL11/SDL_joystick.h
memmenu.o: /usr/local/include/SDL11/SDL_events.h
memmenu.o: /usr/local/include/SDL11/SDL_active.h
memmenu.o: /usr/local/include/SDL11/SDL_keyboard.h
memmenu.o: /usr/local/include/SDL11/SDL_keysym.h
memmenu.o: /usr/local/include/SDL11/SDL_mouse.h
memmenu.o: /usr/local/include/SDL11/SDL_video.h
memmenu.o: /usr/local/include/SDL11/SDL_mutex.h
memmenu.o: /usr/local/include/SDL11/SDL_quit.h
memmenu.o: /usr/local/include/SDL11/SDL_version.h gfx.h gui.h util.h
util.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
util.o: /usr/include/sys/_null.h /usr/include/sys/_types.h
util.o: /usr/include/machine/_types.h /usr/include/string.h
util.o: /usr/include/strings.h /usr/include/stdarg.h /usr/include/sys/param.h
util.o: /usr/include/sys/types.h /usr/include/machine/endian.h
util.o: /usr/include/sys/_pthreadtypes.h /usr/include/sys/select.h
util.o: /usr/include/sys/_sigset.h /usr/include/sys/_timeval.h
util.o: /usr/include/sys/timespec.h /usr/include/sys/syslimits.h
util.o: /usr/include/sys/signal.h /usr/include/machine/signal.h
util.o: /usr/include/machine/trap.h /usr/include/machine/param.h
util.o: /usr/include/sys/limits.h /usr/include/machine/_limits.h util.h
util.o: /usr/include/stdlib.h exit.h
kbbmp.o: kbbmp.h
exit.o: /usr/include/stdlib.h /usr/include/sys/cdefs.h
exit.o: /usr/include/sys/_null.h /usr/include/sys/_types.h
exit.o: /usr/include/machine/_types.h /usr/include/stdarg.h exit.h
exit.o: /usr/local/include/SDL11/SDL.h /usr/local/include/SDL11/SDL_main.h
exit.o: /usr/local/include/SDL11/SDL_types.h
exit.o: /usr/local/include/SDL11/SDL_getenv.h
exit.o: /usr/local/include/SDL11/SDL_error.h
exit.o: /usr/local/include/SDL11/begin_code.h
exit.o: /usr/local/include/SDL11/close_code.h
exit.o: /usr/local/include/SDL11/SDL_rwops.h /usr/include/stdio.h
exit.o: /usr/local/include/SDL11/SDL_timer.h
exit.o: /usr/local/include/SDL11/SDL_audio.h
exit.o: /usr/local/include/SDL11/SDL_byteorder.h
exit.o: /usr/local/include/SDL11/SDL_cdrom.h
exit.o: /usr/local/include/SDL11/SDL_joystick.h
exit.o: /usr/local/include/SDL11/SDL_events.h
exit.o: /usr/local/include/SDL11/SDL_active.h
exit.o: /usr/local/include/SDL11/SDL_keyboard.h
exit.o: /usr/local/include/SDL11/SDL_keysym.h
exit.o: /usr/local/include/SDL11/SDL_mouse.h
exit.o: /usr/local/include/SDL11/SDL_video.h
exit.o: /usr/local/include/SDL11/SDL_mutex.h
exit.o: /usr/local/include/SDL11/SDL_quit.h
exit.o: /usr/local/include/SDL11/SDL_version.h
expr.o: /usr/include/stdlib.h /usr/include/sys/cdefs.h
expr.o: /usr/include/sys/_null.h /usr/include/sys/_types.h
expr.o: /usr/include/machine/_types.h /usr/include/stdio.h
expr.o: /usr/include/string.h /usr/include/strings.h /usr/include/ctype.h
expr.o: /usr/include/_ctype.h /usr/include/runetype.h
z80.o: /usr/include/stdlib.h /usr/include/sys/cdefs.h
z80.o: /usr/include/sys/_null.h /usr/include/sys/_types.h
z80.o: /usr/include/machine/_types.h /usr/include/string.h
z80.o: /usr/include/strings.h z80.h z80_private.h
z80_decode.o: /usr/include/stdlib.h /usr/include/sys/cdefs.h
z80_decode.o: /usr/include/sys/_null.h /usr/include/sys/_types.h
z80_decode.o: /usr/include/machine/_types.h z80.h z80_private.h
z80_dis.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
z80_dis.o: /usr/include/sys/_null.h /usr/include/sys/_types.h
z80_dis.o: /usr/include/machine/_types.h /usr/include/string.h
z80_dis.o: /usr/include/strings.h /usr/include/stdarg.h z80.h z80_private.h
