/* $Id: plugin_asterisk.c 1153 2011-07-27 05:12:30Z michael $
 * $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/plugin_asterisk.c $
 *
 * plugin for asterisk
 *
 * Copyright (C) 2003 Michael Reinelt <michael@reinelt.co.at>
 * Copyright (C) 2004, 2005, 2006, 2007 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
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
 * int plugin_init_sample (void)
 *  adds various functions
 *
 */


/* define the include files you need */
#include "config.h"
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "debug.h"
#include "plugin.h"

#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif

struct Line {
    char Channel[25];		/* Zap Channel */
    char EndPoint[25];
    unsigned char active;
};

static char *rtrim(char *string, char junk)
{
    char *original = string + strlen(string);
    while (*--original == junk);
    *(original + 1) = '\0';

    return string;
}

static void zapstatus(RESULT * result, RESULT * arg1)
{
    FILE *infile;
    int skipline = 0;		// Skip the first in the file, it throws off the detection
    char line[100], *SipLoc, Channel[25], Location[25], __attribute__ ((unused)) State[9], Application[25], EndPoint[8],
	Ret[50];
    int i = 0, ChannelInt = 0, ZapLine = 0;
    struct Line Lines[32];	// Setup 32 lines, ZAP 1-32 (memory is cheap)

    ZapLine = R2N(arg1);

    // Set all the lines status's default to inactive
    for (i = 0; i < 32; i++) {
	strcpy(Lines[i].Channel, "ZAP/");
	Lines[i].Channel[4] = (char) (i + 49);
	Lines[i].Channel[5] = '\0';
	Lines[i].active = 0;
    }

    system("touch /tmp/asterisk.state");	// Touch the file in it's naughty place
    system("chmod 744 /tmp/asterisk.state");
    system("asterisk -rx \"show channels\" > /tmp/asterisk.state");	// Crappy CLI way to do it

    infile = fopen("/tmp/asterisk.state", "r");

    for (i = 0; i < 100; i++) {
	line[i] = ' ';
    }
    line[99] = '\0';

    while (fgets(line, 100, infile) != NULL) {
	if (strstr(line, "Zap") != NULL) {
	    for (i = 0; i < (int) strlen(line); i++) {
		if (i < 20) {
		    Channel[i] = line[i];
		} else if (i < 42) {
		    Location[i - 21] = line[i];
		} else if (i < 50) {
		    State[i - 42] = line[i];
		} else {
		    Application[i - 50] = line[i];
		}
	    }
	    strncpy(Channel, Channel, 7);
	    Channel[7] = '\0';
	    strcpy(Location, rtrim(Location, ' '));
	    State[4] = '\0';
	    memcpy(EndPoint, Application + 13, 7);
	    EndPoint[7] = '\0';

	    if (strstr(Application, "Bridged Call") != NULL) {
		// Subtract 48 from the character value to get the int
		// value. Subtract one more because arrays start at 0.
		ChannelInt = (int) (Channel[4]) - 49;
		strcpy(Lines[ChannelInt].Channel, Channel);
		strncpy(Lines[ChannelInt].EndPoint, EndPoint, 8);
		Lines[ChannelInt].active = 1;
	    } else {
		SipLoc = strstr(Application, "SIP");
		if (SipLoc != NULL) {
		    strncpy(EndPoint, SipLoc, 7);
		} else {
		    EndPoint[0] = '\0';
		}
		ChannelInt = (int) (Channel[4]) - 49;
		strcpy(Lines[ChannelInt].Channel, Channel);
		Lines[ChannelInt].active = 1;
	    }
	} else {
	    if (strlen(line) > 54 && skipline > 1) {
		for (i = 55; i < 88; i++) {
		    if (i < 80) {
			Channel[i - 55] = line[i];
		    } else {
			EndPoint[i - 80] = line[i];
		    }
		}
		strncpy(Channel, rtrim(Channel, ' '), 5);
		strncpy(EndPoint, rtrim(EndPoint, ' '), 7);

		ChannelInt = (int) (Channel[4]) - 49;
		strcpy(Lines[ChannelInt].Channel, Channel);
		strcpy(Lines[ChannelInt].EndPoint, EndPoint);
		Lines[ChannelInt].active = 1;
	    }
	}
	skipline += 1;
    }
    fclose(infile);

    ZapLine -= 1;
    if (ZapLine < 0 || ZapLine > 31) {
	memset(Ret, ' ', 50);
	Ret[0] = '\0';
	strcat(Ret, "Invalid ZAP #");
	SetResult(&result, R_STRING, &Ret);
    } else if (Lines[ZapLine].active == 1) {
	memset(Ret, ' ', 50);
	Ret[0] = '\0';
	strcat(Ret, Lines[ZapLine].Channel);
	strcat(Ret, " -> ");
	strncat(Ret, Lines[ZapLine].EndPoint, 8);
	SetResult(&result, R_STRING, &Ret);
    } else {
	memset(Ret, ' ', 50);
	Ret[0] = '\0';
	strcat(Ret, Lines[ZapLine].Channel);
	strcat(Ret, ": inactive");
	SetResult(&result, R_STRING, &Ret);
    }
    return;
}

static void corecalls(RESULT * result)
{
    FILE *infile;
    char line[100];
    int calls;

    system("asterisk -rx 'core show channels' > /tmp/asterisk.calls");

    infile = fopen("/tmp/asterisk.state", "r");
    line[0] = '\0';
    while (fgets(line, 100, infile) != NULL) {
	if (strstr(line, "active calls") != NULL) {
	    break;
	}
    }
    fclose(infile);

    if (line[0] != '\0') {
	sscanf(line, "%d active calls", &calls);
    } else {
	calls = 0;
    }

    SetResult(&result, R_NUMBER, &calls);
    return;
}

int sipinfo(int type)
{
    FILE *infile;
    char line[100];
    int peers, online;

    system("asterisk -rx 'sip show peers' > /tmp/asterisk.sip");

    infile = fopen("/tmp/asterisk.sip", "r");
    line[0] = '\0';
    while (fgets(line, 100, infile) != NULL) {
    }				// Get the last line
    fclose(infile);

    if (line[0] != '\0') {
	sscanf(line, "%d sip peers [Monitored: %d online,", &peers, &online);
    } else {
	peers = 0;
	online = 0;
    }

    return (type == 1) ? peers : online;
}

static void sippeers(RESULT * result)
{
    int peers;
    peers = sipinfo(1);
    SetResult(&result, R_NUMBER, &peers);
    return;
}

static void siponline(RESULT * result)
{
    int online;
    online = sipinfo(2);
    SetResult(&result, R_NUMBER, &online);
    return;
}

static void uptime(RESULT * result)
{
    FILE *infile;
    char line[100], *tok;
    int fields[5], toknum = 0, num, s = 0;

    fields[0] = fields[1] = fields[2] = fields[3] = fields[4] = 0;

    system("asterisk -rx 'core show uptime' > /tmp/asterisk.uptime");

    infile = fopen("/tmp/asterisk.uptime", "r");
    line[0] = '\0';
    while (fgets(line, 100, infile) != NULL) {
	if (strstr(line, "System uptime") != NULL) {
	    break;
	}
    }
    fclose(infile);

    if (line[0] != '\0') {
	for (tok = strtok(line, " "); tok != NULL; tok = strtok(NULL, " ")) {
	    toknum++;
	    if (toknum == 3 || toknum == 5 || toknum == 7 || toknum == 9 || toknum == 11) {
		sscanf(tok, "%d", &num);
	    } else if (toknum == 4 || toknum == 6 || toknum == 8 || toknum == 10 || toknum == 12) {
		if (strstr(tok, "weeks") != NULL || (strstr(tok, "week") != NULL)) {
		    fields[4] = num;
		} else if (strstr(tok, "days") != NULL || (strstr(tok, "day") != NULL)) {
		    fields[3] = num;
		} else if (strstr(tok, "hours") != NULL || (strstr(tok, "hour") != NULL)) {
		    fields[2] = num;
		} else if (strstr(tok, "minutes") != NULL || (strstr(tok, "minute") != NULL)) {
		    fields[1] = num;
		} else {
		    fields[0] = num;
		}
	    }
	}
	s = (fields[4] * 604800) + (fields[3] * 86400) + (fields[2] * 3600) + (fields[1] * 60) + fields[0];
    }

    SetResult(&result, R_NUMBER, &s);
    return;
}

int plugin_init_asterisk(void)
{
    AddFunction("asterisk::zapstatus", 1, zapstatus);
    AddFunction("asterisk::corecalls", 0, corecalls);
    AddFunction("asterisk::sippeers", 0, sippeers);
    AddFunction("asterisk::siponline", 0, siponline);
    AddFunction("asterisk::uptime", 0, uptime);
    return 0;
}

void plugin_exit_asterisk(void)
{
    /* free any allocated memory */
    /* close filedescriptors */
}
