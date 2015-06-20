#include <hiredis/hiredis.h>
#include <jansson.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define IP "127.0.0.1"
#define PORT 6379
#define REDIS_ECONNECT -1
#define JSON_ERROR	-2
#define REDIS_EENV	-3
#define MESSAGES_ITEMS 100

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

void addSignal(const char * sig_name, double sig_value, json_t *signals)
{
	//char val[50];
	//printf("%s : %lf\n",sig_name, sig_value);
				char *n = strndup(sig_name,100);
				int i;
				for (i=0; i<strnlen(n, 100); i++)
				{
					char c = n[i];
					if(!(c>='0' && c<='9') && !(c>='A' && c<='Z') && !(c>='a' && c<='z') && c!='_' && c!='.')
					{
						n[i] = '_';
					}
				}
				json_object_set_new(signals, n, json_real(sig_value));
				free(n);
}

int messagesItems ()
{
	static time_t firstMessage = 0;
	static long messages = 0;
	if (firstMessage == 0)
	{
		time (&firstMessage);
	}

	time_t now;
	time (&now);

	if (now != firstMessage)
	{
		firstMessage = now;
		messages = 0;
	}
	else
	{
		messages = messages + 1;
	}
	// printf ("messages %d\n", messages);
	return messages < MESSAGES_ITEMS;
}

int sendSignalsAndFlag  (const char *text, const char *name, double value, ...)
{
	if(messagesItems() && projectId != NULL && c != NULL)
	{
		//printf("sendSignalsAndDebug\n");
		va_list arguments;
		va_start ( arguments, value );  

		json_t *root, *signals;
		root = json_object();
		signals = json_object();

		if(sessionId != NULL)
			json_object_set_new(root, "session", json_string(sessionId));

		struct timeval tv;
		gettimeofday(&tv, NULL);
		double time_in_mill = (tv.tv_sec) + ((tv.tv_usec) / 1000)/1000.0 ;
		json_object_set_new(root, "timestamp", json_real(time_in_mill));
		json_object_set_new(root, "userid", json_string(userId));

		if(text != NULL)
			json_object_set_new(root, "text", json_string(text));

		addSignal(name, value, signals);

		const char *sig_name;
		double sig_value;
		do
		{
			//printf("do\n");
			sig_name = va_arg(arguments, const char*);
			if(sig_name != NULL)
			{
				sig_value = va_arg(arguments, double);
				addSignal(sig_name, sig_value, signals);
			}
		}
		while(sig_name != NULL);
		va_end(arguments);

		json_object_set_new(root, "signals", signals);
		char *j = json_dumps(root, 0);
		if(j == NULL)
			return JSON_ERROR;

		//printf("comanda: rpush %s %s\n",projectId, j);
		redisCommand(c, "rpush %s %s", projectId, j);
		free(j);

		redisCommand(c, "publish wyliodrin signal:%s",projectId);
	}
	else
	{
		printf("Too many messages/s or no projectId\n");
		return REDIS_EENV;
	}
	return 0;
}

int sendSignals  (const char *name, double value, ...)
{
	if(messagesItems () && projectId != NULL && c!= NULL)
	{
		//printf("sendSignals\n");
		va_list arguments;
		va_start ( arguments, value );  

		json_t *root, *signals;
		root = json_object();
		signals = json_object();

		if(sessionId != NULL)
			json_object_set_new(root, "session", json_string(sessionId));

		struct timeval tv;
		gettimeofday(&tv, NULL);
		double time_in_mill = (tv.tv_sec) + ((tv.tv_usec) / 1000)/1000.0 ;
		json_object_set_new(root, "timestamp", json_real(time_in_mill));
		json_object_set_new(root, "userid", json_string(userId));

		addSignal(name, value, signals);

		const char *sig_name;
		double sig_value;
		do
		{
			//printf("do\n");
			sig_name = va_arg(arguments, const char*);
			if(sig_name != NULL)
			{
				sig_value = va_arg(arguments, double);
				addSignal(sig_name, sig_value, signals);
			}
		}
		while(sig_name != NULL);
		va_end(arguments);

		json_object_set_new(root, "signals", signals);
		char *j = json_dumps(root, 0);
		if(j == NULL)
			return JSON_ERROR;

		//printf("comanda: rpush %s %s\n",projectId, j);
		redisCommand(c, "rpush %s %s", projectId, j);
		free(j);

		redisCommand(c, "publish wyliodrin signal:%s",projectId);
	}
	else
	{
		printf("Too many messages/s or no projectId\n");
		return REDIS_EENV;
	}
	return 0;
}

int sendSignalsListAndFlag  (const char *flag, const char **names, double *values, int elements)
{
	if(messagesItems () && projectId != NULL && c!=NULL)
	{
		int i;
		json_t *root, *signals;
		root = json_object();
		signals = json_object();

		if(sessionId != NULL)
			json_object_set_new(root, "session", json_string(sessionId));

		struct timeval tv;
		gettimeofday(&tv, NULL);
		double time_in_mill = (tv.tv_sec) + ((tv.tv_usec) / 1000)/1000.0 ;
		json_object_set_new(root, "timestamp", json_real(time_in_mill));
		json_object_set_new(root, "userid", json_string(userId));

		if(flag != NULL)
			json_object_set_new(root, "text", json_string(flag));

		for (i = 0; i<elements; i++)
		{
			addSignal(names[i], values[i], signals);
		}
		json_object_set_new(root, "signals", signals);
		char *j = json_dumps(root, 0);
		if(j == NULL)
			return JSON_ERROR;

		//printf("comanda: rpush %s %s\n",projectId, j);
		redisCommand(c, "rpush %s %s", projectId, j);
		free(j);

		redisCommand(c, "publish wyliodrin signal:%s",projectId);
	}
	else
	{
		printf("Too many messages/s or no projectId\n");
		return REDIS_EENV;
	}
	return 0;
}

int sendSignalsList (const char **names, double *values, int elements)
{
	return sendSignalsListAndFlag(NULL, names, values, elements);
}

int sendSignalAndFlag(const char *tag, const char *name, double value)
{
	if(messagesItems() && projectId != NULL && c != NULL)
	{
		//printf("sendSignalAndDebug\n");
		json_t *root, *signals;
		root = json_object();
		signals = json_object();

		if(sessionId != NULL)
			json_object_set_new(root, "session", json_string(sessionId));

		struct timeval  tv;
		gettimeofday(&tv, NULL);
		double time_in_mill = (tv.tv_sec) + ((tv.tv_usec) / 1000)/1000.0 ; // convert tv_sec & tv_usec to millisecond
		json_object_set_new(root, "timestamp", json_real(time_in_mill));
		json_object_set_new(root, "userid", json_string(userId));
		if(tag != NULL)
		{
			json_object_set_new(root, "text", json_string(tag));
		}
		addSignal(name, value, signals);
		json_object_set_new(root, "signals", signals);

		char *j = json_dumps(root, 0);
		// for (i=0; i<=strlen(j); i++) printf ("%c %d ", j[i], j[i]);
		//printf ("\n");
		if(j == NULL)
			return JSON_ERROR;
		// printf("comanda: rpush %s %s\n",projectId, j);
		redisCommand(c, "rpush %s %s", projectId, j);
		// printf(" error = %s\n", c->errstr);
		// printf("repl = %p\n",reply);
		free(j);

		redisCommand(c, "publish wyliodrin signal:%s",projectId);
		//free(n);
	//printf("repl = %s\n",reply->str);
	}
	else
	{
		printf("Too many messages/s or no projectId\n");
		return REDIS_EENV;
	}
	return 0;
}

int sendSignal  (const char *name, double value)
{
	//printf("sendSignal\n");
	return sendSignalAndFlag(NULL, name, value);
}

int putFlag (const char *signal, const char *flag)
{
	char newsig[200];
	snprintf(newsig, 199, "debug_%s",signal);
	sendSignalAndFlag(flag,newsig, 0.0);
}

int sendCoordinates(const char *name, double latitude, double longitude)
{
	return sendCoordinatesAndFlag(name, latitude, longitude, NULL);
}

int sendCoordinatesAndFlag(const char *name, double latitude, double longitude, const char *flag)
{
	int rc;
	char *lon = malloc(100 *sizeof(char));
	char *lat = malloc(100 *sizeof(char));
	snprintf(lat, 99, "%s_latitude",name);
	snprintf(lon, 99, "%s_longitude",name);
	if(flag != NULL)
		rc = sendSignalsAndFlag(flag,lat,latitude,lon,longitude,NULL);
	else
		rc = sendSignals(lat,latitude,lon,longitude,NULL);
	free(lon);
	free(lat);
	return rc;

}

int sendSignalXY(const char *name, double x, double y)
{
	return sendSignalXYAndFlag(name, x, y, NULL);
}

int sendSignalXYAndFlag(const char *name, double x, double y, const char *flag)
{
	int rc;
	char *xstr = malloc(100 *sizeof(char));
	char *ystr = malloc(100 *sizeof(char));
	snprintf(xstr, 99, "x_%s",name);
	snprintf(ystr, 99, "y_%s",name);
	if(flag != NULL)
		rc = sendSignalsAndFlag(flag,xstr,x,ystr,y,NULL);
	else
		rc = sendSignals(xstr,x,ystr,y,NULL);
	free(xstr);
	free(ystr);
	return rc;

}
