/**************************************************************************************************
 * Communication API.
 *
 * Author: Razvan Madalin MATEI <matei.rm94@gmail.com>
 * Date last modified: August 2015
 *************************************************************************************************/

#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H



#define REDIS_HOST "127.0.0.1"
#define REDIS_PORT 6379

#define CLIENT_CHANNEL "communication_client"
#define SERVER_CHANNEL "communication_server"


/**
 * Connect to redis and subscribe to CLIENT_CHANNEL:*.
 */
void init_communication();

/**
 * When a message is received on the CLIENT_CHANNEL:<label>, the handler_function will be called.
 * The message has the following JSON format: {"from": "<sender>", "data": "<data>"}.
 */
void set_handler_for_label(const char *label,
                           void (*handler_function)(const char *sender,
                                                    const char *label,
                                                    int error,
                                                    const char *data));

/**
 * Remove the handler set for a label.
 */
void clear_handler_for_label(const char *label);

/**
 * Send a message to a board with a specific label and corresponding data.
 * The message has the following JSON format: {"id": "<to>", "data": "<data>"}.
 * This message is published on SERVER_CHANNEL:<label>. The wyliodrind program
 * is subscribed to SERVER_CHANNEL:*. Once the message is received, is sent via
 * XMPP to the recipient and the recipient will publish on CLIENT:CHANNEL:<label>
 * the following message: {"from": "<sender>", "data": "<data>"}. This last message
 * will trigger the handler set via set_handler_for_label.
 */
void send_message(const char *to, const char *label, const char *data);

/**
 * This functions will block the program's execution for a specified number of seconds.
 * If <seconds> is zero or negative, the function will block until the user presses CTRL+C.
 */
void wait_for_messages(int seconds);

/**
 * Unsubscribe from CLIENT_CHANNEL:*.
 */
void close_communication();



#endif /* _COMMUNICATION_H */
