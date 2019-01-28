/* $Id: qprintf.h 840 2007-09-09 12:17:42Z michael $
 * $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/qprintf.h $
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

#ifndef _QPRINTF_H_
#define _QPRINTF_H_

/* size_t */
#include <stdio.h>

int qprintf(char *str, size_t size, const char *format, ...);

#endif
