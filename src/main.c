/*

    espec - Sinclair Spectrum emulator

    Copyright (C) 2003  Ian Cowburn (ianc@noddybox.demon.co.uk)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    -------------------------------------------------------------------------

*/
static const char id[]="$Id$";

#include <stdlib.h>
#include <stdio.h>

#include "SDL.h"

#include "z80.h"
#include "spec.h"
#include "gfx.h"
#include "gui.h"
#include "memmenu.h"
#include "config.h"
#include "exit.h"


/* ---------------------------------------- MACROS
*/
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


/* ---------------------------------------- STATICS
*/
static Uint32	white;
static Uint32	black;
static Uint32	grey;


/* ---------------------------------------- PROTOS
*/


/* ---------------------------------------- MAIN
*/
int main(int argc, char *argv[])
{
    Z80 *z80;
    SDL_Event *e;
    int quit;
    int trace;

    ConfigRead();

    trace=IConfig(CONF_TRACE);

    z80=Z80Init(SPECWriteMem,
    		SPECReadMem,
		SPECWritePort,
		SPECReadPort,
		SPECReadForDisassem,
		SPECGetLabel);

    GFXInit();

    SPECInit(z80);

    white=GFXRGB(255,255,255);
    grey=GFXRGB(128,128,128);
    black=GFXRGB(0,0,0);

    quit=FALSE;

    while(!quit)
    {
	Z80State s1,s2;

	Z80GetState(z80,&s1);
	Z80SingleStep(z80);
	Z80GetState(z80,&s2);

	if (s2.PC==0)
	{
	    printf("PC zero - prev %4.4x\n",s1.PC);
	}

	if (trace)
	{
	    DisplayState(z80);
	    GFXEndFrame(FALSE);
	}

	while((e=GFXGetKey()))
	{
	    switch (e->key.keysym.sym)
	    {
	    	case SDLK_ESCAPE:
		    if (e->key.state==SDL_PRESSED)
			quit=TRUE;
		    break;

		case SDLK_F11:
		    if (e->key.state==SDL_PRESSED)
		    	MemoryMenu(z80);
		    break;

		case SDLK_F12:
		    if (e->key.state==SDL_PRESSED)
		    	trace=!trace;
		    break;

		default:
		    SPECKeyEvent(e);
		    break;
	    }
	}
    }

    return EXIT_SUCCESS;
}


/* ---------------------------------------- PRIVATE FUNCTIONS
*/


/* END OF FILE */
