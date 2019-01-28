/* $Id: drv_MilfordInstruments.c 840 2007-09-09 12:17:42Z michael $
 * $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/drv_MilfordInstruments.c $
 *
 * driver for Milford Instruments 'BPK' piggy-back serial interface board
 * for standard Hitachi 44780 compatible lcd modules.
 *
 * Copyright (C) 2003, 2004 Andy Baxter <andy@earthsong.free-online.co.uk>
 * Copyright (C) 2004 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
 *
 * based on the MatrixOrbital driver which is
 * Copyright (C) 1999, 2000 Michael Reinelt <michael@reinelt.co.at>
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
 *
 * exported fuctions:
 *
 * struct DRIVER drv_MilfordInstruments
 *
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "debug.h"
#include "cfg.h"
#include "plugin.h"
#include "widget.h"
#include "widget_text.h"
#include "widget_icon.h"
#include "widget_bar.h"
#include "drv.h"
#include "drv_generic_text.h"
#include "drv_generic_serial.h"


static char Name[] = "MilfordInstruments";

typedef struct {
    int type;
    char *name;
    int rows;
    int cols;
} MODEL;

static MODEL Models[] = {
    {216, "MI216", 2, 16},
    {220, "MI220", 2, 20},
    {240, "MI240", 2, 40},
    {420, "MI420", 4, 20},
    {-1, "unknown", -1, -1},
};

static int Model;


/****************************************/
/***  hardware dependant functions    ***/
/****************************************/

static void drv_MI_clear(void)
{
    drv_generic_serial_write("\376\001", 2);	/* clear screen */
}


static void drv_MI_write(const int row, const int col, const char *data, const int len)
{
    char cmd[2] = "\376x";
    int ddbase = 128;
    if (row & 1) {		/* i.e. if row is 1 or 3 */
	ddbase += 64;
    }
    if (row & 2) {		/* i.e. if row is 0 or 2. */
	ddbase += 20;
    }
    cmd[1] = (char) (ddbase + col);
    drv_generic_serial_write(cmd, 2);

    drv_generic_serial_write(data, len);
}


static void drv_MI_defchar(const int ascii, const unsigned char *matrix)
{
    int i;
    char cmd[10] = "\376x";

    if (ascii < 8) {
	cmd[1] = (char) (64 + ascii * 8);
	for (i = 0; i < 8; i++) {
	    cmd[i + 2] = matrix[i] & 0x1f;
	};
	drv_generic_serial_write(cmd, 10);
    }
}


static int drv_MI_start(const char *section, const int quiet)
{
    int i;
    char *model;

    model = cfg_get(section, "Model", NULL);
    if (model == NULL && *model == '\0') {
	error("%s: no '%s.Model' entry from %s", Name, section, cfg_source());
	return -1;
    }

    for (i = 0; Models[i].type != 0xff; i++) {
	if (strcasecmp(Models[i].name, model) == 0)
	    break;
    }
    if (Models[i].type == 0xff) {
	error("%s: %s.Model '%s' is unknown from %s", Name, section, model, cfg_source());
	return -1;
    }
    Model = i;
    info("%s: using model '%s'", Name, Models[Model].name);

    if (drv_generic_serial_open(section, Name, 0) < 0)
	return -1;

    /* initialize global variables */
    DROWS = Models[Model].rows;
    DCOLS = Models[Model].cols;

    drv_MI_clear();
    drv_generic_serial_write("\376\014", 2);	/* cursor off */

    if (!quiet) {
	if (drv_generic_text_greet(Models[Model].name, "Milford Instruments")) {
	    sleep(3);
	    drv_MI_clear();
	}
    }

    return 0;
}


/****************************************/
/***            plugins               ***/
/****************************************/

/* none at the moment... */


/****************************************/
/***        widget callbacks          ***/
/****************************************/

/* using drv_generic_text_draw(W) */
/* using drv_generic_text_icon_draw(W) */
/* using drv_generic_text_bar_draw(W) */


/****************************************/
/***        exported functions        ***/
/****************************************/


/* list models */
int drv_MI_list(void)
{
    int i;

    for (i = 0; Models[i].type > 0; i++) {
	printf("%s ", Models[i].name);
    }
    return 0;
}


/* initialize driver & display */
int drv_MI_init(const char *section, const int quiet)
{
    WIDGET_CLASS wc;
    int ret;

    info("%s: %s", Name, "$Rev: 840 $");

    /* display preferences */
    XRES = 5;			/* pixel width of one char  */
    YRES = 8;			/* pixel height of one char  */
    CHARS = 8;			/* number of user-defineable characters */
    CHAR0 = 0;			/* ASCII of first user-defineable char */
    GOTO_COST = 4;		/* number of bytes a goto command requires */

    /* real worker functions */
    drv_generic_text_real_write = drv_MI_write;
    drv_generic_text_real_defchar = drv_MI_defchar;


    /* start display */
    if ((ret = drv_MI_start(section, quiet)) != 0)
	return ret;

    /* initialize generic text driver */
    if ((ret = drv_generic_text_init(section, Name)) != 0)
	return ret;

    /* initialize generic icon driver */
    if ((ret = drv_generic_text_icon_init()) != 0)
	return ret;

    /* initialize generic bar driver */
    if ((ret = drv_generic_text_bar_init(0)) != 0)
	return ret;

    /* add fixed chars to the bar driver */
    drv_generic_text_bar_add_segment(0, 0, 255, 32);	/* ASCII  32 = blank */
    drv_generic_text_bar_add_segment(255, 255, 255, 255);	/* ASCII 255 = block */

    /* register text widget */
    wc = Widget_Text;
    wc.draw = drv_generic_text_draw;
    widget_register(&wc);

    /* register icon widget */
    wc = Widget_Icon;
    wc.draw = drv_generic_text_icon_draw;
    widget_register(&wc);

    /* register bar widget */
    wc = Widget_Bar;
    wc.draw = drv_generic_text_bar_draw;
    widget_register(&wc);

    /* register plugins */
    /* none at the moment... */

    return 0;
}


/* close driver & display */
int drv_MI_quit(const int quiet)
{

    info("%s: shutting down.", Name);

    drv_generic_text_quit();

    /* clear display */
    drv_MI_clear();

    /* say goodbye... */
    if (!quiet) {
	drv_generic_text_greet("goodbye!", NULL);
    }

    drv_generic_serial_close();

    return (0);
}


DRIVER drv_MilfordInstruments = {
    .name = Name,
    .list = drv_MI_list,
    .init = drv_MI_init,
    .quit = drv_MI_quit,
};
