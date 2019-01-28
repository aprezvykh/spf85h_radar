/* $Id: qprintf.c 918 2008-12-31 06:07:29Z michael $
 * $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/qprintf.c $
 *
 * simple but quick snprintf() replacement
 *
 * Copyright (C) 2004 Michael Reinelt <michael@reinelt.co.at>
 * Copyright (C) 2004 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
 *
 * derived from a patch from Martin Hejl which is
 * Copyright (C) 2003 Martin Hejl (martin@hejl.de)
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
 * int qprintf(char *str, size_t size, const char *format, ...)
 *   works like snprintf(), but format only knows about %d, %x, %u and %s
 *     and for the numbers an optional length like %<len>d. If <len> beginns
 *     with '0' the free space is filled with '0's, otherwise with ' '
 */


#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static char *itoa(char *buffer, const size_t size, int value, unsigned int fixedlen, unsigned int fill0)
{
    char *p;
    int sign;

    /* sanity checks */
    if (buffer == NULL || size < 2)
	return (NULL);

    /* remember sign of value */
    sign = 0;
    if (value < 0) {
	sign = 1;
	if (fill0)
	    fixedlen -= 1;
	value = -value;
    }

    /* p points to last char */
    p = buffer + size - 1;

    /* set terminating zero */
    *p = '\0';

    do {
	*--p = value % 10 + '0';
	value = value / 10;
    } while (value != 0 && p > buffer);

    if (sign && !fill0 && p > buffer)
	*--p = '-';

    /* fill fixed length */
    while (p > buffer && strlen(p) < fixedlen) {
	if (fill0) {
	    *--p = '0';
	} else {
	    *--p = ' ';
	}
    }

    if (sign && fill0 && p > buffer)
	*--p = '-';

    return p;
}


static char *utoa(char *buffer, const size_t size, unsigned int value, unsigned int fixedlen, unsigned int fill0)
{
    char *p;

    /* sanity checks */
    if (buffer == NULL || size < 2)
	return (NULL);

    /* p points to last char */
    p = buffer + size - 1;

    /* set terminating zero */
    *p = '\0';

    do {
	*--p = value % 10 + '0';
	value = value / 10;
    } while (value != 0 && p > buffer);

    /* fill fixed length */
    while (p > buffer && strlen(p) < fixedlen) {
	if (fill0) {
	    *--p = '0';
	} else {
	    *--p = ' ';
	}
    }

    return p;
}


static char *utox(char *buffer, const size_t size, unsigned int value, unsigned int fixedlen, unsigned int fill0)
{
    char *p;
    int digit;

    /* sanity checks */
    if (buffer == NULL || size < 2)
	return (NULL);

    /* p points to last char */
    p = buffer + size - 1;

    /* set terminating zero */
    *p = '\0';

    do {
	digit = value % 16;
	value = value / 16;
	*--p = (digit < 10 ? '0' : 'a' - 10) + digit;
    } while (value != 0 && p > buffer);

    /* fill fixed length */
    while (p > buffer && strlen(p) < fixedlen) {
	if (fill0) {
	    *--p = '0';
	} else {
	    *--p = ' ';
	}
    }

    return p;
}


/*!
    @function   qprintf
    @abstract   quick print values into string
    @discussion similar to snprintf(), but only support for "%s", "%d", "%u", "%x" with optional length for the numbers
                like "%5d" (filled with ' ') or "%05x" (filled with '0')
    @param      str  destination
    @param      size  maximum length of destination string
    @param      format  (like printf() with reduced number of formats)
    @result     length of produced string
*/
int qprintf(char *str, const size_t size, const char *format, ...)
{

    va_list ap;
    const char *src;
    char *dst;
    unsigned int len;

    src = format;
    dst = str;
    len = 0;

    va_start(ap, format);

    /* use size-1 for terminating zero */
    while (len < size - 1) {

	if (*src == '%') {
	    char buf[12], *s;
	    int d;
	    unsigned int u;
	    unsigned int fixedlen = 0;
	    unsigned int fill0 = 0;

	    if (*++src == '0')
		fill0 = 1;
	    while (*src >= '0' && *src <= '9') {
		fixedlen = fixedlen * 10 + (*src - '0');
		src++;
	    }

	    switch (*src) {
	    case 's':
		src++;
		s = va_arg(ap, char *);
		while (len < size - 1 && *s != '\0') {
		    len++;
		    *dst++ = *s++;
		}
		break;
	    case 'd':
		src++;
		d = va_arg(ap, int);
		s = itoa(buf, sizeof(buf), d, fixedlen, fill0);
		while (len < size && *s != '\0') {
		    len++;
		    *dst++ = *s++;
		}
		break;
	    case 'u':
		src++;
		u = va_arg(ap, unsigned int);
		s = utoa(buf, sizeof(buf), u, fixedlen, fill0);
		while (len < size - 1 && *s != '\0') {
		    len++;
		    *dst++ = *s++;
		}
		break;
	    case 'x':
		src++;
		u = va_arg(ap, unsigned int);
		s = utox(buf, sizeof(buf), u, fixedlen, fill0);
		while (len < size - 1 && *s != '\0') {
		    len++;
		    *dst++ = *s++;
		}
		break;
	    default:
		len++;
		*dst++ = '%';
	    }
	} else {
	    len++;
	    *dst++ = *src;
	    if (*src++ == '\0')
		break;
	}
    }

    va_end(ap);

    /* enforce terminating zero */
    if (len >= size - 1 && *(dst - 1) != '\0') {
	len++;
	*dst = '\0';
    }

    /* do not count terminating zero */
    return len - 1;
}
