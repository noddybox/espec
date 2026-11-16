/*

    espec - Sinclair Spectrum emulator

    Copyright (C) 2003  Ian Cowburn (deathstation9000@gmail.com)

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

    Config file

*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "exit.h"
#include "config.h"
#include "util.h"

/* ---------------------------------------- CONFIG
*/
static char	config_path[MAX_PATH_BUFF];

static char	rompath[MAX_PATH_BUFF]="./spec48.rom";
static char	tapedir[MAX_PATH_BUFF]=".";
static int	fullscreen=FALSE;
static int	frames=50;
static int	scale=1;
static int	trace=0;
static int	sound=1;
static int	kempston_joystick=1;
static int	fuller_joystick=1;
static int	amx_mouse=0;
static int	kempston_mouse=0;
static int	zx_printer=0;

static const struct
{
    const char	*name;
    void	*var;
    int		is_int;
} config[]= {
		{"rompath",		rompath,		FALSE},
		{"tapedir",		tapedir,		FALSE},
		{"fullscreen",		&fullscreen,		TRUE},
		{"frames",		&frames,		TRUE},
		{"scale",		&scale,			TRUE},
		{"trace",		&trace,			TRUE},
		{"sound",		&sound,			TRUE},
		{"kempston_joystick",	&kempston_joystick,	TRUE},
		{"fuller_joystick",	&fuller_joystick,	TRUE},
		{"amx_mouse",		&amx_mouse,		TRUE},
		{"kempston_mouse",	&kempston_mouse,	TRUE},
		{"zx_printer",		&zx_printer,		TRUE},
		{NULL,			NULL,			FALSE}
	    };

static struct
{
    int		*ptr;
    int		min;
    int		max;
} iconfig[CONF_NUM_INT];

static char *sconfig[CONF_NUM_STRING];

static const struct
{
    IConfigVar	var;
    int		*ptr;
    int		min;
    int		max;
} iconfig_setup[] =
{
    {CONF_FULLSCREEN,		&fullscreen,		0,	1},
    {CONF_FRAMES_PER_SEC,	&frames,		1,	100},
    {CONF_SCALE,		&scale,			1,	10},
    {CONF_TRACE,		&trace,			0,	1},
    {CONF_SOUND,		&sound,			0,	63},
    {CONF_KEMPSTON_JOYSTICK,	&kempston_joystick,	0,	1},
    {CONF_FULLER_JOYSTICK,	&fuller_joystick,	0,	1},
    {CONF_AMX_MOUSE,		&amx_mouse,		0,	1},
    {CONF_KEMPSTON_MOUSE,	&kempston_mouse,	0,	1},
    {CONF_ZX_PRINTER,		&zx_printer,		0,	1},
    {0,				NULL,			0,	0}
};

static const struct
{
    SConfigVar	var;
    char	*ptr;
} sconfig_setup[] =
{
    {CONF_ROMFILE,	rompath},
    {CONF_TAPEDIR,	tapedir},
    {0,			NULL}
};

static const struct
{
    IConfigVar	var;
    const char	*description;
} iconfig_description [] =
{
    {CONF_FULLSCREEN,		"Full screen"},
    {CONF_FRAMES_PER_SEC,	"Frames per second"},
    {CONF_SCALE,		"Frames per second"},
    {CONF_TRACE,		"Frames per second"},
    {CONF_SOUND,		"Frames per second"},
    {CONF_KEMPSTON_JOYSTICK,	"Kempston Joystick"},
    {CONF_FULLER_JOYSTICK,	"Fuller Joystick"},
    {CONF_AMX_MOUSE,		"AMX Mouse"},
    {CONF_KEMPSTON_MOUSE,	"Kempston Mouse"},
    {CONF_ZX_PRINTER,		"ZX Printer"},
    {0,				NULL}
};

static const struct
{
    SConfigVar	var;
    const char	*description;
} sconfig_description [] =
{
    {CONF_ROMFILE,		"Path to ROM image"},
    {CONF_TAPEDIR,		"Initial tape directory"},
    {0,				NULL}
};


/* ---------------------------------------- PRIVATE FUNCTIONS
*/
static void SetupLookupTables(void)
{
    int f;

    for(f = 0; iconfig_setup[f].ptr; f++)
    {
    	iconfig[iconfig_setup[f].var].ptr = iconfig_setup[f].ptr;
    	iconfig[iconfig_setup[f].var].min = iconfig_setup[f].min;
    	iconfig[iconfig_setup[f].var].max = iconfig_setup[f].max;
    }

    for(f = 0; sconfig_setup[f].ptr; f++)
    {
    	sconfig[sconfig_setup[f].var] = sconfig_setup[f].ptr;
    }

    for(f = 0; f < CONF_NUM_INT; f++)
    {
    	if (!iconfig[f].ptr)
	{
	    Exit("Error in config int setup tables\n");
	}
    }

    for(f = 0; f < CONF_NUM_STRING; f++)
    {
    	if (!sconfig[f])
	{
	    Exit("Error in config string setup tables\n");
	}
    }
}

static void Parse(FILE *fp)
{
    char buff[1024];

    while(fgets(buff,sizeof buff,fp))
    {
    	size_t l;

	l=strlen(buff);

	if (buff[l-1]=='\n')
	    buff[--l]=0;

	if (l>0 && buff[0]!='#')
	{
	    char *t1=NULL;
	    char *t2=NULL;

	    t1=strtok(buff,"\t ");
	    t2=strtok(NULL,"\t ");

	    if (t2)
	    {
	    	int f;

		for(f=0;config[f].name;f++)
		{
		    if (strcmp(config[f].name,t1)==0)
		    {
			if (config[f].is_int)
			{
			    int *i;

			    i=config[f].var;
			    *i=atoi(t2);
			}
			else
			{
			    char *p;

			    p=config[f].var;
			    strcpy(p,t2);
			}
		    }
		}
	    }
	    else
	    {
	    	fprintf(stderr,"Ignored bad config: %s %s\n",t1,t2 ? t2:"");
	    }
	}
    }
}


/* ---------------------------------------- EXPORTED INTERFACES
*/
void ConfigRead(void)
{
    FILE *fp;
    const char *home = getenv("HOME");

    strcpy(config_path, home ? home : ".");
    strcat(config_path, "/.espec");

    if ((fp = fopen(config_path,"r")))
    {
    	Parse(fp);
	fclose(fp);
    }

    SetupLookupTables();
}


int IConfig(IConfigVar v)
{
    return Clamp(*iconfig[v].ptr, iconfig[v].min, iconfig[v].max);
}


const char *SConfig(SConfigVar v)
{
    return sconfig[v];
}


void IConfigSet(IConfigVar v, int value)
{
    *iconfig[v].ptr = value;
}


void SConfigSet(SConfigVar v, const char *value)
{
    SafeStrCopy(sconfig[v], value, MAX_PATH_BUFF);
}

const char *IConfigDescription(IConfigVar var)
{
    for(int f = 0; iconfig_description[f].description; f++)
    {
    	if (iconfig_description[f].var == var)
	{
	    return iconfig_description[f].description;
	}
    }

    return "Unknown";
}

const char *SConfigDescription(SConfigVar var)
{
    for(int f = 0; sconfig_description[f].description; f++)
    {
    	if (sconfig_description[f].var == var)
	{
	    return sconfig_description[f].description;
	}
    }

    return "Unknown";
}

void ConfigSave(void)
{
    FILE *fp;

    if ((fp = fopen(config_path, "w")))
    {
	for(int f = 0; config[f].name; f++)
	{
	    fprintf(fp, "%-20s", config[f].name);

	    if (config[f].is_int)
	    {
	    	int *p = config[f].var;

		fprintf(fp, "%d\n", *p);
	    }
	    else
	    {
	    	char *p = config[f].var;

		fprintf(fp, "%s\n", p);
	    }
	}

	fclose(fp);
    }
}
