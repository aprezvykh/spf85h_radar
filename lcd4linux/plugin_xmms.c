/* $Id: plugin_xmms.c 728 2007-01-14 11:14:38Z michael $
 * $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/plugin_xmms.c $
 *
 * XMMS-Plugin for LCD4Linux
 * Copyright (C) 2003 Markus Keil <markus_keil@t-online.de>
 *
 * This file is part of LCD4Linux.
 *
 * LCD4Linux is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * LCD4Linux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/* 
 * exported functions:
 *
 * int plugin_init_xmms (void)
 *  adds parser for /tmp/xmms-info
 *
 */


/*
 * The Argument 'arg1' must be one of these Things (without brackets):
 *
 * 'Title' - The title of the current song
 * 'Status' - The status of XMMS (playing, pause, ...)
 * 'Tunes in playlist' - How many entries are in the playlist
 * 'Currently playing' - which playlist-entry is playing
 * 'uSecPosition' - The position of the title in seconds (usefull for bargraphs ;-) )
 * 'Position' - The position of the title in mm:ss
 * 'uSecTime' - The length of the current title in seconds
 * 'Time' - The length of the current title in mm:ss
 * 'Current bitrate' - The current bitrate in bit
 * 'Samping Frequency' - The current samplingfreqency in Hz
 * 'Channels' - The current number of audiochannels
 * 'File' - The full path of the current file
 * 
 * These arguments are case-sensitive
 */


#include "config.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "hash.h"
#include "debug.h"
#include "plugin.h"


static HASH xmms;


static int parse_xmms_info(void)
{
    int age;
    FILE *xmms_stream;
    char zeile[200];

    /* reread every 100msec only */
    age = hash_age(&xmms, NULL);
    if (age >= 0 && age <= 200)
	return 0;
    /* Open Filestream for '/tmp/xmms-info' */
    xmms_stream = fopen("/tmp/xmms-info", "r");

    /* Check for File */
    if (!xmms_stream) {
	error("Error: Cannot open XMMS-Info Stream! Is XMMS started?");
	return -1;
    }

    /* Read Lines from the Stream */
    while (fgets(zeile, sizeof(zeile), xmms_stream)) {
	char *c, *key, *val;
	c = strchr(zeile, ':');
	if (c == NULL)
	    continue;
	key = zeile;
	val = c + 1;
	/* strip leading blanks from key */
	while (isspace(*key))
	    *key++ = '\0';
	/* strip trailing blanks from key */
	do
	    *c = '\0';
	while (isspace(*--c));
	/* strip leading blanks from value */
	while (isspace(*val))
	    *val++ = '\0';
	/* strip trailing blanks from value */
	for (c = val; *c != '\0'; c++);
	while (isspace(*--c))
	    *c = '\0';
	hash_put(&xmms, key, val);
    }

    fclose(xmms_stream);
    return 0;

}

static void my_xmms(RESULT * result, RESULT * arg1)
{
    char *key, *val;

    if (parse_xmms_info() < 0) {
	SetResult(&result, R_STRING, "");
	return;
    }

    key = R2S(arg1);
    val = hash_get(&xmms, key, NULL);
    if (val == NULL)
	val = "";

    SetResult(&result, R_STRING, val);
}


int plugin_init_xmms(void)
{
    hash_create(&xmms);

    /* register xmms info */
    AddFunction("xmms", 1, my_xmms);

    return 0;
}

void plugin_exit_xmms(void)
{
    hash_destroy(&xmms);
}
