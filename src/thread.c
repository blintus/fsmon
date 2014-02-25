#include <stdio.h>
#include <pthread.h>

#include "fsmond.h"
#include "thread.h"

void *thread_main(void *arg) {

	ThreadArg *targ = (ThreadArg*) arg;
	printf("file: %s\ncommand: %s\nmask: %d\n", targ->filepath, targ->command, targ->mask);

	pthread_exit(NULL);
}