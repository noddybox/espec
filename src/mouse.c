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
#include "mouse.h"
#include "util.h"

/* ---------------------------------------- STATICS
*/
static MouseButtonCallback	button_callback;
static MouseMoveCallback	move_callback;

/* ---------------------------------------- PRIVATE FUNCTIONS
*/
static MouseButton MapIndex(Uint8 button)
{
}

/* ---------------------------------------- EXPORTED FUNCTIONS
*/
void MOUSEPump(SDL_Event *e)
{
    switch (e->type)
    {
	case SDL_MOUSEMOTION:
	    if (move_callback)
	    {
	    	move_callback(e->motion.x, e->motion.y,
			      e->motion.xrel, e->motion.yrel);
	    }
	    break;

	case SDL_MOUSEBUTTONDOWN:
	    if (button_callback)
	    {
		button_callback(MapIndex(e->button.button), TRUE);
	    }
	    break;

	case SDL_MOUSEBUTTONUP:
	    if (button_callback)
	    {
		button_callback(MapIndex(e->button.button), FALSE);
	    }
	    break;

	default:
	    break;
    }
}


void MOUSERegister(MouseButtonCallback button, MouseMoveCallback move)
{
    button_callback = button;
    move_callback = move;
}
