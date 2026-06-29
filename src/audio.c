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

#include <SDL.h>

#include "audio.h"
#include "util.h"

/* ---------------------------------------- INTERNAL DATA
*/
static SDL_AudioSpec		actual;
static SDL_AudioDeviceID	device;


/* ---------------------------------------- INTERFACES
*/
int AUDIOInit(void)
{
    SDL_AudioSpec spec = {0};

    spec.freq = SAMPLE_RATE;
    spec.format = AUDIO_S8;
    spec.channels = 1;
    spec.samples = 4096;

    device = SDL_OpenAudioDevice(NULL, 0, &spec, &actual, 0);

    if (device != 0)
    {
    	SDL_PauseAudioDevice(device, 0);
    }

    return device != 0;
}

void AUDIOQueue(Uint8 *buffer, size_t len)
{
    if (device != 0)
    {
	SDL_QueueAudio(device, buffer, len);
    }
}


/* END OF FILE */
