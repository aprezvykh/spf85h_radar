/* $Id: plugin_pop3.c 1159 2011-08-31 22:00:29Z jmccrohan $
 * $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/plugin_pop3.c $
 *
 * Plugin to check POP3 mail accounts
 *
 * Copyright (C) 2004 Javi Garcia Dominguez (aka Stolz) <javi@gsmlandia.com>
 * Based on code from  pop3check (C) 1999 http://sourceforge.net/projects/pop3check
 *     Simon Liddington <squidly@users.sourceforge.net> is the pop3check current maintainer.
 *     The pop3check original author is Steven Radack <steve@lconn.net>.
 *
 * Copyright (C) 2004 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
 *
 * This file is a pluging for LCD4Linux.
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


#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "debug.h"
#include "plugin.h"
#include "cfg.h"

/*added */
#include <sys/socket.h>
#include <sys/types.h>
/*#include <netinet/in.h> */
#include <netdb.h>
#include <unistd.h>
/*#include <pwd.h> */
#include <stdio.h>

#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif

/*POP 3 */
#define POPERR           "-ERR"
#define LOCKEDERR        "-ERR account is locked by another session or for maintenance, try again."
#define BUFSIZE          8192
#define POP3PORT         110
#define MAX_NUM_ACCOUNTS 3


struct check {
    int id;
    char *username;
    char *password;
    char *server;
    int port;
    int messages;
    struct check *next;
};


/************************ PROTOTYPES  ********************************/
/* list */
static struct check *check_node_alloc(void);
static void check_node_add(struct check **head, struct check *new_check);
static void check_destroy(struct check **head);

/* pop3 */
static void pop3_check_messages(struct check *hi, int verbose);
static void pop3_recv_crlf_terminated(int sockfd, char *buf, int size);

/* socket  */
static int tcp_connect(struct check *hi);


/************************ GLOBAL ***********************************/
static char Section[] = "Plugin:POP3";
static struct check *head = NULL;
/********************************************************************/


/************************ LIST  ***********************************/

static struct check *check_node_alloc(void)
{
    struct check *new_check;
    new_check = (struct check *) calloc(1, sizeof(struct check));
    if (new_check == NULL) {
	error("[POP3] out of memory\n");
    }
    return new_check;
}

static void check_node_add(struct check **head, struct check *new_check)
{
    new_check->next = *head;
    *head = new_check;
}

static void check_destroy(struct check **head)
{
    struct check *iter;
    while (*head) {
	iter = (*head)->next;
	free((*head)->username);
	free((*head)->password);
	free((*head)->server);
	free(*head);
	*head = iter;
    }
    *head = NULL;
}

/************************ POP3  ********************************/
static void pop3_check_messages(struct check *hi, int verbose)
{
    char buf[BUFSIZE];
    int sockfd;

    if ((sockfd = tcp_connect(hi)) < 0) {
	hi->messages = -1;
	return;
    }

    pop3_recv_crlf_terminated(sockfd, buf, sizeof(buf));	/* server greeting */
    if (verbose)
	info("[POP3] %s -> %s\n", hi->server, buf);

    snprintf(buf, sizeof(buf), "USER %s\r\n", hi->username);
    write(sockfd, buf, strlen(buf));
    buf[strlen(buf) - 1] = '\0';
    if (verbose)
	info("[POP3] %s <- %s\n", hi->server, buf);
    pop3_recv_crlf_terminated(sockfd, buf, sizeof(buf));	/* response from USER command */
    if (verbose)
	info("[POP3] %s -> %s\n", hi->server, buf);

    snprintf(buf, sizeof(buf), "PASS %s\r\n", hi->password);
    write(sockfd, buf, strlen(buf));
    if (verbose)
	info("[POP3] %s <- PASS ???\n", hi->server);
    pop3_recv_crlf_terminated(sockfd, buf, sizeof(buf));	/* response from PASS command */
    if (verbose)
	info("[POP3] %s -> %s\n", hi->server, buf);

    if (strncmp(buf, LOCKEDERR, strlen(LOCKEDERR)) == 0) {
	hi->messages = -2;
	close(sockfd);
	return;
    }
    if (strncmp(buf, POPERR, strlen(POPERR)) == 0) {
	error("[POP3] error logging into %s\n", hi->server);
	error("[POP3] server responded: %s\n", buf);
	hi->messages = -1;
	close(sockfd);
	return;
    }

    snprintf(buf, sizeof(buf), "STAT\r\n");
    write(sockfd, buf, strlen(buf));
    if (verbose)
	info("[POP3] %s <- STAT\n", hi->server);
    pop3_recv_crlf_terminated(sockfd, buf, sizeof(buf));	/* response from PASS command */
    if (verbose)
	info("[POP3] %s -> %s\n", hi->server, buf);

    strtok(buf, " ");
    hi->messages = atoi(strtok(NULL, " "));

    snprintf(buf, sizeof(buf), "QUIT\r\n");
    write(sockfd, buf, strlen(buf));
    if (verbose)
	info("[POP3] %s <- QUIT\n", hi->server);
    pop3_recv_crlf_terminated(sockfd, buf, sizeof(buf));	/* response from QUIT command */
    if (verbose)
	info("[POP3] %s -> %s\n", hi->server, buf);

    close(sockfd);
}

static void pop3_recv_crlf_terminated(int sockfd, char *buf, int size)
{
    /* receive one line server responses terminated with CRLF */
    char *pos;
    int bytes = 0;
    memset(buf, 0, size);
    while ((pos = strstr(buf, "\r\n")) == NULL)
	bytes += read(sockfd, buf + bytes, size - bytes);
    *pos = '\0';
}

/************************ SOCKET  ********************************/
static int tcp_connect(struct check *hi)
{
    struct sockaddr_in addr;
    struct hostent *he = gethostbyname(hi->server);
    int sockfd;

    if (hi == NULL)
	return -1;

    if (!he) {
	error("[POP3] Failed to lookup %s\n", hi->server);
	return (-1);
    }

    memset((char *) &addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    memcpy(&(addr.sin_addr.s_addr), he->h_addr, he->h_length);
    addr.sin_port = htons(hi->port);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	perror("socket()");
	return (-1);
    }

    if (connect(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr)) < 0) {
	perror("connect()");
	close(sockfd);
	return (-1);
    }

    return (sockfd);
}


static int getConfig(void)
{
    struct check *node = NULL;
    int i, n = 0;
    char *user = (char *) calloc(1, sizeof("user") + sizeof(int));
    char *password = (char *) calloc(1, sizeof("password") + sizeof(int));
    char *server = (char *) calloc(1, sizeof("server") + sizeof(int));
    char *port = (char *) calloc(1, sizeof("port") + sizeof(int));

    for (i = 1; i <= MAX_NUM_ACCOUNTS; i++) {
	char *x;
	sprintf(user, "user%d", i);
	sprintf(password, "password%d", i);
	sprintf(server, "server%d", i);
	sprintf(port, "port%d", i);

	x = cfg_get(Section, server, "");
	if (*x == '\0') {
	    info("[POP3] No '%s.%s' entry from %s, disabling POP3 account #%d", Section, server, cfg_source(), i);
	    free(x);
	} else {
	    node = check_node_alloc();
	    node->id = i;
	    node->server = x;
	    node->messages = 0;
	    node->next = NULL;

	    x = cfg_get(Section, user, "");
	    if (*x == '\0') {
		info("[POP3] No '%s.%s' entry from %s, disabling POP3 account #%d", Section, user, cfg_source(), i);
		free(x);
	    } else {
		node->username = x;
		x = cfg_get(Section, password, "");
		if (*x == '\0') {
		    info("[POP3] No '%s.%s' entry from %s, disabling POP3 account #%d", Section, password, cfg_source(),
			 i);
		    free(x);
		} else {
		    node->password = x;
		    if (cfg_number(Section, port, POP3PORT, 1, 65536, &node->port) < 1) {
			info("[POP3] No '%s.%s' entry from %s, %d will be used for account #%d", Section, port,
			     cfg_source(), POP3PORT, i);
		    }
		    check_node_add(&head, node);
		    n++;
		}
	    }
	}
    }
    return (n);
}


static int configure_pop3(void)
{
    static int configured = 0;
    int n;

    if (configured != 0)
	return configured;

    n = getConfig();
    /* by now, head should point to a list of all our accounts */
    if (head) {
	info("[POP3] %d POP3 accounts have been successfully defined", n);
	configured = 1;
    } else {
	configured = -1;
    }
    return configured;
}


static void my_POP3check(RESULT * result, RESULT * check)
{
    double param = R2N(check);
    struct check *node = NULL;
    double value;

    if (configure_pop3() < 0) {
	value = -1;
	SetResult(&result, R_NUMBER, &value);
	return;
    }

    for (node = head; node; node = node->next) {
	if (node->id == param)
	    break;
    }
    if (node == NULL) {		/*Inexistent account */
	value = -1;
    } else {
	pop3_check_messages(node, 0);
	value = (double) node->messages;
    }
    SetResult(&result, R_NUMBER, &value);
}


int plugin_init_pop3(void)
{
    AddFunction("POP3check", 1, my_POP3check);
    return 0;
}

void plugin_exit_pop3(void)
{
    check_destroy(&head);
}
