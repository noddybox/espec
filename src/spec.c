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

    Provides the emulation for the SPEC

*/
static const char ident[]="$Id$";

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "spec.h"
#include "gfx.h"
#include "gui.h"
#include "config.h"
#include "exit.h"

static const char ident_h[]=ESPEC_SPECH;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


/* ---------------------------------------- STATICS
*/
static const int	ROMLEN=0x4000;
static const int	ROM_SAVE=0x4c6;
static const int	ROM_LOAD=0x562;

/* The SPEC screen
*/
#define			GFX_W	320
#define			GFX_H	300
#define			SCR_W	256
#define			SCR_H	192
#define			TXT_W	32
#define			TXT_H	24
#define			SCRDATA	0x4000
#define			ATTR	0x5800

#define			ATTR_AT(x,y)		\
				mem[ATTR+(x)+((y)/8)*32]

static const int	OFF_X=(GFX_W-SCR_W)/2;
static const int	OFF_Y=(GFX_H-SCR_H)/2;

static Z80Byte		mem[0x10000];


/* Number of cycles per scan lines and scan line control
*/
static Z80Val		SCAN_CYCLES=224;
static int		scanline=0;


/* GFX vars
*/
#define			FLASH	16	/* Frames per flash */

static int		flash=0;
static int		flashctr=0;

static int		border=0;

#define			TOPL	64	/* Scanlines before 'first' line */
#define			SCRL	SCR_H	/* Scanlines making up screen data */
#define			BOTL	56	/* Scanlines after 'last' line */

#define			TOTL	(TOPL+SCRL+BOTL)

#define 		NVAL	235	/* Normal RGB intensity */
#define 		BVAL	255	/* Bright RGB intensity */

static struct
{
    Uint32	col;
    int		r,g,b;
} coltable[16]=
{
    {0,     0x00,0x00,0x00},        /* BLACK */
    {0,     0x00,0x00,NVAL},        /* BLUE */
    {0,     NVAL,0x00,0x00},        /* RED */
    {0,     NVAL,0x00,NVAL},        /* MAGENTA */
    {0,     0x00,NVAL,0x00},        /* GREEN */
    {0,     0x00,NVAL,NVAL},        /* CYAN */
    {0,     NVAL,NVAL,0x00},        /* YELLOW */
    {0,     NVAL,NVAL,NVAL},        /* WHITE */

    {0,     0x00,0x00,0x00},        /* BLACK */
    {0,     0x00,0x00,BVAL},        /* BLUE */
    {0,     BVAL,0x00,0x00},        /* RED */
    {0,     BVAL,0x00,BVAL},        /* MAGENTA */
    {0,     0x00,BVAL,0x00},        /* GREEN */
    {0,     0x00,BVAL,BVAL},        /* CYAN */
    {0,     BVAL,BVAL,0x00},        /* YELLOW */
    {0,     BVAL,BVAL,BVAL},        /* WHITE */

};


/* The keyboard
*/
static Z80Byte		matrix[8];

typedef struct
{
    SDLKey	key;
    int		m1,b1,m2,b2;
} MatrixMap;

#define KY1(m,b)		m,1<<b,-1,-1
#define KY2(m1,b1,m2,b2)	m1,1<<b1,m2,1<<b2

static const MatrixMap keymap[]=
{
    {SDLK_1,		KY1(3,0)},
    {SDLK_2,		KY1(3,1)},
    {SDLK_3,		KY1(3,2)},
    {SDLK_4,		KY1(3,3)},
    {SDLK_5,		KY1(3,4)},
    {SDLK_6,		KY1(4,4)},
    {SDLK_7,		KY1(4,3)},
    {SDLK_8,		KY1(4,2)},
    {SDLK_9,		KY1(4,1)},
    {SDLK_0,		KY1(4,0)},

    {SDLK_a,		KY1(1,0)},
    {SDLK_b,		KY1(7,4)},
    {SDLK_c,		KY1(0,3)},
    {SDLK_d,		KY1(1,2)},
    {SDLK_e,		KY1(2,2)},
    {SDLK_f,		KY1(1,3)},
    {SDLK_g,		KY1(1,4)},
    {SDLK_h,		KY1(6,4)},
    {SDLK_i,		KY1(5,2)},
    {SDLK_j,		KY1(6,3)},
    {SDLK_k,		KY1(6,2)},
    {SDLK_l,		KY1(6,1)},
    {SDLK_m,		KY1(7,2)},
    {SDLK_n,		KY1(7,3)},
    {SDLK_o,		KY1(5,1)},
    {SDLK_p,		KY1(5,0)},
    {SDLK_q,		KY1(2,0)},
    {SDLK_r,		KY1(2,3)},
    {SDLK_s,		KY1(1,1)},
    {SDLK_t,		KY1(2,4)},
    {SDLK_u,		KY1(5,3)},
    {SDLK_v,		KY1(0,4)},
    {SDLK_w,		KY1(2,1)},
    {SDLK_x,		KY1(0,2)},
    {SDLK_y,		KY1(5,4)},
    {SDLK_z,		KY1(0,1)},

    {SDLK_RETURN,	KY1(6,0)},
    {SDLK_SPACE,	KY1(7,0)},

    {SDLK_BACKSPACE,	KY2(0,0,4,0)},
    {SDLK_DELETE,	KY2(0,0,4,0)},
    {SDLK_UP,		KY2(0,0,4,3)},
    {SDLK_DOWN,		KY2(0,0,4,4)},
    {SDLK_LEFT,		KY2(0,0,3,4)},
    {SDLK_RIGHT,	KY2(0,0,4,2)},

    {SDLK_LSHIFT,	KY1(0,0)},
    {SDLK_RSHIFT,	KY1(7,1)},
    {SDLK_LCTRL,	KY1(0,0)},
    {SDLK_RCTRL,	KY1(7,1)},
    {SDLK_LALT,		KY1(0,0)},
    {SDLK_RALT,		KY1(7,1)},
    {SDLK_CAPSLOCK,	KY2(0,0,3,1)},

    {SDLK_UNKNOWN,	0,0,0,0},
};


/* ---------------------------------------- PRIVATE FUNCTIONS
*/
void DrawScanline(int y)
{
    int aline;
    int f,r;
    int ink,paper,t;
    Z80Byte *scr;
    Z80Byte b;
    Z80Byte att;

    aline=scanline-TOPL;

    GFXHLine(0,GFX_W-1,y,coltable[border].col);

    if (aline>=0 && aline<SCRL)
    {
	GFXLock();

	scr=mem+SCRDATA+aline*TXT_W;

	for(f=0;f<TXT_W;f++)
	{
	    att=ATTR_AT(f,aline);

	    ink=(att&0x07);
	    paper=(att&0x38)>>3;

	    if (att&0x40)
	    {
		ink+=8;
		paper+=8;
	    }

	    if ((att&0x80)&&(flash))
	    {
		t=ink;
		ink=paper;
		paper=t;
	    }

	    for(r=7,b=*scr++;r>=0;r--)
		if (b&(1<<r))
		    GFXFastPlot(f*8+r+OFF_X,y,coltable[ink].col);
		else
		    GFXFastPlot(f*8+r+OFF_X,y,coltable[paper].col);
	}

	GFXUnlock();
    }
}


static void RomPatch(void)
{
    static const Z80Byte save[]=
    {
    	0xed, 0xf0,		/* ED F0 illegal op	*/
	0xc9,			/* RET 			*/
	0xff			/* End of patch		*/
    };

    static const Z80Byte load[]=
    {
	0x08,			/* EX AF,AF'		*/
    	0xed, 0xf1,		/* ED F1 illegal op	*/
	0xc9,			/* RET 			*/
	0xff			/* End of patch		*/
    };

    int f;

    for(f=0;save[f]!=0xff;f++)
    	mem[ROM_SAVE+f]=save[f];

    for(f=0;load[f]!=0xff;f++)
    	mem[ROM_LOAD+f]=load[f];
}



static void LoadTape(Z80State *state)
{
    state->AF|=Z80_F_Carry;
}


static void SaveTape(Z80State *state)
{
    state->AF|=Z80_F_Carry;
}


static int EDCallback(Z80 *z80, Z80Val data)
{
    Z80State state;

    Z80GetState(z80,&state);

    switch((Z80Byte)data)
    {
    	case 0xf0:
	    SaveTape(&state);
	    break;

    	case 0xf1:
	    LoadTape(&state);
	    break;

	default:
	    break;
    }

    Z80SetState(z80,&state);

    return TRUE;
}


static int CheckTimers(Z80 *z80, Z80Val val)
{
    if (val>SCAN_CYCLES)
    {
	int y;

	Z80ResetCycles(z80,val-SCAN_CYCLES);

	/* Increment scan line and check for frame flyback
	*/
	scanline++;

	if (scanline==TOTL)
	{
	    scanline=0;

	    flashctr++;

	    if (flashctr==FLASH)
	    {
	    	flash^=1;
		flashctr=0;
	    }

	    Z80Interrupt(z80,0xff);

	    GFXEndFrame(TRUE);
	    GFXStartFrame();
	}

	/* Draw scanline
	y=OFF_X-TOPL+scanline;
	*/
	y=scanline-TOPL+OFF_Y;

	if (y>=0 && y<GFX_H)
	    DrawScanline(y);

	/* TODO: Process sound emulation */
    }

    return TRUE;
}


/* ---------------------------------------- EXPORTED INTERFACES
*/
void SPECInit(Z80 *z80)
{
    FILE *fp;
    Z80Word f;

    if (!(fp=fopen(SConfig(CONF_ROMFILE),"rb")))
    {
	GUIMessage("ERROR","Failed to open Spectrum ROM\n%s",
						SConfig(CONF_ROMFILE));
	Exit("");
    }

    if (fread(mem,1,ROMLEN,fp)!=ROMLEN)
    {
    	fclose(fp);
	GUIMessage("ERROR","ROM file must be %d bytes long\n",ROMLEN);
	Exit("");
    }

    /* Patch the ROM
    */
    RomPatch();
    Z80LodgeCallback(z80,Z80_EDHook,EDCallback);
    Z80LodgeCallback(z80,Z80_Fetch,CheckTimers);

    /* Set up the keyboard
    */
    for(f=0;f<8;f++)
    	matrix[f]=0x1f;

    /* Set up the colours
    */
    for(f=0;f<16;f++)
    	coltable[f].col=GFXRGB(coltable[f].r,coltable[f].g,coltable[f].b);

    scanline=0;
    flash=0;
    flashctr=0;

    GFXStartFrame();
}


void SPECKeyEvent(SDL_Event *e)
{
    const MatrixMap *m;

    m=keymap;

    while(m->key!=SDLK_UNKNOWN)
    {
    	if (e->key.keysym.sym==m->key)
	{
	    if (e->key.state==SDL_PRESSED)
	    {
	    	matrix[m->m1]&=~m->b1;

		if (m->m2!=-1)
		    matrix[m->m2]&=~m->b2;
	    }
	    else
	    {
	    	matrix[m->m1]|=m->b1;

		if (m->m2!=-1)
		    matrix[m->m2]|=m->b2;
	    }
	}

	m++;
    }
}


Z80Byte SPECReadMem(Z80 *z80, Z80Word addr)
{
    /* TODO: Emulation of contention */
    return mem[addr];
}


void SPECWriteMem(Z80 *z80, Z80Word addr, Z80Byte val)
{
    if (addr>=ROMLEN)
	mem[addr]=val;
}


Z80Byte SPECReadPort(Z80 *z80, Z80Word port)
{
    Z80Byte lo=port&0xff;
    Z80Byte hi=port>>8;
    Z80Byte b=0xff;
    int f;

    switch(lo)
    {
	case 0x1f:	/* TODO: Kempston joystick */
	    break;

	case 0x7f:	/* TODO: Fuller joystick */
	    break;

	case 0xfb:	/* TODO: ZX Printer */
	    break;

    	case 0x01:	/* ULA */
	    /* Key matrix
	    */
	    b=0;

	    for(f=0;f<8;f++)
	    	if (!(hi&(1<<f)))
		    b&=matrix[f];

	    b|=0xa0;

	    /* TODO: Emulation of contention */

	    break;

	default:
	    b=0xff;
	    break;
    }

    return b;
}


void SPECWritePort(Z80 *z80, Z80Word port, Z80Byte val)
{
    Z80Byte lo=port&0xff;

    switch(lo)
    {
	case 0x01:	/* ULA */
	    border=val&0x07;
	    break;

	default:
	    break;
    }
}


Z80Byte SPECReadForDisassem(Z80 *z80, Z80Word addr)
{
    return mem[addr];
}


const char *SPECInfo(Z80 *z80)
{
    static char buff[80]={0};

    return buff;
}


/* END OF FILE */
