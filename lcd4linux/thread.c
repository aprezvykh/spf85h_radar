/* $Id: thread.c 1153 2011-07-27 05:12:30Z michael $
 * $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/thread.c $
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

/* 
 * exported functions:
 * 
 * int  mutex_create  (void);
 *   creates a mutex and treturns its ID
 * 
 * void mutex_lock    (int semid);
 *   try to lock a mutex
 *
 * void mutex_unlock  (int semid);
 *   unlock a mutex
 *
 * void mutex_destroy (int semid);
 *   release a mutex
 *
 *
 * int shm_create    (void **buffer, int size);
 *   create shared memory segment
 *
 * void shm_destroy   (int shmid, void *buffer) ;
 *   release shared memory segment
 *
 * int thread_create (char *name, void (*thread)(void *data), void *data);
 *   create a new thread
 *
 */


#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "debug.h"
#include "thread.h"


#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif


int thread_argc;
char **thread_argv;


/* glibc 2.1 requires defining semun ourselves */
#ifdef _SEM_SEMUN_UNDEFINED
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};
#endif


int mutex_create(void)
{
    int semid;
    union semun semun;

    semid = semget(IPC_PRIVATE, 1, 0);
    if (semid == -1) {
	error("fatal error: semget() failed: %s", strerror(errno));
	return -1;
    }
    semun.val = 1;
    semctl(semid, 0, SETVAL, semun);

    return semid;
}


void mutex_lock(const int semid)
{
    struct sembuf sembuf;
    sembuf.sem_num = 0;
    sembuf.sem_op = -1;
    sembuf.sem_flg = 0;
    semop(semid, &sembuf, 1);
}


void mutex_unlock(const int semid)
{
    struct sembuf sembuf;
    sembuf.sem_num = 0;
    sembuf.sem_op = 1;
    sembuf.sem_flg = 0;
    semop(semid, &sembuf, 1);
}


void mutex_destroy(const int semid)
{
    union semun arg;
    semctl(semid, 0, IPC_RMID, arg);
}


int shm_create(void **buffer, const int size)
{
    int shmid;

    shmid = shmget(IPC_PRIVATE, size, SHM_R | SHM_W);
    if (shmid == -1) {
	error("fatal error: shmget() failed: %s", strerror(errno));
	return -1;
    }

    *buffer = shmat(shmid, NULL, 0);
    if (*buffer == NULL) {
	error("fatal error: shmat() failed: %s", strerror(errno));
	return -1;
    }

    return shmid;
}


void shm_destroy(const int shmid, const void *buffer)
{
    shmdt(buffer);
    shmctl(shmid, IPC_RMID, NULL);
}


int thread_create(const char *name, void (*thread) (void *data), void *data)
{
    pid_t pid;

    switch (pid = fork()) {
    case -1:
	error("fatal error: fork(%s) failed: %s", name, strerror(errno));
	return -1;
    case 0:
	info("thread %s starting...", name);
	if (thread_argc > 0) {
	    strncpy(thread_argv[0], name, strlen(thread_argv[0]));
	}
	thread(data);
	info("thread %s ended.", name);
	exit(0);
    default:
	info("forked process %d for thread %s", pid, name);
    }

    return pid;
}


int thread_destroy(const int pid)
{
    return kill(pid, SIGKILL);
}
