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

#ifndef ESPEC_SPECH
#define ESPEC_SPECH

#include "z80.h"
#include "SDL.h"

/* Initialise the SPEC
*/
void		SPECInit(Z80 *z80);

/* Handle keypresses
*/
void		SPECKeyEvent(SDL_Event *e);

/* Interfaces for the Z80
*/
Z80Byte		SPECPeek(Z80 *z80, Z80Word addr);
void		SPECPoke(Z80 *z80, Z80Word addr, Z80Byte val);

#define SPECDisPeek SPECPeek

Z80Byte		SPECReadPort(Z80 *z80, Z80Word port);
void		SPECWritePort(Z80 *z80, Z80Word port, Z80Byte val);

/* const Z80Label	*SPECGetLabel(void); */

/* Interfaces for memory menu
*/
const char	*SPECInfo(Z80 *z80);
void		SPECEnableScreen(int enable);
void		SPECShowScreen(void);

/* Called when the machine is reset
*/
void		SPECReset(Z80 *z80);

/* Return the audio frequency wanted by the Spectrum
*/
int		SPECAudioFrequency(void);

#endif


/* END OF FILE */
