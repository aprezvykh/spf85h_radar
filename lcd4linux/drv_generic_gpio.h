/* $Id: drv_generic_gpio.h 1051 2009-11-05 08:02:32Z peterbailey $
 * $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/drv_generic_gpio.h $
 *
 * generic driver helper for GPIO's
 *
 * Copyright (C) 2005 Michael Reinelt <michael@reinelt.co.at>
 * Copyright (C) 2005 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
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


#ifndef _DRV_GENERIC_GPO_H_
#define _DRV_GENERIC_GPO_H_

#include "widget.h"

extern int GPIS;		/* number of GPI's */
extern int GPOS;		/* number of GPO's */

/* these function must be implemented by the real driver */
extern int (*drv_generic_gpio_real_set) (const int num, const int val);
extern int (*drv_generic_gpio_real_get) (const int num);

/* generic functions and widget callbacks */
int drv_generic_gpio_init(const char *section, const char *driver);
int drv_generic_gpio_clear(void);
int drv_generic_gpio_get(const int num);
int drv_generic_gpio_draw(WIDGET * W);
int drv_generic_gpio_quit(void);

#endif
