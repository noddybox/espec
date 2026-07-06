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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SDL.h"

#include "z80.h"
#include "spec.h"
#include "gfx.h"
#include "gui.h"
#include "memmenu.h"
#include "config.h"
#include "kbbmp.h"
#include "exit.h"
#include "tape.h"
#include "util.h"
#include "audio.h"

/* ---------------------------------------- MACROS
*/
#define ESPEC_VERSION "V1.1-dev"

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


/* ---------------------------------------- PRIVATE FUNCTIONS
*/
static void Usage(void)
{
    fprintf(stderr,"usage: espec [-m] [-l tape_file] [-s tape_file]\n");
    exit(EXIT_FAILURE);
}


/* ---------------------------------------- MAIN
*/
int main(int argc, char *argv[])
{
    Z80 *z80;
    SDL_Event *e;
    int quit;
    int trace;
    int inital_menu;
    int f;

    ConfigRead();

    trace=IConfig(CONF_TRACE);

    z80=Z80Init(SPECPeek,SPECPoke,SPECReadPort,SPECWritePort,SPECDisPeek);

    /* Z80SetLabels(z80,SPECGetLabel()); */

    GFXInit();

    if (IConfig(CONF_SOUND))
    {
    	if (!AUDIOInit(SPECAudioFrequency()))
	{
	    fprintf(stderr, "warning: couldn't initialise audio\n");
	}
    }

    SPECInit(z80);

    white=GFXRGB(255,255,255);
    grey=GFXRGB(128,128,128);
    black=GFXRGB(0,0,0);

    quit=FALSE;

    /* Parse switches
    */
    inital_menu=FALSE;
    f=1;

    while(f<argc && argv[f][0]=='-')
    {
	switch(argv[f][1])
	{
	    case 'm':
		inital_menu=TRUE;
		break;

	    case 'l':
	    	if (f>argc-2)
		{
		    Usage();
		}

                TAPEMount(TAP_IN, argv[++f]);
		break;

	    case 's':
	    	if (f>argc-2)
		{
		    Usage();
		}

                TAPEMount(TAP_OUT, argv[++f]);
		break;

	    default:
	    	Usage();
		break;
	}

	f++;
    }

    if (inital_menu)
    {
    	quit=MemoryMenu(z80);
    }

    /* Main loop
    */
    while(!quit)
    {
	const char *brk;

	Z80SingleStep(z80);

	if (trace)
	{
	    DisplayState(z80);
	    GFXEndFrame(FALSE);
	}

	if ((brk=Break()))
	{
	    GUIMessage(eMessageBox,"BREAKPOINT","%s",brk);
	    quit=MemoryMenu(z80);
	}

	while(!quit && (e=GFXGetKey()))
	{
	    switch (e->key.keysym.sym)
	    {
	    	case SDLK_ESCAPE:
		    if (e->key.state==SDL_PRESSED)
			quit=GUIMessage(eYesNoBox,"QUIT","Sure?");
		    break;

		case SDLK_F1:
		    if (e->key.state==SDL_PRESSED)
			GUIMessage(eMessageBox,
				   "HELP",
				   "ESC - Quit                        \n"
				   "F1  - Help                        \n"
				   "F2  - About                       \n"
				   "F3  - View Spectrum keyboad       \n"
				   "F4  - View mounted tapes          \n"
				   "F8  - Select tape file for loading\n"
				   "F9  - Select tape file for saving \n"
				   "F10 - Close all open tape files   \n"
				   "F11 - Memory Menu                 \n"
				   "F12 - Toggle onscreen trace       ");
		    break;

		case SDLK_F2:
		    if (e->key.state==SDL_PRESSED)
			GUIMessage(eMessageBox,
				   "espec - ZX Spectrum Emulator",
				   "(c) 2004-2026 Ian Cowburn "
				   ESPEC_VERSION "\n"
				   " \n"
				   "This software comes with ABSOLUTELY \n"
				   "NO WARRANTY, and you are free to    \n"
				   "to redistribute it under certain    \n"
				   "conditions.  See the supplied GNU   \n"
				   "General Public License in LICENSE   \n"
				   "for details.                        \n"
				   " \n"
				   "If you did not recieve a license,   \n"
				   "vist www.gnu.org or wrote to:       \n"
				   " \n"
				   "Free Software Foundation, Inc.,     \n"
				   "59 Temple Place, Suite 330,         \n"
				   "Boston, MA 02111-1307 USA           ");
		    break;

		case SDLK_F3:
		    if (e->key.state==SDL_PRESSED)
		    {
			GFXBitmap((GFX_WIDTH-KBBMP_WIDTH)/2,
				  (GFX_HEIGHT-KBBMP_HEIGHT)/2,
				  KBBMP_WIDTH, KBBMP_HEIGHT,
				  keyboard_bitmap);

			GFXEndFrame(FALSE);
			GFXWaitKey();
		    }
		    break;

		case SDLK_F4:
                    TAPEDisplayInfo();
		    break;


		case SDLK_F8:
		    if (e->key.state==SDL_PRESSED)
		    {
                        TAPESelectInput();
		    }
		    break;

		case SDLK_F9:
		    if (e->key.state==SDL_PRESSED)
		    {
                        TAPESelectOutput();
		    }
		    break;

		case SDLK_F10:
		    if (e->key.state==SDL_PRESSED)
		    {
			TAPEUnmount(TAP_IN);
			TAPEUnmount(TAP_OUT);
		    }
		    break;

		case SDLK_F11:
		    if (e->key.state==SDL_PRESSED)
		    	quit=MemoryMenu(z80);
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


/* END OF FILE */
