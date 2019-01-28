/* $Id: thread.h 1010 2009-03-27 13:13:28Z michux $
 * $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/thread.h $
 *
 * thread handling (mutex, shmem, ...)
 *
 * Copyright (C) 2004 Michael Reinelt <michael@reinelt.co.at>
 * Copyright (C) 2004 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
 *
 * parts of this code are based on the old XWindow driver which is
 * Copyright (C) 2000 Herbert Rosmanith <herp@wildsau.idv.uni-linz.ac.at>
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

#ifndef _THREAD_H_
#define _THREAD_H_

#ifdef __CYGWIN__

#ifndef HAVE_UNION_SEMUN
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
#endif

#ifndef SHM_R
#define SHM_R 0400
#endif
#ifndef SHM_W
#define SHM_W 0660
#endif

#endif

extern int thread_argc;
extern char **thread_argv;

int mutex_create(void);
void mutex_lock(const int semid);
void mutex_unlock(const int semid);
void mutex_destroy(const int semid);

int shm_create(void **buffer, const int size);
void shm_destroy(const int shmid, const void *buffer);

int thread_create(const char *name, void (*thread) (void *data), void *data);
int thread_destroy(const int pid);

#endif
