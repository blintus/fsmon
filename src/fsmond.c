#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define OPT_ARGS "dkp:h"
static struct option long_options[] = {
	{"pidfile", required_argument, 0, 'p'},
	{"help",    no_argument,       0, 'h'}
};

static char *PID_FILE = "/var/run/fsmon.pid";
static int PID_FILE_STATIC = 1;

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
			/* getopt_long already printed an error message. */
			break;

		default:
			abort();
		}
	}

	// if (daemon) {
	// 	fork here
	// }

	pid_t myPid = getpid();

	char *line = NULL;
	size_t lineSize;
	ssize_t length;
	FILE *tab = fopen("test.fsmontab", "r");

	while ((length = getline(&line, &lineSize, tab)) != -1) {
		if (line[length - 1] == '\n') {
			length--;
			line[length] = '\0';
		}
		printf("File: %s\n", strtok(line, " "));
		printf("Mode: %s\n", strtok(NULL, " "));
		printf("Command: %s\n", strtok(NULL, ""));
	}

	free(line);

	if (!PID_FILE_STATIC) free(PID_FILE);

	return 0;
}