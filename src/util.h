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

    Usual library wrappers and utils

*/

#ifndef ESPEC_UTIL_H
#define ESPEC_UTIL_H "$Id$"

#include <stdlib.h>

/* ---------------------------------------- INTERFACES
*/

/* Returns result from malloc(size), calling Exit() if it fails.
*/
void		*Malloc(size_t size);


/* Copies a string.  The result must be freed.
*/
char		*StrCopy(const char *source);


/* Returns the filename portion of path.  Note returned pointer is pointing
   inside of path.
*/
const char	*Basename(const char *path);


/* Returns the directory portion of path.  Note returned pointer is internal
   static storage.  If there are no directory seperators in path, "." is
   returned.
*/
const char	*Dirname(const char *path);


#endif


/* END OF FILE */
