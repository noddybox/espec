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
#include <string.h>

#include "SDL.h"

#include "z80.h"
#include "spec.h"
#include "gfx.h"
#include "gui.h"
#include "memmenu.h"
#include "config.h"
#include "exit.h"
#include "util.h"


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
    char tape_in[FILENAME_MAX];
    char tape_out[FILENAME_MAX];
    Z80 *z80;
    SDL_Event *e;
    int quit;
    int trace;

    ConfigRead();

    trace=IConfig(CONF_TRACE);

    strcpy(tape_in,SConfig(CONF_TAPEDIR));
    strcpy(tape_out,SConfig(CONF_TAPEDIR));

    z80=Z80Init(SPECWriteMem,
    		SPECReadMem,
		SPECWriteWord,
		SPECReadWord,
		SPECWritePort,
		SPECReadPort,
		SPECReadForDisassem);

    Z80SetLabels(z80,SPECGetLabel());

    GFXInit();

    SPECInit(z80);

    white=GFXRGB(255,255,255);
    grey=GFXRGB(128,128,128);
    black=GFXRGB(0,0,0);

    quit=FALSE;

    /* Check for initial memory menu usage
       TODO: Proper switch handling
    */
    if (argc>1 && strcmp(argv[1],"-m")==0)
    	quit=MemoryMenu(z80);

    while(!quit)
    {
	const char *brk;

	Z80State s1,s2;

	Z80GetState(z80,&s1);
	Z80SingleStep(z80);
	Z80GetState(z80,&s2);

	if (s2.PC==0)
	{
	    Debug("PC zero - prev %4.4x\n",s1.PC);
	}

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
				   "(c) 2004 Ian Cowburn\n"
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
			GUIMessage(eMessageBox,"TODO","Sorry, not done yet");
		    break;

		case SDLK_F8:
		    if (e->key.state==SDL_PRESSED)
		    {
			if (GUIFileSelect("TAPE TO LOAD",TRUE,
					  tape_in,tape_in))
			{
			    SPECMount(SPEC_TAPE_IN,tape_in);
			}
		    }
		    break;

		case SDLK_F9:
		    if (e->key.state==SDL_PRESSED)
		    {
			if (GUIFileSelect("TAPE TO SAVE",FALSE,
					  tape_out,tape_out))
			{
			    SPECMount(SPEC_TAPE_OUT,tape_out);
			}
		    }
		    break;

		case SDLK_F10:
		    if (e->key.state==SDL_PRESSED)
		    {
			SPECUnmount(SPEC_TAPE_IN);
			SPECUnmount(SPEC_TAPE_OUT);
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


/* ---------------------------------------- PRIVATE FUNCTIONS
*/


/* END OF FILE */
