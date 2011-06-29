/*
 * =====================================================================================
 *
 *       Filename:  run_with_timeout.c
 *
 *    Description:  Execute a command with a timeout.  When the timeout is reached,
 *                  the command and all of it's descendant processes are killed.
 *
 *        Version:  1.0
 *        Created:  06/25/11 13:21:32 CEST
 *       Revision:  none
 *       Compiler:  gcc / clang
 *
 *         Author:  Frank Vanderhallen (frank.vanderhallen@gmail.com)
 *
 * -------------------------------------------------------------------------------------
 *
 * This program can be distributed under the terms of the GNU GPL.
 * See the file COPYING.
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>

static int pid = 0;

#define USAGE \
"\nUsage:" \
"\n" \
"\n\trun_with_timeout <timeout in seconds> '<command>'" \
"\n"

static void print_usage(void)
{
    puts(USAGE);
    exit(-1);
}

static void signal_handler(int signum)
{
    int child_gid = 0;

    child_gid = getpgid(pid);
    if (child_gid > 0) {
	killpg(child_gid, SIGTERM);
    }
}

int main(int argc, char *const argv[])
{
    long timeout = 0;

    if (argc < 3) print_usage();
    timeout = strtol(argv[1], NULL, 10);
    if (timeout == 0) print_usage();

    pid = fork();

    if (pid == 0) {
	/* make the child process a process group leader */
	setpgrp();
	if (-1 == execl("/bin/sh", "/bin/sh", "-c", argv[2], (char *)0)) {
	    perror("run_with_timeout");
	    _exit(errno);
	}
    } else if (pid > 0) {
	int status = 0;
	struct itimerval timer = {};

	/* install signal handlers */
	signal(SIGHUP, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGALRM, signal_handler);

	/* install a timer */
	timer.it_value.tv_sec = timeout;
	if (-1 == setitimer(ITIMER_REAL, &timer, NULL)) {
	    perror("setitimer");
	    exit(errno);
	}

	/* wait for child termination */
	waitpid(pid, &status, 0);
	return status;
    } else {
	return -1;
    }
    return 0;
}

/* vim: set ts=8 sw=4 tw=0: */

