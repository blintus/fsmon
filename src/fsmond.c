#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>

#include "fsmond.h"

// IN_ACCESS         File was accessed (read) (*).
// IN_ATTRIB         Metadata  changed, e.g., permissions, timestamps, extended attributes, link count (since Linux 2.6.25), UID, GID, etc. (*).
// IN_CLOSE_WRITE    File opened for writing was closed (*).
// IN_CLOSE_NOWRITE  File not opened for writing was closed (*).
// IN_CREATE         File/directory created in watched directory (*).
// IN_DELETE         File/directory deleted from watched directory (*).
// IN_DELETE_SELF    Watched file/directory was itself deleted.
// IN_MODIFY         File was modified (*).
// IN_MOVE_SELF      Watched file/directory was itself moved.
// IN_MOVED_FROM     File moved out of watched directory (*).
// IN_MOVED_TO       File moved into watched directory (*).
// IN_OPEN           File was opened (*).
// IN_ALL_EVENTS     All events.

uint32_t getMask(char *maskStr) {
	char *s = strtok(maskStr, "|");
	uint32_t mask = 0;
	do {
		if (strcmp(s, "ACCESS") == 0)
			mask |= IN_ACCESS;
		else if (strcmp(s, "ATTRIB") == 0)
			mask |= IN_ATTRIB;
		else if (strcmp(s, "CLOSE_WRITE") == 0)
			mask |= IN_CLOSE_WRITE;
		else if (strcmp(s, "CLOSE_NOWRITE") == 0)
			mask |= IN_CLOSE_NOWRITE;
		else if (strcmp(s, "CREATE") == 0)
			mask |= IN_CREATE;
		else if (strcmp(s, "DELETE") == 0)
			mask |= IN_DELETE;
		else if (strcmp(s, "DELETE_SELF") == 0)
			mask |= IN_DELETE_SELF;
		else if (strcmp(s, "MODIFY") == 0)
			mask |= IN_MODIFY;
		else if (strcmp(s, "MOVE_SELF") == 0)
			mask |= IN_MOVE_SELF;
		else if (strcmp(s, "MOVED_FROM") == 0)
			mask |= IN_MOVED_FROM;
		else if (strcmp(s, "MOVED_TO") == 0)
			mask |= IN_MOVED_TO;
		else if (strcmp(s, "OPEN") == 0)
			mask |= IN_OPEN;
		else if (strcmp(s, "ALL_EVENTS") == 0)
			mask |= IN_ALL_EVENTS;
	} while((s = strtok(NULL, "|")));
	return mask;
}

int main(int argc, char *argv[]) {

	int pidFileLen = 0;

	int option_index = 0;
	int arg;
	while ((arg = getopt_long(argc, argv, OPT_ARGS, long_options, &option_index)) != -1) {
		switch (arg) {
		case 'd':
			printf("This will eventually make the program daemonize itself.\n");
			printf("This is not implemented yet. The program will now exit.\n");
			exit(0);
			break;

		case 'k':
			printf("This will eventually make the daemonized program exit.\n");
			printf("This is not implemented yet. The program will now exit.\n");
			exit(0);
			break;

		case 'p':
			// printf ("option -p with value `%s'\n", optarg);
			pidFileLen = strlen(optarg);
			PID_FILE = malloc(sizeof(char) * (pidFileLen + 1));
			strcpy(PID_FILE, optarg);
			PID_FILE_STATIC = 0;
			break;

		case 'h':
			printf("fsmond [-d][-k][-p PID_FILE|--pidfile=PID_FILE]\n");
			exit(0);
			break;

		case '?':
			exit(1);
			/* getopt_long already printed an error message. */
			break;

		default:
			abort();
		}
	}

	// if (daemon) {
	// 	fork here
	// }

	FILE *f = fopen(PID_FILE, "w");
	if (f) {
		pid_t myPid = getpid();
		fprintf(f, "%d\n", myPid);
		fclose(f);
	} else {
		fprintf(stderr, "Error creating pid file.\n");
	}

	char *line = NULL;
	size_t lineSize;
	ssize_t length;
	FILE *tab = fopen("test.fsmontab", "r");

	char *maskStrTmp = NULL;
	char *maskStr = NULL;

	while ((length = getline(&line, &lineSize, tab)) != -1) {
		if (line[length - 1] == '\n') {
			length--;
			line[length] = '\0';
		}
		if (line[0] == '#') continue;
		else if (strcmp(line, "") == 0) continue;

		printf("File: %s\n", strtok(line, " "));

		maskStrTmp = strtok(NULL, " ");
		maskStr = realloc(maskStr, sizeof(char) * (strlen(maskStrTmp) + 1));
		strcpy(maskStr, maskStrTmp);

		printf("Command: %s\n", strtok(NULL, ""));

		uint32_t mask = getMask(maskStr);
		printf("%d\n", mask);
	}

	free(maskStr);
	free(line);

	unlink(PID_FILE);

	if (!PID_FILE_STATIC) free(PID_FILE);

	return 0;
}