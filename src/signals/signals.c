#include <hiredis/hiredis.h>
#include <jansson.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define IP "127.0.0.1"
#define PORT 6379
#define REDIS_ECONNECT -1
#define JSON_ERROR	-2
#define REDIS_EENV	-3

redisContext *c;
const char *projectId;
const char *sessionId;
const char *userId;

int initSignal(int port, const char *pId, const char *sId, const char *uid)
{
	if(pId == NULL)
		return REDIS_ECONNECT;
	projectId = pId;
	sessionId = sId;
	userId = uid;
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
   "projectid":"89452744-4f73-4c29-a58a-83ab3bda5fe2", -se pune din clientul nodejs 
   "gadgetid":"alexandru.radovici_galileo@wyliodrin.org", -se pune din clientul nodejs
   "userid":""
   "session":"",
   "timestamp":"12",
   "signals":{
    "s1":"1",
   "s3":"3"
   }
}

*/

int sendSignal(const char *name, float value)
{
	printf ("%s: %f\n", name, value);
	if(projectId != NULL)
	{
		json_t *root, *signals;
		root = json_object();
		signals = json_object();

		if(sessionId != NULL)
			json_object_set_new(root, "session", json_string(sessionId));

		struct timeval  tv;
		gettimeofday(&tv, NULL);
		double time_in_mill = (tv.tv_sec) + ((tv.tv_usec) / 1000)/1000.0 ; // convert tv_sec & tv_usec to millisecond
		char t[100];
		sprintf(t, "%.3lf", time_in_mill);
		// sprintf(t, "%lu", tv.tv_sec);
		json_object_set_new(root, "timestamp", json_string(t));
		json_object_set_new(root, "userid", json_string(userId));

		char val[50];
		sprintf(val, "%f",value);
		// printf("float = %f\n",value);
		// printf("char = %s\n",val);
		char *n = strndup(name,100);
		int i;
		for (i=0; i<strnlen(n, 100); i++)
		{
			char c = n[i];
			if(!(c>='0' && c<='9') && !(c>='A' && c<='Z') && !(c>='a' && c<='z') && c!='_' && c!='.')
			{
				n[i] = '_';
				// printf("a intrat in if %c\n",n[i]);
			}
		}
		// printf("a iesit din for\n");
		json_object_set_new(signals, n, json_string(val));
		
		json_object_set_new(root, "signals", signals);

		char *j = json_dumps(root, 0);
		// for (i=0; i<=strlen(j); i++) printf ("%c %d ", j[i], j[i]);
		printf ("\n");
		if(j == NULL)
			return JSON_ERROR;
		// printf("comanda: rpush %s %s\n",projectId, j);
		redisReply *reply = redisCommand(c, "rpush %s %s", projectId, j);
		// printf(" error = %s\n", c->errstr);
		// printf("repl = %p\n",reply);
		free(j);

		reply = redisCommand(c, "publish wyliodrin signal:%s",projectId);
		free(n);
	//printf("repl = %s\n",reply->str);
	}
	else
	{
		printf("No projectId\n");
		return REDIS_EENV;
	}
	return 0;
}