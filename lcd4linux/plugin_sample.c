/* $Id: plugin_sample.c 1091 2010-01-21 04:26:24Z michael $
 * $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/plugin_sample.c $
 *
 * plugin template
 *
 * Copyright (C) 2003 Michael Reinelt <michael@reinelt.co.at>
 * Copyright (C) 2004, 2005, 2006, 2007, 2008 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
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
 * int plugin_init_sample (void)
 *  adds various functions
 *
 */


#include "config.h"

/* these should always be included */
#include "debug.h"
#include "plugin.h"

/* define the include files you need */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif



/* sample function 'mul2' */
/* takes one argument, a number */
/* multiplies the number by 2.0 */
/* Note: all local functions should be declared 'static' */

static void my_mul2(RESULT * result, RESULT * arg1)
{
    double param;
    double value;

    /* Get Parameter */
    /* R2N stands for 'Result to Number' */
    param = R2N(arg1);

    /* calculate value */
    value = param * 2.0;

    /* store result */
    /* when called with R_NUMBER, it assumes the */
    /* next parameter to be a pointer to double */
    SetResult(&result, R_NUMBER, &value);
}


/* sample function 'mul3' */
/* takes one argument, a number */
/* multiplies the number by 3.0 */
/* same as 'mul2', but shorter */

static void my_mul3(RESULT * result, RESULT * arg1)
{
    /* do it all in one line */
    double value = R2N(arg1) * 3.0;

    /* store result */
    SetResult(&result, R_NUMBER, &value);
}


/* sample function 'diff' */
/* takes two arguments, both numbers */
/* returns |a-b| */

static void my_diff(RESULT * result, RESULT * arg1, RESULT * arg2)
{
    /* do it all in one line */
    double value = R2N(arg1) - R2N(arg2);

    /* some more calculations... */
    if (value < 0)
	value = -value;

    /* store result */
    SetResult(&result, R_NUMBER, &value);
}


/* sample function 'answer' */
/* takes no argument! */
/* returns the answer to all questions */

static void my_answer(RESULT * result)
{
    /* we have to declare a variable because */
    /* SetResult needs a pointer  */
    double value = 42;

    /* store result */
    SetResult(&result, R_NUMBER, &value);
}


/* sample function 'length' */
/* takes one argument, a string */
/* returns the string length */

static void my_length(RESULT * result, RESULT * arg1)
{
    /* Note #1: value *must* be double!  */
    /* Note #2: R2S stands for 'Result to String' */
    double value = strlen(R2S(arg1));

    /* store result */
    SetResult(&result, R_NUMBER, &value);
}



/* sample function 'upcase' */
/* takes one argument, a string */
/* returns the string in upper case letters */

static void my_upcase(RESULT * result, RESULT * arg1)
{
    char *value, *p;

    /* create a local copy of the argument */
    /* Do *NOT* try to modify the original string! */
    value = strdup(R2S(arg1));

    /* process the string */
    for (p = value; *p != '\0'; p++)
	*p = toupper(*p);

    /* store result */
    /* when called with R_STRING, it assumes the */
    /* next parameter to be a pointer to a string */
    /* 'value' is already a char*, so use 'value', not '&value' */
    SetResult(&result, R_STRING, value);

    /* free local copy again */
    /* Note that SetResult() makes its own string copy  */
    free(value);
}


/* sample function 'cat' */
/* takes variable number of arguments, all strings */
/* returns all prameters concatenated */

static void my_concat(RESULT * result, int argc, RESULT * argv[])
{
    int i, len;
    char *value, *part;

    /* start with a empty string */
    value = strdup("");

    /* process all arguments */
    for (i = 0; i < argc; i++) {
	part = R2S(argv[i]);
	len = strlen(value) + strlen(part);
	value = realloc(value, len + 1);
	strcat(value, part);
    }

    /* store result */
    SetResult(&result, R_STRING, value);

    /* free local string */
    free(value);
}


/* plugin initialization */
/* MUST NOT be declared 'static'! */
int plugin_init_sample(void)
{

    /* register all our cool functions */
    /* the second parameter is the number of arguments */
    /* -1 stands for variable argument list */
    AddFunction("sample::mul2", 1, my_mul2);
    AddFunction("sample::mul3", 1, my_mul3);
    AddFunction("sample::answer", 0, my_answer);
    AddFunction("sample::diff", 2, my_diff);
    AddFunction("sample::length", 1, my_length);
    AddFunction("sample::upcase", 1, my_upcase);
    AddFunction("sample::concat", -1, my_concat);

    return 0;
}

void plugin_exit_sample(void)
{
    /* free any allocated memory */
    /* close filedescriptors */
}
