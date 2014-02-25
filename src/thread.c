#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "fsmond.h"
#include "thread.h"

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

void *thread_main(void *arg) {

	ThreadArg *targ = (ThreadArg*) arg;

	int length, i = 0;
	int fd;
	int wd;
	char buffer[EVENT_BUF_LEN];

	/*creating the INOTIFY instance*/
	fd = inotify_init();

	/*checking for error*/
	if ( fd < 0 ) {
		perror( "inotify_init" );
	}

	/*adding the “/tmp” directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
	wd = inotify_add_watch( fd, targ->filepath, targ->mask );

	printf("watches added to %s\n", targ->filepath);

	while (1) {

		/*read to determine the event change happens on “/tmp” directory. Actually this read blocks until the change event occurs*/ 
		length = read( fd, buffer, EVENT_BUF_LEN ); 

		/*checking for error*/
		if ( length < 0 ) {
			perror( "read" );
		}  

		/*actually read return the list of change events happens. Here, read the change event one by one and process it accordingly.*/
		while ( i < length ) {
			struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
			if ( event->len ) {
				printf("Command: %s\n%s\n", targ->command, event->name);
			}
			i += EVENT_SIZE + event->len;
		}

		i = 0;
	}

	/*removing the “/tmp” directory from the watch list.*/
	 inotify_rm_watch( fd, wd );

	/*closing the INOTIFY instance*/
	 close( fd );

	pthread_exit(NULL);
}