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

    Seleted tape files

*/

#ifndef ESPEC_TAPE_H
#define ESPEC_TAPE_H

#include <stdio.h>

/* ---------------------------------------- TYPES
*/

/* Types for Mount and Unmount
*/
typedef enum {TAP_IN, TAP_OUT} TAPEMountType;


/* ---------------------------------------- INTERFACES
*/

/* Select the tape file to use for input.  Returns TRUE if a tape was
   selected.
*/
int		TAPESelectInput(void);


/* Select the tape file to use for output.  Returns TRUE if a tape was
   selected.
*/
int		TAPESelectOutput(void);


/* Mount a file without using the selector
*/
void            TAPEMount(TAPEMountType type, const char *path);


/* Unmount a file without using the selector
*/
void            TAPEUnmount(TAPEMountType type);


/* Get the current tape
*/
FILE            *TAPEFile(TAPEMountType type);


/* Display tape info in a message box
*/
void            TAPEDisplayInfo(void);


#endif


/* END OF FILE */
