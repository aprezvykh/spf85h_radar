/* $Id: drv_generic_keypad.h 856 2008-03-03 16:54:18Z michux $
 * $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/drv_generic_keypad.h $
 *
 * generic driver helper for keypads
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

#ifndef _DRV_GENERIC_KEYPAD_H_
#define _DRV_GENERIC_KEYPAD_H_

#include "widget.h"

/* these functions must be implemented by the real driver */
extern int (*drv_generic_keypad_real_press) (const int num);

/* generic functions and widget callbacks */
int drv_generic_keypad_init(const char *section, const char *driver);
int drv_generic_keypad_press(const int num);
int drv_generic_keypad_quit(void);

#endif
