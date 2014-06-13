#include <hiredis/hiredis.h>
#include <jansson.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define IP "127.0.0.1"
#define PORT 6379
#define REDIS_ECONNECT -1
#define JSON_ERROR	-2
#define REDIS_EENV	-3

redisContext *c;
const char *projectId;
const char *sessionId;

int initSignal(int port, const char *pId, const char *sId)
{
	if(pId == NULL)
		return REDIS_ECONNECT;
	projectId = pId;
	sessionId = sId;
	c = redisConnect(IP, port);
	if (c != NULL && c->err)
	{
		return REDIS_ECONNECT;
	} 
	if(c == NULL)
		return REDIS_ECONNECT;
	if(projectId == NULL)
		return REDIS_EENV;
    return 0;
}


/*
{
session: “session id to send data live” - optional
timestamp: “timestamp in milliseconds”	
name: “component name”
value: “float value”
}
*/

int sendSignal(char *name, float value)
{
	if(projectId != NULL)
	{
		json_t *root;
		root = json_object();

		if(sessionId != NULL)
			json_object_set_new(root, "session", json_string(sessionId));

		struct timeval  tv;
		gettimeofday(&tv, NULL);
		double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; // convert tv_sec & tv_usec to millisecond
		char t[100];
		sprintf(t, "%d", time_in_mill);
		json_object_set_new(root, "timestamp", json_string(t ));

		json_object_set_new(root, "name", json_string(name));
		char val[50];
		sprintf(val, "%f",value);
		json_object_set_new(root, "value", json_string(val));

		char *j = json_dumps(root, 0);
		if(j == NULL)
			return JSON_ERROR;
		
		redisReply *reply = redisCommand(c, "rpush %s %s", projectId, j);
		//printf("repl = %s\n",reply->str);
		free(j);

		reply = redisCommand(c, "publish wyliodrin signal:%s",projectId);
	//printf("repl = %s\n",reply->str);
	}
	else
	{
		printf("No projectId\n");
		return REDIS_EENV;
	}
	return 0;
}