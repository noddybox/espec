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

    Provides the emulation for the Spectrum

*/
static const char ident[]="$Id$";

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "spec.h"
#include "snap.h"
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

#define HIBYTE(w)	((w)>>8)
#define LOBYTE(w)	((w)&0xff)


/* ---------------------------------------- STATICS
*/
static const int	ROMLEN=0x4000;
static const int	ROM_SAVE=0x4c6;
static const int	ROM_LOAD=0x562;

static FILE		*tape_in;
static FILE		*tape_out;

#define LOAD_PATCH	0xf0
#define SAVE_PATCH	0xf1

/* The SPEC screen
*/
#define			SCR_W	256
#define			SCR_H	192
#define			TXT_W	32
#define			TXT_H	24
#define			SCRDATA	0x4000
#define			ATTR	0x5800

#define			ATTR_AT(x,y)		\
				mem[ATTR+(x)+((y)/8)*32]

static const int	OFF_X=(GFX_WIDTH-SCR_W)/2;
static const int	OFF_Y=(GFX_HEIGHT-SCR_H)/2;

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

static Z80Byte		*line[SCRL];	/* Accelerators to screen data */

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


/* ---------------------------------------- DEBUG FUNCTIONS
static const char *FlagString(Z80Byte flag)
{
    static char s[]="76543210";
    static char c[]="SZ5H3PNC";
    int f;

    for(f=0;f<8;f++)
        if (flag&(1<<(7-f)))
            s[f]=c[f];
        else
            s[f]='-';

    return s;
}


static void DumpZ80(Z80 *z80)
{
    Z80State s;

    Z80GetState(z80,&s);

    printf("---------------------------------\n");
    printf("PC=%4.4x  A=%2.2x     F=%s\n",s.PC,s.AF>>8,FlagString(s.AF&0xff));
    printf("BC=%4.4x  DE=%4.4x  HL=%4.4x\n",s.BC,s.DE,s.HL);
    printf("IX=%4.4x  IY=%4.4x  SP=%4.4x\n",s.IX,s.IY,s.SP);
    printf("I=%2.2x     IM=%2.2x    R=%2.2x\n",s.I,s.IM,s.R);
    printf("IFF1=%2.2x  IFF2=%2.2x\n",s.IFF1,s.IFF2);
    printf("%s\n",Z80Disassemble(z80,&s.PC));
}
*/


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

    GFXHLine(0,GFX_WIDTH-1,y,coltable[border].col);

    if (aline>=0 && aline<SCRL)
    {
	GFXLock();

	scr=line[aline];

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

	    for(r=0,b=*scr++;r<8;r++)
		if (b&(1<<(8-r)))
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
    	0xed, SAVE_PATCH,	/* ED illegal op	*/
	0xc9,			/* RET 			*/
	0xff			/* End of patch		*/
    };

    static const Z80Byte load[]=
    {
	0x08,			/* EX AF,AF'		*/
    	0xed, LOAD_PATCH,	/* ED illegal op	*/
	0xc9,			/* RET 			*/
	0xff			/* End of patch		*/
    };

    int f;

    for(f=0;save[f]!=0xff;f++)
    	mem[ROM_SAVE+f]=save[f];

    for(f=0;load[f]!=0xff;f++)
    	mem[ROM_LOAD+f]=load[f];
}



static int EDCallback(Z80 *z80, Z80Val data)
{
    Z80State state;

    Z80GetState(z80,&state);

    switch((Z80Byte)data)
    {
    	case SAVE_PATCH:
	    if (!tape_out)
	    {
		state.AF|=eZ80_Carry;
	    }
	    else
	    {
	    	if (TAPSave(tape_out,HIBYTE(state.AF),&state.IX,&state.DE,mem))
		{
		    state.AF&=~eZ80_Carry;
		}
		else
		{
		    state.AF|=eZ80_Carry;
		}
	    }
	    break;

    	case LOAD_PATCH:
	    if (!tape_in)
	    {
		state.AF|=eZ80_Carry;
	    }
	    else
	    {
	    	if (TAPLoad(tape_in,HIBYTE(state.AF),&state.IX,&state.DE,mem))
		{
		    state.AF&=~eZ80_Carry;
		}
		else
		{
		    state.AF|=eZ80_Carry;
		}
	    }
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
	*/
	y=scanline-TOPL+OFF_Y;

	if (y>=0 && y<GFX_HEIGHT)
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
    int f;
    int c;
    int r;

    if (!(fp=fopen(SConfig(CONF_ROMFILE),"rb")))
    {
	GUIMessage(eMessageBox,
		   "ERROR",
		   "Failed to open Spectrum ROM\n%s",
		   SConfig(CONF_ROMFILE));
	Exit("");
    }

    if (fread(mem,1,ROMLEN,fp)!=ROMLEN)
    {
    	fclose(fp);
	GUIMessage(eMessageBox,
		   "ERROR",
		   "ROM file must be %d bytes long\n",
		   ROMLEN);
	Exit("");
    }

    /* Patch the ROM
    */
    RomPatch();
    Z80LodgeCallback(z80,eZ80_EDHook,EDCallback);
    Z80LodgeCallback(z80,eZ80_Instruction,CheckTimers);

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

    /* Set up screen
    */
    c=0;
    r=0;
    for(f=0;f<SCRL;f++)
    {
    	line[f]=mem+SCRDATA+(c*8*TXT_W)+(r*TXT_W);

	c++;

	if ((c%8)==0)
	{
	    if (++r==8)
	    	r=0;
	    else
	    	c-=8;
	}
    }

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


Z80Word SPECReadWord(Z80 *z80, Z80Word addr)
{
    /* TODO: Emulation of contention */
    return (Z80Word)mem[addr]|(Z80Word)mem[addr+1]<<8;
}


void SPECWriteWord(Z80 *z80, Z80Word addr, Z80Word val)
{
    if (addr>=ROMLEN)
	mem[addr]=val&0xff;

    addr++;

    if (addr>=ROMLEN)
	mem[addr]=val>>8;
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

	default:	/* ULA */
	    if (!(lo&1))
	    {
		/* Key matrix
		*/
		b=0xff;

		for(f=0;f<8;f++)
		    if (!(hi&(1<<f)))
			b&=matrix[f];

		b|=0xa0;

		/* TODO: Emulation of contention? */
	    }
	    else
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
	case 0xfe:	/* ULA */
	    border=val&0x07;
	    break;

	case 0xfb:	/* TODO: ZX Printer */
	    break;

	default:
	    break;
    }
}


Z80Byte SPECReadForDisassem(Z80 *z80, Z80Word addr)
{
    return mem[addr];
}


/* TODO: Implement this as a binary search
*/
const char *SPECGetLabel(Z80 *z80, Z80Word addr)
{
    static const struct
    {
    	Z80Word		addr;
	const char	*txt;
    } label[]=
    	{
	    {0x5c00,	"KSTATE"},
	    {0x5c01,	"KSTATE+1"},
	    {0x5c02,	"KSTATE+2"},
	    {0x5c03,	"KSTATE+3"},
	    {0x5c04,	"KSTATE+4"},
	    {0x5c05,	"KSTATE+5"},
	    {0x5c06,	"KSTATE+6"},
	    {0x5c07,	"KSTATE+7"},
	    {0x5c08,	"LAST_K"},
	    {0x5c09,	"REPDEL"},
	    {0x5c0a,	"REPPER"},
	    {0x5c0b,	"DEFADD"},
	    {0x5c0c,	"DEFADD+1"},
	    {0x5c0d,	"K_DATA"},
	    {0x5c0e,	"TVDATA"},
	    {0x5c0f,	"TVDATA+1"},
	    {0x5c10,	"STRMS"},
	    {0x5c11,	"STRMS+1"},
	    {0x5c12,	"STRMS+2"},
	    {0x5c13,	"STRMS+3"},
	    {0x5c14,	"STRMS+4"},
	    {0x5c15,	"STRMS+5"},
	    {0x5c16,	"STRMS+6"},
	    {0x5c17,	"STRMS+7"},
	    {0x5c18,	"STRMS+8"},
	    {0x5c19,	"STRMS+9"},
	    {0x5c1a,	"STRMS+10"},
	    {0x5c1b,	"STRMS+11"},
	    {0x5c1c,	"STRMS+12"},
	    {0x5c1d,	"STRMS+13"},
	    {0x5c1e,	"STRMS+14"},
	    {0x5c1f,	"STRMS+15"},
	    {0x5c20,	"STRMS+16"},
	    {0x5c21,	"STRMS+17"},
	    {0x5c22,	"STRMS+18"},
	    {0x5c23,	"STRMS+19"},
	    {0x5c24,	"STRMS+20"},
	    {0x5c25,	"STRMS+21"},
	    {0x5c26,	"STRMS+22"},
	    {0x5c27,	"STRMS+23"},
	    {0x5c28,	"STRMS+24"},
	    {0x5c29,	"STRMS+25"},
	    {0x5c2a,	"STRMS+26"},
	    {0x5c2b,	"STRMS+27"},
	    {0x5c2c,	"STRMS+28"},
	    {0x5c2d,	"STRMS+29"},
	    {0x5c2e,	"STRMS+30"},
	    {0x5c2f,	"STRMS+31"},
	    {0x5c30,	"STRMS+32"},
	    {0x5c31,	"STRMS+33"},
	    {0x5c32,	"STRMS+34"},
	    {0x5c33,	"STRMS+35"},
	    {0x5c34,	"STRMS+36"},
	    {0x5c35,	"STRMS+37"},
	    {0x5c36,	"CHARS"},
	    {0x5c37,	"CHARS+1"},
	    {0x5c38,	"RASP"},
	    {0x5c39,	"PIP"},
	    {0x5c3a,	"ERR_NR"},
	    {0x5c3b,	"FLAGS"},
	    {0x5c3c,	"TV_FLAG"},
	    {0x5c3d,	"ERR_SP"},
	    {0x5c3e,	"ERR_SP+1"},
	    {0x5c3f,	"LIST_SP"},
	    {0x5c40,	"LIST_SP+1"},
	    {0x5c41,	"MODE"},
	    {0x5c42,	"NEWPPC"},
	    {0x5c43,	"NEWPPC+1"},
	    {0x5c44,	"NSPPC"},
	    {0x5c45,	"PPC"},
	    {0x5c46,	"PPC+1"},
	    {0x5c47,	"SUBPPC"},
	    {0x5c48,	"BORDCR"},
	    {0x5c49,	"E_PPC"},
	    {0x5c4a,	"E_PPC+1"},
	    {0x5c4b,	"VARS"},
	    {0x5c4c,	"VARS+1"},
	    {0x5c4d,	"DEST"},
	    {0x5c4e,	"DEST+1"},
	    {0x5c4f,	"CHANS"},
	    {0x5c50,	"CHANS+1"},
	    {0x5c51,	"CURCHL"},
	    {0x5c52,	"CURCHL+1"},
	    {0x5c53,	"PROG"},
	    {0x5c54,	"PROG+1"},
	    {0x5c55,	"NXTLIN"},
	    {0x5c56,	"NXTLIN+1"},
	    {0x5c57,	"DATADD"},
	    {0x5c58,	"DATADD+1"},
	    {0x5c59,	"E_LINE"},
	    {0x5c5a,	"E_LINE+1"},
	    {0x5c5b,	"K_CUR"},
	    {0x5c5c,	"K_CUR+1"},
	    {0x5c5d,	"CH_ADD"},
	    {0x5c5e,	"CH_ADD+1"},
	    {0x5c5f,	"X_PTR"},
	    {0x5c60,	"X_PTR+1"},
	    {0x5c61,	"WORKSP"},
	    {0x5c62,	"WORKSP+1"},
	    {0x5c63,	"STKBOT"},
	    {0x5c64,	"STKBOT+1"},
	    {0x5c65,	"STKEND"},
	    {0x5c66,	"STKEND+1"},
	    {0x5c67,	"BREG"},
	    {0x5c68,	"MEM"},
	    {0x5c69,	"MEM+1"},
	    {0x5c6a,	"FLAGS2"},
	    {0x5c6b,	"DF_SZ"},
	    {0x5c6c,	"S_TOP"},
	    {0x5c6d,	"S_TOP+1"},
	    {0x5c6e,	"OLDPPC"},
	    {0x5c6f,	"OLDPPC+1"},
	    {0x5c70,	"OSPCC"},
	    {0x5c71,	"FLAGX"},
	    {0x5c72,	"STRLEN"},
	    {0x5c73,	"STRLEN+1"},
	    {0x5c74,	"T_ADDR"},
	    {0x5c75,	"T_ADDR+1"},
	    {0x5c76,	"SEED"},
	    {0x5c77,	"SEED+1"},
	    {0x5c78,	"FRAMES"},
	    {0x5c79,	"FRAMES+1"},
	    {0x5c7a,	"FRAMES+2"},
	    {0x5c7b,	"UDG"},
	    {0x5c7c,	"UDG+1"},
	    {0x5c7d,	"COORDS_X"},
	    {0x5c7e,	"COORDS_Y"},
	    {0x5c7f,	"P_POSN"},
	    {0x5c80,	"PR_CC"},
	    {0x5c82,	"ECHO_E"},
	    {0x5c83,	"ECHO_E+1"},
	    {0x5c84,	"DF_CC"},
	    {0x5c85,	"DF_CC+1"},
	    {0x5c86,	"DFCCL"},
	    {0x5c87,	"DFCCL+1"},
	    {0x5c88,	"S_POSN_X"},
	    {0x5c89,	"S_POSN_Y"},
	    {0x5c8a,	"S_POSNL_X"},
	    {0x5c8b,	"S_POSNL_Y"},
	    {0x5c8c,	"SCR_CT"},
	    {0x5c8d,	"ATTR_P"},
	    {0x5c8e,	"MASK_P"},
	    {0x5c8f,	"ATTR_T"},
	    {0x5c90,	"MASK_T"},
	    {0x5c91,	"P_FLAG"},
	    {0x5c92,	"MEMBOT"},
	    {0x5c93,	"MEMBOT+1"},
	    {0x5c94,	"MEMBOT+2"},
	    {0x5c95,	"MEMBOT+3"},
	    {0x5c96,	"MEMBOT+4"},
	    {0x5c97,	"MEMBOT+5"},
	    {0x5c98,	"MEMBOT+6"},
	    {0x5c99,	"MEMBOT+7"},
	    {0x5c9a,	"MEMBOT+8"},
	    {0x5c9b,	"MEMBOT+9"},
	    {0x5c9c,	"MEMBOT+10"},
	    {0x5c9d,	"MEMBOT+11"},
	    {0x5c9e,	"MEMBOT+12"},
	    {0x5c9f,	"MEMBOT+13"},
	    {0x5ca0,	"MEMBOT+14"},
	    {0x5ca1,	"MEMBOT+15"},
	    {0x5ca2,	"MEMBOT+16"},
	    {0x5ca3,	"MEMBOT+17"},
	    {0x5ca4,	"MEMBOT+18"},
	    {0x5ca5,	"MEMBOT+19"},
	    {0x5ca6,	"MEMBOT+20"},
	    {0x5ca7,	"MEMBOT+21"},
	    {0x5ca8,	"MEMBOT+22"},
	    {0x5ca9,	"MEMBOT+23"},
	    {0x5caa,	"MEMBOT+24"},
	    {0x5cab,	"MEMBOT+25"},
	    {0x5cac,	"MEMBOT+26"},
	    {0x5cad,	"MEMBOT+27"},
	    {0x5cae,	"MEMBOT+28"},
	    {0x5caf,	"MEMBOT+29"},
	    {0x5cb2,	"RAMTOP"},
	    {0x5cb3,	"RAMTOP+1"},
	    {0x5cb4,	"P_RAMT"},
	    {0x5cb5,	"P_RAMT+1"},
	    {0,		NULL}
	};

    int f;

    for(f=0;label[f].txt;f++)
    	if (addr==label[f].addr)
	    return label[f].txt;

    return NULL;
}


const char *SPECInfo(Z80 *z80)
{
    static char buff[80]={0};

    return buff;
}


void SPECMount(SPECMountType type, const char *path)
{
    switch(type)
    {
    	case SPEC_TAPE_IN:
	    if (tape_in)
	    	fclose(tape_in);

	    tape_in=fopen(path,"rb");
	    break;

    	case SPEC_TAPE_OUT:
	    if (tape_out)
	    	fclose(tape_out);

	    tape_out=fopen(path,"wb");
	    break;
    }
}


void SPECUnmount(SPECMountType type)
{
    switch(type)
    {
    	case SPEC_TAPE_IN:
	    if (tape_in)
	    {
	    	fclose(tape_in);
		tape_in=NULL;
	    }
	    break;

    	case SPEC_TAPE_OUT:
	    if (tape_out)
	    {
	    	fclose(tape_out);
		tape_out=NULL;
	    }
	    break;
    }
}


/* END OF FILE */
