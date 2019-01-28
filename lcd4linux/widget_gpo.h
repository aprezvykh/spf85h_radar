/* $Id: widget_gpo.h 1164 2011-12-22 10:48:01Z mjona $
 * $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/widget_gpo.h $
 *
 * GPO widget handling
 *
 * Copyright (C) 2005 Michael Reinelt <michael@reinelt.co.at>
 * Copyright (C) 2005, 2006, 2007 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
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


#ifndef _WIDGET_GPO_H_
#define _WIDGET_GPO_H_

#include "property.h"
#include "widget.h"

typedef struct WIDGET_GPO {
    PROPERTY expression;	/* main GPO expression */
    PROPERTY update;		/* update interval (msec) */
} WIDGET_GPO;


extern WIDGET_CLASS Widget_GPO;

#endif
