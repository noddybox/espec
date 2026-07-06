/*

    espec - Sinclair Spectrum emulator

    Copyright (C) 2026  Ian Cowburn (ianc@noddybox.demon.co.uk)

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

    Audio handling

*/

#ifndef ESPEC_AUDIO_H
#define ESPEC_AUDIO_H

#include <stdlib.h>
#include <SDL.h>

/* ---------------------------------------- INTERFACES
*/

/* Initialise the audio interface.  Returns FALSE on failure to open
   audio.  All other interfaces will silently do nothing if the device
   can't be opened.
*/
int		AUDIOInit(int frequency);


/* Add the passed buffer to the sound queue.  It is a signed 8-bit mono
   sample which will be converted as appropriate by the audio interface.
   Well turns out SDL does that for you.
*/
void		AUDIOQueue(Sint8 *buffer, size_t len);


#endif


/* END OF FILE */
