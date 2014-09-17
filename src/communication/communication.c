#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include "hiredis/hiredis.h"
#include "hiredis/async.h"
#include "hiredis/adapters/libevent.h"
#include <time.h>

#define IP "127.0.0.1"
#define REDIS_ECONNECT -1

redisAsyncContext *c;


int initCommunication(int redis_port)
{
	
	c = redisAsyncConnect(IP, redis_port);
	signal(SIGPIPE, SIG_IGN);
	if (c != NULL && c->err)
	{
		return REDIS_ECONNECT;
	} 
	if(c == NULL)
		return REDIS_ECONNECT;
	return 0;
}

int openConnection (int communication_port, void (*f)(int err, char *data))
{

}

void onMessage(redisAsyncContext *c, void *reply, void *privdata) {
    int j;
    redisReply *r = reply;
    if (reply == NULL) return;

    if (r->type == REDIS_REPLY_ARRAY) {
        for (j = 0; j < r->elements; j++) {
            printf("%u) %s\n", j, r->element[j]->str);
        }
    }
}

void *m()
{
	
	struct event_base *base = event_base_new();
	initCommunication(6379);
	redisLibeventAttach(c, base);
    redisAsyncCommand(c, onMessage, NULL, "SUBSCRIBE a");
    event_base_dispatch(base);   
}

int main()
{
	pthread_t a;
	pthread_create(&a, NULL, &m, NULL);
	printf("ana");
}



