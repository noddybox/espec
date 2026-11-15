/*

    espec - Sinclair Spectrum emulator

    Copyright (C) 2026  Ian Cowburn (deathstation9000@gmail.com)

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

    Mouse handling

*/
#ifndef ESPEC_MOUSE_H
#define ESPEC_MOUSE_H

#include <SDL.h>

/* Buttons
*/
typedef enum
{
    MOUSE_Left,
    MOUSE_Right,
    MOUSE_Middle,
    MOUSE_NumButtons
} MouseButton;

/* Callback for mouse button events
*/
typedef void (*MouseButtonCallback)(MouseButton button, int pressed);
typedef void (*MouseMoveCallback)(int x, int y, int dx, int dy);


/* Pump mouse events
*/
void		MOUSEPump(SDL_Event *e);


/* Register callbacks for mouse events
*/
void		MOUSERegister(MouseButtonCallback button,
			      MouseMoveCallback move);


#endif
