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

    Selected tape files

*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <sys/param.h>

#include "tape.h"
#include "util.h"
#include "gui.h"
#include "config.h"


/* ---------------------------------------- MACROS
*/
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


/* ---------------------------------------- GLOBALS
*/
static char tape_in[FILENAME_MAX]="";
static char tape_out[FILENAME_MAX]="";
static FILE *in;
static FILE *out;

/* ---------------------------------------- INTERFACES
*/

int TAPESelectInput(void)
{
    char path[FILENAME_MAX];

    if (GUIFileSelect("TAPE TO LOAD",TRUE,
                      tape_in[0] ? Dirname(tape_in) : SConfig(CONF_TAPEDIR),
                      path))
    {
        TAPEMount(TAP_IN, path);
        return TRUE;
    }

    return FALSE;
}


int TAPESelectOutput(void)
{
    char path[FILENAME_MAX];

    if (GUIFileSelect("TAPE TO SAVE",FALSE,
                      tape_out[0] ? Dirname(tape_out) : SConfig(CONF_TAPEDIR),
                      path))
    {
        TAPEMount(TAP_OUT, path);
        return TRUE;
    }

    return FALSE;
}


void TAPEMount(TAPEMountType type, const char *path)
{
    TAPEUnmount(type);

    switch(type)
    {
        case TAP_IN:
            strncpy(tape_in, path, FILENAME_MAX);
            in = fopen(tape_in, "rb");
            break;

        case TAP_OUT:
            strncpy(tape_out, path, FILENAME_MAX);
            out = fopen(tape_out, "wb");
            break;
    }
}


void TAPEUnmount(TAPEMountType type)
{
    switch(type)
    {
        case TAP_IN:
            if (in)
            {
                fclose(in);
                in = NULL;
            }
            tape_in[0] = 0;
            break;

        case TAP_OUT:
            if (out)
            {
                fclose(out);
                out = NULL;
            }
            tape_out[0] = 0;
            break;
    }
}


FILE *TAPEFile(TAPEMountType type)
{
    switch(type)
    {
        case TAP_IN:
            return in;

        case TAP_OUT:
            return out;

        default:
            return NULL;
    }
}


void TAPEDisplayInfo(void)
{
    GUIMessage(eMessageBox,
               "Mounted Tape Files",
               "In:  %-20.20s\nOut: %-20.20s",
               tape_in[0] ? Basename(tape_in):"None",
               tape_out[0] ? Basename(tape_out):"None");
}


/* END OF FILE */
