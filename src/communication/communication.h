#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H



#define REDIS_HOST "127.0.0.1"
#define REDIS_PORT 6379

#define CLIENT_CHANNEL "communication_client"
#define SERVER_CHANNEL "communication_server"


/**
 * TODO: Description here
 */
void init_communication();

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
