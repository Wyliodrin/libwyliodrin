#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "hiredis/hiredis.h"
#include "hiredis/async.h"
#include "hiredis/adapters/libevent.h"

#define IP "127.0.0.1"
#define PORT 6379
#define REDIS_ECONNECT -1

redisAsyncContext *c;
redisContext *context;
char *redis_channel = "communication:";

void redisDisconnect (const redisAsyncContext *c, int status)
{
	if(status != REDIS_OK)
	{
		asyncConnect(PORT);
	}
}

int asyncConnect(int redis_port)
{
	c = redisAsyncConnect(IP, redis_port);
	signal(SIGPIPE, SIG_IGN);
	if (c != NULL && c->err)
	{
		return REDIS_ECONNECT;
	} 
	if(c == NULL)
		return REDIS_ECONNECT;
	redisAsyncSetDisconnectCallback(c, redisDisconnect);
	return 0;
}

int initCommunication(int redis_port)
{
	int rc = asyncConnect(redis_port);
	if (rc != 0)
		return rc;

	context = redisConnect(IP, redis_port);
	if (context != NULL && context->err)
	{
		return REDIS_ECONNECT;
	} 
	if(context == NULL)
		return REDIS_ECONNECT;
	return 0;
}

void onMessage(redisAsyncContext *c, void *reply, void *privdata) {
    int j;
    char *data;
    redisReply *r = reply;
    void (*f)(int err, char *data) = privdata;
    if (reply == NULL) return;
    if (r->type == REDIS_REPLY_ARRAY) {
        for (j = 0; j < (r->elements-2); j=j+3) {
            data = r->element[j]->str;
          	if(strcmp(data, "message") == 0)
          	{
          		data = r->element[j+2]->str;
          		f(0, data);
          	}          
        }
    }
}

void openConnection (int communication_port, void (*f)(int err, char *data))
{
	struct event_base *base = event_base_new();
	initCommunication(6379);
	redisLibeventAttach(c, base);
    redisAsyncCommand(c, onMessage, f, "SUBSCRIBE %s%d",redis_channel,communication_port);
    event_base_dispatch(base);
}

int sendMessage (int communication_port, char *data)
{
	redisReply *reply = redisCommand(context, "PUBLISH %s%d %s", redis_channel, communication_port, data);
	if(reply)
	{
		freeReplyObject(reply);
		return 0;
	}		
	else
		return -1;
}

int closeCommunication ()
{
	redisFree(context);
	redisAsyncDisconnect(c);
}

