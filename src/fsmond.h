#ifndef FSMOND_H
#define FSMOND_H

#include <pthread.h>

#define OPT_ARGS "dkp:h"
static struct option long_options[] = {
	{"pidfile", required_argument, 0, 'p'},
	{"help",    no_argument,       0, 'h'}
};

static char *PID_FILE = "/tmp/fsmon.pid";
static int PID_FILE_STATIC = 1;

pthread_t *threads = NULL;
int numThreads = 0;

#endif