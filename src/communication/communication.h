#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H



#define REDIS_HOST "127.0.0.1"
#define REDIS_PORT 6379

#define CLIENT_CHANNEL "communication_client"


/**
 * TODO: Description here
 */
void init_communication();

/**
 * TODO: Description here
 */
void *init_communication_routine(void *args);

/**
 * TODO: Description here
 */
void start_subscriber();

/**
 * TODO: Description here
 */
void *start_subscriber_routine(void *arg);

/**
 * TODO: Description here
 */
void connectCallback(const redisAsyncContext *c, int status);

/**
 * TODO: Description here
 */
void onMessage(redisAsyncContext *c, void *reply, void *privdata);

/**
 * TODO: Description here
 */
void open_connection(const char *label,
                     void (*handler_function)(const char *sender,
                                             const char *label,
                                             int error,
                                             const char *data));

/**
 * TODO: Description here
 */
void send_message(const char *to, const char *label, const char *data);

/**
 * TODO: Description here
 */
void close_communication();



#endif /* _COMMUNICATION_H */
