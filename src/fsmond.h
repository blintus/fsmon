#ifndef FSMOND_H
#define FSMOND_H

#include <sys/inotify.h>

typedef struct {
	char *filepath;
	char *command;
	uint32_t mask;
} ThreadArg;

#endif