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

    Utilities for handling snapshots

*/
static const char ident[]="$Id$";

#include "snap.h"
#include "util.h"

static const char ident_h[]=ESPEC_SNAP_H;


/* ---------------------------------------- MACROS
*/
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define ROMLEN 0x4000


/* ---------------------------------------- PRIVATE FUNCTIONS
*/
static Z80Word GetLSBWord(FILE *fp)
{
    if (fp)
    {
        int c1,c2;

        c1=getc(fp);
        c2=getc(fp);

        if (c1==EOF || c2==EOF)
            return 0;

        return c1+(c2<<8);
    }
    else
        return 0;
}


static void PutLSBWord(FILE *fp, Z80Word w)
{
    if (fp)
    {
	putc(w&0xff,fp);
	putc(w>>8,fp);
    }
}


static Z80Byte GetByte(FILE *fp)
{
    int c;

    if (!fp || (c=getc(fp))==EOF)
        return 0;

    return c;
}


static void PutByte(FILE *fp, Z80Byte b)
{
    putc(b,fp);
}



/* ---------------------------------------- INTERFACES
*/
int TAPLoad(FILE *fp, Z80Byte id, Z80Word *addr, Z80Word *len, SNAP_Poke poke)
{
    Z80Word blen;
    Z80Byte type,b,csum,tape_csum;

    b=0;

    if (!fp)
    {
	Debug("No file to load!\n");
    	return FALSE;
    }

    /* Get length (wrapping file if at eof)
    */
    blen=GetLSBWord(fp);

    if (feof(fp))
    {
    	rewind(fp);
	blen=GetLSBWord(fp);
    }

    Debug("blen=%u  type=%u\n",id);

    type=GetByte(fp);
    csum=id;

    /* Have we found the requested block?
    */
    if (id==type)
    {
	Debug("Matched type\n");
    	/* Knock of block type
	*/
	blen--;

	while(blen && *len)
	{
	    b=GetByte(fp);
	    csum^=b;

	    poke(*addr,b);

	    (*addr)++;
	    (*len)--;
	    blen--;
	}

	/* Get the checksum
	*/
	if (blen)
	{
	    tape_csum=GetByte(fp);
	    blen--;
	}
	else
	    tape_csum=b;

	/* In case we've been request less bytes than the block size
	*/
	while(blen--)
	    fgetc(fp);

	/* Check the checksum
	*/
	Debug("csum = %d, tape csum = %d\n",csum,tape_csum);
	return csum==tape_csum;
    }
    else
    {
    	/* If it's the wrong type, skip it
	*/
	while(blen--)
	    fgetc(fp);

	return FALSE;
    }
}


int TAPSave(FILE *fp, Z80Byte id, Z80Word *addr, Z80Word *len, SNAP_Peek peek)
{
    Z80Byte csum;

    if (!fp)
    	return FALSE;

    /* Write out the length and ID byte
    */
    PutLSBWord(fp,(*len)+2);
    PutByte(fp,id);

    /* Write out data and calc checksum
    */
    csum=id;

    while(*len)
    {
	Z80Byte b;

	b=peek(*addr);

    	PutByte(fp,b);
	csum^=b;
	(*addr)++;
	(*len)--;
    }

    PutByte(fp,csum);

    return TRUE;
}


/* END OF FILE */
