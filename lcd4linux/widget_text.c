/* $Id: widget_text.c 1199 2013-05-23 03:07:28Z michael $
 * $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/widget_text.c $
 *
 * simple text widget handling
 *
 * Copyright (C) 2003, 2004 Michael Reinelt <michael@reinelt.co.at>
 * Copyright (C) 2004 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
 * Copyright (C) 2008 Michael Vogt <michu@neophob.com>
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
 * WIDGET_CLASS Widget_Text
 *   a simple text widget which
 *   must be supported by all displays
 *
 */


#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "debug.h"
#include "cfg.h"
#include "evaluator.h"
#include "property.h"
#include "timer.h"
#include "timer_group.h"
#include "event.h"
#include "widget.h"
#include "widget_text.h"

#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif


void widget_text_scroll(void *Self)
{
    WIDGET *W = (WIDGET *) Self;
    if (NULL == W || NULL == W->data) {
	error("Warning: internal data error in Textwidget");
	return;
    }
    WIDGET_TEXT *T = W->data;

    char *prefix = P2S(&T->prefix);
    char *postfix = P2S(&T->postfix);

    char *string = T->string;

    int num, len, width, pad;
    char *src, *dst;

    if (NULL == string) {
	error("Warning: Widget %s has no string", W->name);
	return;
    }
    num = 0;
    len = strlen(string);
    width = T->width - strlen(prefix) - strlen(postfix);
    if (width < 0)
	width = 0;

    switch (T->align) {
    case ALIGN_LEFT:
	pad = 0;
	break;
    case ALIGN_CENTER:
	pad = (width - len) / 2;
	if (pad < 0)
	    pad = 0;
	break;
    case ALIGN_RIGHT:
	pad = width - len;
	if (pad < 0)
	    pad = 0;
	break;
    case ALIGN_AUTOMATIC:
	if (len <= width) {
	    pad = 0;
	    break;
	}
    case ALIGN_MARQUEE:
	pad = width - T->scroll;
	T->scroll++;
	if (T->scroll >= width + len)
	    T->scroll = 0;
	break;
    case ALIGN_PINGPONG_LEFT:
    case ALIGN_PINGPONG_CENTER:
    case ALIGN_PINGPONG_RIGHT:
#define PINGPONGWAIT 2

	/* scrolling is not necessary - align the string */
	if (len <= width) {
	    switch (T->align) {
	    case ALIGN_PINGPONG_LEFT:
		pad = 0;
		break;
	    case ALIGN_PINGPONG_RIGHT:
		pad = width - len;
		if (pad < 0)
		    pad = 0;
		break;
	    default:
		pad = (width - len) / 2;
		if (pad < 0)
		    pad = 0;
		break;
	    }
	} else {
	    if (T->direction == 1)
		T->scroll++;	/* scroll right */
	    else
		T->scroll--;	/* scroll left */

	    /*pad = if positive, add leading space characters, else offset of string begin */
	    pad = 0 - T->scroll;

	    if (pad < 0 - (len - width)) {
		if (T->delay-- < 1) {	/* wait before switch direction */
		    T->direction = 0;	/* change scroll direction */
		    T->delay = PINGPONGWAIT;
		    T->scroll -= PINGPONGWAIT;
		}		/* else debug("wait1"); */
		pad = 0 - (len - width);
	    } else if (pad > 0) {
		if (T->delay-- < 1) {
		    T->direction = 1;
		    T->delay = PINGPONGWAIT;
		    T->scroll += PINGPONGWAIT;
		}		/* else debug("wait2"); */
		pad = 0;
	    }

	}
	break;
    default:			/* not reached  */
	pad = 0;
    }

    dst = T->buffer;

    /* process prefix */
    src = prefix;
    while (num < T->width) {
	if (*src == '\0')
	    break;
	*(dst++) = *(src++);
	num++;
    }

    src = string;

    /* pad blanks on the beginning */
    while (pad > 0 && num < T->width) {
	*(dst++) = ' ';
	num++;
	pad--;
    }

    /* skip src chars (marquee) */
    while (pad < 0) {
	src++;
	pad++;
    }

    /* copy content */
    while (num < T->width) {
	if (*src == '\0')
	    break;
	*(dst++) = *(src++);
	num++;
    }

    /* pad blanks on the end */
    src = postfix;
    len = strlen(src);
    while (num < T->width - len) {
	*(dst++) = ' ';
	num++;
    }

    /* process postfix */
    while (num < T->width) {
	if (*src == '\0')
	    break;
	*(dst++) = *(src++);
	num++;
    }

    *dst = '\0';

    /* finally, draw it! */
    if (W->class->draw)
	W->class->draw(W);
}



void widget_text_update(void *Self)
{
    WIDGET *W = (WIDGET *) Self;
    WIDGET_TEXT *T = W->data;
    char *string;
    int update = 0;

    /* evaluate properties */
    update += property_eval(&T->prefix);
    update += property_eval(&T->postfix);
    update += property_eval(&T->style);

    /* evaluate value */
    property_eval(&T->value);

    /* string or number? */
    if (T->precision == 0xDEAD) {
	string = strdup(P2S(&T->value));
    } else {
	double number = P2N(&T->value);
	int width = T->width - strlen(P2S(&T->prefix)) - strlen(P2S(&T->postfix));
	int precision = T->precision;
	/* print zero bytes so we can specify NULL as target  */
	/* and get the length of the resulting string */
	int size = snprintf(NULL, 0, "%.*f", precision, number);
	/* number does not fit into field width: try to reduce precision */
	if (width < 0)
	    width = 0;
	if (size > width && precision > 0) {
	    int delta = size - width;
	    if (delta > precision)
		delta = precision;
	    precision -= delta;
	    size -= delta;
	    /* zero precision: omit decimal point, too */
	    if (precision == 0)
		size--;
	}
	/* number still doesn't fit: display '*****'  */
	if (size > width) {
	    string = malloc(width + 1);
	    memset(string, '*', width);
	    *(string + width) = '\0';
	} else {
	    string = malloc(size + 1);
	    snprintf(string, size + 1, "%.*f", precision, number);
	}
    }

    /* did the formatted string change? */
    if (T->string == NULL || strcmp(T->string, string) != 0) {
	update++;
	free(T->string);
	T->string = string;
    } else {
	free(string);
    }

    /* something has changed and should be updated */
    if (update) {
	/* reset marquee counter if content has changed */
	T->scroll = 0;

	/* Init pingpong scroller. start scrolling left (wrong way) to get a delay */
	if (T->align == ALIGN_PINGPONG_LEFT || T->align == ALIGN_PINGPONG_CENTER || T->align == ALIGN_PINGPONG_RIGHT) {
	    T->direction = 0;
	    T->delay = PINGPONGWAIT;
	}
	/* if there's a marquee scroller active, it has its own */
	/* update callback timer, so we do nothing here; otherwise */
	/* we simply call this scroll callback directly */
	if (T->align != ALIGN_MARQUEE || T->align != ALIGN_AUTOMATIC || T->align != ALIGN_PINGPONG_LEFT
	    || T->align != ALIGN_PINGPONG_CENTER || T->align != ALIGN_PINGPONG_RIGHT) {
	    widget_text_scroll(Self);
	}
    }

}


int widget_text_init(WIDGET * Self)
{
    char *section;
    char *c;
    WIDGET_TEXT *Text;

    /* prepare config section */
    /* strlen("Widget:")=7 */
    section = malloc(strlen(Self->name) + 8);
    strcpy(section, "Widget:");
    strcat(section, Self->name);

    Text = malloc(sizeof(WIDGET_TEXT));
    memset(Text, 0, sizeof(WIDGET_TEXT));

    /* load properties */
    property_load(section, "prefix", NULL, &Text->prefix);
    property_load(section, "expression", "", &Text->value);
    property_load(section, "postfix", NULL, &Text->postfix);
    property_load(section, "style", NULL, &Text->style);

    /* sanity checks */
    if (!property_valid(&Text->value)) {
	error("Warning: widget %s has no expression", section);
    }

    /* field width, default 10 */
    cfg_number(section, "width", 10, 0, -1, &(Text->width));

    /* precision: number of digits after the decimal point (default: none) */
    /* Note: this is the *maximum* precision on small values, */
    /* for larger values the precision may be reduced to fit into the field width. */
    /* The default value 0xDEAD is used to distinguish between numbers and strings: */
    /* if no precision is given, the result is always treated as a string. If a */
    /* precision is specified, the result is treated as a number. */
    cfg_number(section, "precision", 0xDEAD, 0, 80, &(Text->precision));

    /* field alignment: Left (default), Center, Right or Marquee */
    c = cfg_get(section, "align", "L");
    switch (toupper(c[0])) {
    case 'L':
	Text->align = ALIGN_LEFT;
	break;
    case 'C':
	Text->align = ALIGN_CENTER;
	break;
    case 'R':
	Text->align = ALIGN_RIGHT;
	break;
    case 'M':
	Text->align = ALIGN_MARQUEE;
	break;
    case 'A':
	Text->align = ALIGN_AUTOMATIC;
	break;
    case 'P':
	switch (toupper(c[1])) {
	case 'C':
	    Text->align = ALIGN_PINGPONG_CENTER;
	    break;
	case 'L':
	    Text->align = ALIGN_PINGPONG_LEFT;
	    break;
	case 'R':
	    Text->align = ALIGN_PINGPONG_RIGHT;
	    break;
	default:
	    Text->align = ALIGN_PINGPONG_CENTER;
	    error("widget %s has unknown alignment '%s', using 'Centered Pingpong'", section, c);
	}
	break;
    default:
	error("widget %s has unknown alignment '%s', using 'Left'", section, c);
	Text->align = ALIGN_LEFT;
    }
    free(c);

    /* update interval (msec), default 1 sec, 0 stands for never */
    cfg_number(section, "update", 1000, 0, -1, &(Text->update));
    /* limit update interval to min 10 msec */
    if (Text->update > 0 && Text->update < 10)
	Text->update = 10;

    /* marquee scroller speed: interval (msec), default 500msec */
    if (Text->align == ALIGN_MARQUEE || Text->align == ALIGN_AUTOMATIC || Text->align == ALIGN_PINGPONG_LEFT
	|| Text->align == ALIGN_PINGPONG_CENTER || Text->align == ALIGN_PINGPONG_RIGHT) {
	cfg_number(section, "speed", 500, 10, -1, &(Text->speed));
    }
    //update on this event
    char *event_name = cfg_get(section, "event", "");
    if (*event_name != '\0') {
	named_event_add(event_name, widget_text_update, Self);
	if (Text->update == 1000) {
	    Text->update = 0;
	}
    }
    free(event_name);


    /* buffer */
    Text->buffer = malloc(Text->width + 1);

    free(section);
    Self->data = Text;
    Self->x2 = Self->col + Text->width;
    Self->y2 = Self->row;

    /* add update timer, use one-shot if 'update' is zero */
    timer_add_widget(widget_text_update, Self, Text->update, Text->update == 0);

    /* a marquee scroller has its own timer and callback */
    if (Text->align == ALIGN_MARQUEE || Text->align == ALIGN_AUTOMATIC || Text->align == ALIGN_PINGPONG_LEFT
	|| Text->align == ALIGN_PINGPONG_CENTER || Text->align == ALIGN_PINGPONG_RIGHT) {
	timer_add(widget_text_scroll, Self, Text->speed, 0);
    }

    return 0;
}


int widget_text_quit(WIDGET * Self)
{
    WIDGET_TEXT *Text;
    if (Self) {
	Text = Self->data;
	if (Self->data) {
	    property_free(&Text->prefix);
	    property_free(&Text->value);
	    property_free(&Text->postfix);
	    property_free(&Text->style);
	    free(Text->string);
	    free(Text->buffer);
	    free(Self->data);
	    Self->data = NULL;
	}

    }
    return 0;

}



WIDGET_CLASS Widget_Text = {
    .name = "text",
    .type = WIDGET_TYPE_RC,
    .init = widget_text_init,
    .draw = NULL,
    .quit = widget_text_quit,
};
