/* $Id: widget_keypad.c 996 2009-03-23 17:22:24Z volker $
 * $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/widget_keypad.c $
 *
 * keypad widget handling
 *
 * Copyright (C) 2006 Chris Maj <cmaj@freedomcorpse.com>
 * Copyright (C) 2006 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
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
 * WIDGET_CLASS Widget_Keypad
 *   the keypad widget
 *
 */


#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "debug.h"
#include "cfg.h"
#include "property.h"
#include "timer.h"
#include "widget.h"
#include "widget_keypad.h"

#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif


int widget_keypad_draw(WIDGET * Self)
{
    WIDGET_KEYPAD *keypad = Self->data;

    /* evaluate properties */
    property_eval(&keypad->expression);

    return P2N(&keypad->expression);
}


int widget_keypad_init(WIDGET * Self)
{
    char *section;
    char *c;
    WIDGET_KEYPAD *keypad;

    /* prepare config section */
    /* strlen("Widget:")=7 */
    section = malloc(strlen(Self->name) + 8);
    strcpy(section, "Widget:");
    strcat(section, Self->name);

    keypad = malloc(sizeof(WIDGET_KEYPAD));
    memset(keypad, 0, sizeof(WIDGET_KEYPAD));

    /* load properties */
    property_load(section, "expression", NULL, &keypad->expression);

    /* state: pressed (default), released */
    c = cfg_get(section, "state", "pressed");
    if (!strcasecmp(c, "released"))
	keypad->key = WIDGET_KEY_RELEASED;
    else
	keypad->key = WIDGET_KEY_PRESSED;

    /* position: confirm (default), up, down, left, right, cancel */
    c = cfg_get(section, "position", "confirm");
    if (!strcasecmp(c, "up"))
	keypad->key += WIDGET_KEY_UP;
    else if (!strcasecmp(c, "down"))
	keypad->key += WIDGET_KEY_DOWN;
    else if (!strcasecmp(c, "left"))
	keypad->key += WIDGET_KEY_LEFT;
    else if (!strcasecmp(c, "right"))
	keypad->key += WIDGET_KEY_RIGHT;
    else if (!strcasecmp(c, "cancel"))
	keypad->key += WIDGET_KEY_CANCEL;
    else
	keypad->key += WIDGET_KEY_CONFIRM;

    free(section);
    Self->data = keypad;
    Self->x2 = NOCOORD;
    Self->y2 = NOCOORD;

    return 0;
}

int widget_keypad_find(WIDGET * Self, void *needle)
{
    WIDGET_KEYPAD *keypad;
    KEYPADKEY key = *(KEYPADKEY *) needle;

    if (Self && Self->data) {
	keypad = Self->data;
	if (keypad->key == key)
	    return 0;
    }

    return -1;
}

int widget_keypad_quit(WIDGET * Self)
{
    if (Self && Self->data) {
	WIDGET_KEYPAD *keypad = Self->data;
	property_free(&keypad->expression);
	free(Self->data);
	Self->data = NULL;
    }
    return 0;
}



WIDGET_CLASS Widget_Keypad = {
    .name = "keypad",
    .type = WIDGET_TYPE_KEYPAD,
    .init = widget_keypad_init,
    .draw = widget_keypad_draw,
    .find = widget_keypad_find,
    .quit = widget_keypad_quit,
};
