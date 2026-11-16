/*

    espec - Sinclair Spectrum emulator

    Copyright (C) 2003  Ian Cowburn (deathstation9000@gmail.com)

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

#include <SDL.h>

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
#include "joystick.h"
#include "gfx-bitmap.h"

/* ---------------------------------------- MACROS
*/
#define ESPEC_VERSION "V1.5-dev"

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


static void JoystickMenu(void)
{
    enum opts
    {
    	SELECT,
    	MAP_KEYS,
    	MAP_PREDEFINED,
	SAVE,
	LOAD,
	CLEAR,
	DISPLAY,
	CLOSE
    };

    static const GUIMenuItem menu[] = 
    {
	{SELECT,	"Select Joytick",		NULL},
	{MAP_KEYS,	"Map Keys to Joytick",		NULL},
	{MAP_PREDEFINED,"Select predefined Mapping",	NULL},
	{SAVE,		"Save Joystick Mappings",	NULL},
	{LOAD,		"Load Joystick Mappings",	NULL},
	{CLEAR,		"Clear Joystick Key Mappings",	NULL},
	{DISPLAY,	"Display Joystick Mappings",	NULL},
	{CLOSE,		"Close Menu",			NULL},
	{0,		NULL,				NULL}
    };

    const GUIMenuItem *item;
    int selected = -1;

    item = GUIMenu("JOYSTICK MENU", selected, menu);

    while(item)
    {
	switch (item->value)
	{
	    case SELECT:
		JOYSelect();
		break;

	    case MAP_KEYS:
		SPECSetupJoystick();
		break;

	    case MAP_PREDEFINED:
		SPECSetupPredefinedJoystick();
		break;

	    case SAVE:
		SPECSaveJoystick();
		break;

	    case LOAD:
		SPECLoadJoystick(NULL);
		break;

	    case CLEAR:
		SPECClearJoystick();
		break;

	    case DISPLAY:
		SPECDisplayJoytick();
		break;

	    case CLOSE:
		return;
	}

	selected = item->value;
	item = GUIMenu("JOYSTICK MENU", selected, menu);
    }
}

static void TapeMenu(void)
{
    enum opts
    {
    	VIEW,
    	LOADING,
    	SAVING,
	CLOSE_ALL,
	CLOSE
    };

    static const GUIMenuItem menu[] = 
    {
	{VIEW,		"View mounted tapes",			NULL},
	{LOADING,	"Select tape file for loading",		NULL},
	{SAVING,	"Select tape file for saving",		NULL},
	{CLOSE_ALL,	"Close all open tape files",		NULL},
	{CLOSE,		"Close Menu",				NULL},
	{0,		NULL,					NULL}
    };

    const GUIMenuItem *item;
    int selected = -1;

    item = GUIMenu("TAPE MENU", selected, menu);

    while(item)
    {
	switch (item->value)
	{
	    case VIEW:
		TAPEDisplayInfo();
		break;

	    case LOADING:
		TAPESelectInput();
		break;

	    case SAVING:
		TAPESelectOutput();
		break;

	    case CLOSE_ALL:
		TAPEUnmount(TAP_IN);
		TAPEUnmount(TAP_OUT);
		break;

	    case CLOSE:
	    	return;
	}

	selected = item->value;
	item = GUIMenu("TAPE MENU", selected, menu);
    }
}

static void ConfigureDevices(void)
{
    enum constants
    {
    	NUM = 5
    };

    static IConfigVar config[NUM] =
    {
    	CONF_KEMPSTON_JOYSTICK,
    	CONF_FULLER_JOYSTICK,
    	CONF_AMX_MOUSE,
    	CONF_KEMPSTON_MOUSE,
	CONF_ZX_PRINTER
    };

    const char * const items[NUM] =
    {
    	IConfigDescription(CONF_KEMPSTON_JOYSTICK),
    	IConfigDescription(CONF_FULLER_JOYSTICK),
    	IConfigDescription(CONF_AMX_MOUSE),
    	IConfigDescription(CONF_KEMPSTON_MOUSE),
    	IConfigDescription(CONF_ZX_PRINTER)
    };

    int options[NUM] =
    {
    	IConfig(CONF_KEMPSTON_JOYSTICK),
    	IConfig(CONF_FULLER_JOYSTICK),
    	IConfig(CONF_AMX_MOUSE),
    	IConfig(CONF_KEMPSTON_MOUSE),
	IConfig(CONF_ZX_PRINTER)
    };

    if (GUIListOption("SELECT DEVICES", NUM, items, options))
    {
    	for(int f = 0; f < NUM; f++)
	{
	    IConfigSet(config[f], options[f]);
	}

	if (GUIMessage(eYesNoBox, "SAVE CONFIG", "Save configuration"))
	{
	    ConfigSave();
	}
    }
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
    GFX_Bitmap bitmap;

    ConfigRead();

    trace=IConfig(CONF_TRACE);

    z80=Z80Init(SPECPeek,SPECPoke,SPECReadPort,SPECWritePort,SPECDisPeek);

    Z80SetLabels(z80,SPECGetLabel());

    if (GFX_Bitmap_Decode(keyboard_bitmap,
    			  KEYBOARD_BITMAP_LEN,
			  &bitmap) != eGFX_Ok)
    {
	fprintf(stderr, "error: failed to decoded keyboard bitmap\n");
	return EXIT_FAILURE;
    }

    GFXInit();
    GUIInit();
    JOYInit();

    if (IConfig(CONF_SOUND))
    {
    	if (!AUDIOInit(SPECAudioFrequency()))
	{
	    fprintf(stderr, "warning: couldn't initialise audio\n");
	}
    }

    GFXSetTextInput(FALSE);

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
				   "F4  - Close open TAP file for load\n"
				   "F5  - Reset                       \n"
				   "F6  - Joystick Menu               \n"
				   "F7  - Tape Menu                   \n"
				   "F8  - Configure attached devices  \n"
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
			GFXBitmap(&bitmap);
			GFXEndFrame(FALSE);
			GFXWaitKey(NULL);
		    }
		    break;

		case SDLK_F4:
		    TAPEUnmount(TAP_IN);
		    break;

		case SDLK_F5:
		    if (GUIMessage(eYesNoBox,"RESET","Sure?"))
		    {
			Z80Reset(z80);
			SPECReset(z80);
		    }
		    break;

		case SDLK_F6:
		    JoystickMenu();
		    break;

		case SDLK_F7:
		    TapeMenu();
		    break;

		case SDLK_F8:
		    ConfigureDevices();
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

    TAPEUnmount(TAP_IN);
    TAPEUnmount(TAP_OUT);

    return EXIT_SUCCESS;
}
