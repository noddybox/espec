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

    Provides the memory menu

*/
static const char ident[]="$Id$";

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "memmenu.h"
#include "spec.h"
#include "gfx.h"
#include "gui.h"

#include <SDL.h>

static const char ident_h[]=ESPEC_MEMMENU_H;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define WHITE	GFXRGB(255,255,255)
#define BLACK	GFXRGB(0,0,0)
#define RED	GFXRGB(255,100,100)
#define GREEN	GFXRGB(100,255,100)
#define BLUE	GFXRGB(100,100,255)

#define TRACE	"trace"


/* ---------------------------------------- STATIC DATA
*/
FILE	*trace=NULL;


/* ---------------------------------------- PRIVATE FUNCTIONS
*/
static void Centre(const char *p, int y, Uint32 col)
{
    GFXPrint((320-strlen(p)*8)/2,y,col,"%s",p);
}


static void DisplayMenu(void)
{
    static const char *menu[]=
    {
    	"1. Disassemble/Hex dump",
    	"2. Disassemble to file ",
	"3. Start/Stop trace log",
	"4. Playback trace log  ",
    	"5. Return              ",
	NULL
    };

    int f;

    GFXClear(BLACK);

    Centre("MEMORY MENU",0,WHITE);
    Centre("Select an option",10,RED);

    f=0;

    while(menu[f])
    {
    	Centre(menu[f],25+f*10,WHITE);
	f++;
    }
}


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


void DisplayZ80State(Z80State *s, int y, Uint32 col)
{
    GFXPrintPaper(0,y,col,BLACK,
		  "PC=%4.4x  A=%2.2x     F=%s",
		  s->PC,s->AF>>8,FlagString(s->AF&0xff));
    y+=8;
    GFXPrintPaper(0,y,col,BLACK,
    		  "BC=%4.4x  DE=%4.4x  HL=%4.4x",
		  s->BC,s->DE,s->HL);
    y+=8;
    GFXPrintPaper(0,y,col,BLACK,
    		  "IX=%4.4x  IY=%4.4x  SP=%4.4x",
		  s->IX,s->IY,s->SP);
    y+=8;
    GFXPrintPaper(0,y,col,BLACK,
    		  "I=%2.2x     IM=%2.2x    R=%2.2x",
		  s->I,s->IM,s->R);
    y+=8;
    GFXPrintPaper(0,y,col,BLACK,
    		  "IFF1=%2.2x  IFF2=%2.2x",
		  s->IFF1,s->IFF2);
}


static int StrEq(const char *a, const char *b)
{
    while(*a && *b && tolower(*a)==tolower(*b))
    {
    	a++;
	b++;
    }

    if (*a || *b)
	return FALSE;
    else
	return TRUE;
}


static Z80Word Address(Z80 *z80, const char *p)
{
    Z80State s;

    Z80GetState(z80,&s);

    if (StrEq(p,"AF"))
    	return s.AF;
    else if (StrEq(p,"BC"))
    	return s.BC;
    else if (StrEq(p,"DE"))
    	return s.DE;
    else if (StrEq(p,"HL"))
    	return s.HL;
    else if (StrEq(p,"IX"))
    	return s.IX;
    else if (StrEq(p,"IY"))
    	return s.IY;
    else if (StrEq(p,"SP"))
    	return s.SP;
    else if (StrEq(p,"PC"))
    	return s.PC;

    return (Z80Word)strtoul(p,NULL,0);
}


static int EnterAddress(const char *prompt, Z80 *z80, Z80Word *w)
{
    unsigned long ul;
    const char *p;

    p=GUIInputString(prompt ? prompt : "Address?","");

    if (*p)
    {
	ul=Address(z80,p);

	if (ul>0xffff)
	{
	    GUIMessage("ERROR","Bad address");
	}
	else
	{
	    *w=(Z80Word)ul;
	    return TRUE;
	}
    }

    return FALSE;
}


static void EnterLong(const char *prompt, long *l)
{
    const char *p;

    p=GUIInputString(prompt ? prompt : "Value?","");

    if (*p)
    	*l=strtol(p,NULL,0);
}


static void DoDisassem(Z80 *z80, const Z80State *s)
{
    static int hexmode=FALSE;
    Z80Word pc=s->PC;
    int quit=FALSE;

    while(!quit)
    {
	SDL_Event *e;
	Z80Word curr;
	Z80Word next;
	int f;

	GFXClear(BLACK);

    	Centre("DISASSEMBLY",0,WHITE);
    	Centre("Press F1 for help",9,RED);

	curr=pc;

	for(f=0;f<21;f++)
	{
	    char s[80];
	    char *p;
	    int y;

	    y=24+f*8;

	    GFXPrint(0,y,GREEN,"%4.4x",curr);

	    if (hexmode)
	    {
		int n;

	    	for(n=0;n<8;n++)
		{
		    GFXPrint(40+n*24,y,WHITE,"%2.2x",
		    		(int)SPECReadForDisassem(z80,curr));

		    curr++;
		}
	    }
	    else
	    {
		strcpy(s,Z80Disassemble(z80,&curr));

		p=strtok(s,";");
		GFXPrint(40,y,WHITE,"%s",p);
	    }

	    if (f==0)
		next=curr;
	}

	GFXEndFrame(FALSE);

	e=GFXWaitKey();

	switch(e->key.keysym.sym)
	{
	    case SDLK_F1:
		GUIMessage
		    ("DISASSEMBLY HELP","%s",
		       "ESC - Exit\n"
		       "H - Switch between disassembly/hex\n"
		       "Enter - Enter address to display\n"
		       "Cursor Up - Prev op\n"
		       "Cursor Down - Next op\n"
		       "Page Up - Move back a page\n"
		       "Page Down - Move forward a page\n"
		       "Cursor Left - Move PC by -1024\n"
		       "Cursor Right - Move PC by 1024");
		break;

	    case SDLK_ESCAPE:
	    	quit=TRUE;
		break;

	    case SDLK_h:
		hexmode=!hexmode;
	    	break;

	    case SDLK_RETURN:
	    case SDLK_KP_ENTER:
		EnterAddress(NULL,z80,&pc);
	    	break;

	    case SDLK_UP:
		if (hexmode)
		    pc-=8;
		else
		    pc--;
	    	break;

	    case SDLK_DOWN:
		pc=next;
	    	break;

	    case SDLK_PAGEUP:
		if (hexmode)
		    pc-=21*8;
		else
		    pc-=21;
		break;

	    case SDLK_PAGEDOWN:
	    	pc=curr;
		break;

	    case SDLK_LEFT:
	    	pc-=1024;
		break;

	    case SDLK_RIGHT:
	    	pc+=1024;
		break;

	    default:
	    	break;
	}
    }
}


static void DoDisassemFile(Z80 *z80, const Z80State *s)
{
    static char fname[FILENAME_MAX]="";
    FILE *fp;
    Z80Word start;
    Z80Word len;
    Z80Word prev;
    const char *p;

    EnterAddress("Disassemble from?",z80,&start);
    EnterAddress("For how many bytes?",z80,&len);

    p=GUIInputString("To file?",fname);

    if (!strlen(p))
    	return;

    strcpy(fname,p);

    if (!(fp=fopen(fname,"w")))
    {
    	GUIMessage("ERROR","Couldn't create file:\n%s",fname);
	return;
    }

    prev=len;

    while(len<=prev)
    {
    	Z80Word orig=start;

	fprintf(fp,"%4.4x: %s\n",orig,Z80Disassemble(z80,&start));
	prev=len;
	len-=(start-orig);
    }

    fclose(fp);
}


static int Instruction(Z80 *z80, Z80Val data)
{
    Z80State s;

    Z80GetState(z80,&s);
    fwrite(&s,sizeof s,1,trace);

    return TRUE;
}


static void EnableTrace(Z80 *z80, Z80State *s)
{
    if (!trace)
    {
    	trace=fopen(TRACE,"wb");

	if (trace)
	{
	    GUIMessage("NOTICE","Created trace log");
	    Z80LodgeCallback(z80,Z80_Instruction,Instruction);
	}
	else
	{
	    GUIMessage("ERROR","Failed to create trace log");
	}
    }
}


static void DisableTrace(Z80 *z80)
{
    if (trace)
    {
	GUIMessage("NOTICE","Closing current trace log");
    	fclose(trace);
	trace=NULL;
	Z80LodgeCallback(z80,Z80_Instruction,NULL);
    }
}


static void PlaybackTrace(Z80 *z80)
{
    FILE *fp;
    int quit;
    long prev_pos;
    long pos;
    long max_pos;
    Z80Word pc;
    Z80State s;

    fp=fopen(TRACE,"rb");

    if (!fp)
    {
	GUIMessage("ERROR","Couldn't open tracefile: " TRACE);
	return;
    }

    fseek(fp,0,SEEK_END);

    max_pos=ftell(fp)/sizeof s;

    if (max_pos==0)
    {
	GUIMessage("ERROR","Empty tracefile: " TRACE);
	fclose(fp);
	return;
    }

    pos=0;
    prev_pos=pos;
    pc=0;
    quit=FALSE;

    while(!quit)
    {
	SDL_Event *e;
	size_t rd;
	int f;

	GFXClear(BLACK);

    	Centre("TRACE PLAYBACK",0,WHITE);
    	Centre("Press F1 for help",9,RED);

	fseek(fp,pos*sizeof s,SEEK_SET);
	fread(&s,sizeof s,1,fp);

	DisplayZ80State(&s,136,WHITE);

	for(f=0;f<10;f++)
	{
	    char str[80];
	    char *p;
	    int paper;
	    int y;

	    y=24+f*8;

	    if (f==0)
		paper=RED;
	    else
		paper=BLACK;

	    GFXPrintPaper(0,y,GREEN,paper,"%4.4x ",s.PC);

	    strcpy(str,Z80Disassemble(z80,&s.PC));
	    p=strtok(str,";");
	    GFXPrintPaper(40,y,WHITE,paper,"%s",str);
	}

	GFXPrint(0,112,GREEN,"Current step: %ld",pos+1);
	GFXPrint(0,120,GREEN,"Total steps : %ld",max_pos);

	prev_pos=pos;

	GFXEndFrame(FALSE);

	e=GFXWaitKey();

	switch(e->key.keysym.sym)
	{
	    case SDLK_F1:
		GUIMessage
		    ("PLAYBACK HELP","%s",
		       "ESC - Exit\n"
		       "Enter - Step number to display\n"
		       "P - Search for PC\n"
		       "Cursor Up - Prev step\n"
		       "Cursor Down - Next step\n"
		       "Page Up - Back 50 steps\n"
		       "Page Down - Forward 50 steps\n"
		       "Cursor Left - Back 1000 steps\n"
		       "Cursor Right - Forward 1000 steps\n \n"
		       "NOTE: Disassembly is as memory is NOW");
		break;

	    case SDLK_ESCAPE:
	    	quit=TRUE;
		break;

	    case SDLK_RETURN:
	    case SDLK_KP_ENTER:
		EnterLong("Step number?",&pos);
		pos--;
		break;

	    case SDLK_p:
		if (EnterAddress("PC to search for?",z80,&pc))
		{
		    while((rd=fread(&s,sizeof s,1,fp))==1)
		    {
			pos++;
			if (s.PC==pc)
			    break;
		    }

		    if (rd!=1)
		    {
		    	GUIMessage("NOTICE","Address not found");
			pos=prev_pos;
		    }
		}
	    	break;

	    case SDLK_UP:
		pos--;
	    	break;

	    case SDLK_DOWN:
		pos++;
	    	break;

	    case SDLK_PAGEUP:
		pos-=50;
		break;

	    case SDLK_PAGEDOWN:
	    	pos+=50;
		break;

	    case SDLK_LEFT:
	    	pos-=1000;
		break;

	    case SDLK_RIGHT:
	    	pos+=1000;
		break;

	    default:
	    	break;
	}

	/* Check position before next loop
	*/
	if (pos<0)
	    pos=0;

	if (pos>=max_pos)
	    pos=max_pos-1;
    }

    fclose(fp);
}


/* ---------------------------------------- EXPORTED INTERFACES
*/
void MemoryMenu(Z80 *z80)
{
    SDL_Event *e;
    int quit=FALSE;
    Z80State s;

    Z80GetState(z80,&s);

    GFXKeyRepeat(TRUE);

    while(!quit)
    {
	DisplayMenu();
	DisplayState(z80);
	GFXEndFrame(FALSE);

	e=GFXWaitKey();

	switch(e->key.keysym.sym)
	{
	    case SDLK_1:
	    	DoDisassem(z80,&s);
		break;

	    case SDLK_2:
	    	DoDisassemFile(z80,&s);
	    	break;

	    case SDLK_3:
		if (!trace)
		    EnableTrace(z80,&s);
		else
		    DisableTrace(z80);
	    	break;

	    case SDLK_4:
		DisableTrace(z80);
	    	PlaybackTrace(z80);
		break;

	    case SDLK_ESCAPE:
	    case SDLK_5:
	    	quit=TRUE;
		break;

	    default:
	    	break;
	}
    }

    GFXKeyRepeat(FALSE);
}


void DisplayState(Z80 *z80)
{
    Z80State s;

    Z80GetState(z80,&s);
    DisplayZ80State(&s,136,RED);
}


/* END OF FILE */
