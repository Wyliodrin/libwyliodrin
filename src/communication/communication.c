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

struct priv_data
{
	void (*f)(int err, char *data);
	int port;
}

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

void onMessage(redisAsyncContext *c, void *reply, void *privdata) {
    int j;
    char *data;
    char *token;
    char *port_temp;
    int port;
    redisReply *r = reply;
    if (reply == NULL) return;

    if (r->type == REDIS_REPLY_ARRAY) {
        for (j = 0; j < r->elements; j++) {
            data = r->element[j]->str);
			if(strchr(data, ":"))
			{
				token = strtok(data, ":");
				strcpy(port_temp, token);
				int port = atoi(port_temp);
				token = strtok(NULL, ":");
				if(port == privdatap->port)
			}
        }
    }
}

int openConnection (int communication_port, void (*f)(int err, char *data))
{
	struct priv_data *pd = malloc(sizeof(struct priv_data));
	pd->f = f;
	pd->port = communication_port;
	struct event_base *base = event_base_new();
	initCommunication(6379);
	redisLibeventAttach(c, base);
    redisAsyncCommand(c, onMessage, pd, "SUBSCRIBE communication");
    event_base_dispatch(base);
}



