#include <pthread.h>

#include "thread.h"

void *thread_main(void *arg) {

	pthread_exit(NULL);
}