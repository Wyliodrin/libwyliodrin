#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "signals/signals.h"
//#include "wiring/wiring.h"

// #define VERBOSE

#define LIBWYLIODRIN_VERSION_MAJOR 1
#define LIBWYLIODRIN_VERSION_MINOR 16

int get_version_major() {
	return LIBWYLIODRIN_VERSION_MAJOR;
}

int get_version_minor() {
	return LIBWYLIODRIN_VERSION_MINOR;
}

static int sock = -1;

/**
 * Writes an error to stderr without modifying errno.
 */
// #define debug_write(str)                \
//     do {                                \
// 		int verbose_errno_save = errno; \
// 		write(2, str, strlen(str));     \
// 		errno = verbose_errno_save;     \
//     } while (0)

void wyliodrinSetup()
{
	wiringSetup ();
	//wyliodrin_port, wyliodrin_project, wyliodrin_session, wyliodrin_userid - environment variables
	char * val = getenv("wyliodrin_port");
	int port = 0;
	if(val != NULL)
		port = atoi(val);
	if(port == 0)
		port = 6379;

	char *projectId = getenv("wyliodrin_project");
	//printf("projid = %s\n", projectId);
	char *sessionId = getenv("wyliodrin_session");
	char *userid = getenv("wyliodrin_userid");
	//printf("port = %d\nprojectid = %s\nsessionid = %s\nuserid=%s\n",port,projectId,sessionId,userid);
	initSignal(port, projectId, sessionId, userid);
	// signalSetup ();
}

// void connectWyliodrin()
// {
// 	if (sock == -1) {
// 		sock = create_inet_stream_socket("127.0.0.1", getenv("wyliodrin_port"), LIBSOCKET_IPv4, 0);

// 		if (sock >= 0) {
// 			char id[1000];
// 			sprintf(id, "id %s\n", getenv("wyliodrin_id"));

// 			if (write (sock, id, strlen(id)) < 0) {
// 				sock = -1;

// 				#ifdef VERBOSE
// 					debug_write("Write attemp failed\n");
// 				#endif
// 			}
// 		} else {
// 			#ifdef VERBOSE
// 				debug_write("Connection attempt failed\n");
// 			#endif
// 		}
// 	} else {
// 		#ifdef VERBOSE
// 			debug_write("Trying to connect when there already exists a connection established\n");
// 		#endif
// 	}
// }

// void sendSignal(const char *name, double value)
// {
// 	connectWyliodrin();

// 	if (sock >= 0) {
// 		char data[1000];
// 		sprintf(data, "%s %lf\n", name, value);

// 		if (write(sock, data, strlen (data)) < 0) {
// 			sock = -1;

// 			#ifdef VERBOSE
// 				debug_write("Sending signal failed\n");
// 			#endif
// 		}
// 	} else {
// 		#ifdef VERBOSE
// 			debug_write("No connection established\n");
// 		#endif
// 	}
// }
