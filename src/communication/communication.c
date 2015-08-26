#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>

#include "communication.h"



#define MAX_CONNECTIONS 128



typedef struct {
  char *label;
  void (*handler_function)(const char *sender,
                           const char *label,
                           int error,
                           const char *data);
} connection_t;



static redisContext *c = NULL;
static redisAsyncContext *ac = NULL;
static bool is_connetion_in_progress = false;
static connection_t connections[MAX_CONNECTIONS];
static pthread_t communication_thread;


/**
 * TODO: Description here
 */
static void *init_communication_routine(void *args);

/**
 * TODO: Description here
 */
static void start_subscriber();

/**
 * TODO: Description here
 */
static void *start_subscriber_routine(void *arg);

/**
 * TODO: Description here
 */
static void connectCallback(const redisAsyncContext *c, int status);

/**
 * TODO: Description here
 */
static void disconnectCallback(const redisAsyncContext *c, int status);

/**
 * TODO: Description here
 *
 * Example of pmessage reply:
 * 0: pmessage
 * 1: communication_client:*
 * 2: communication_client:myLabel
 * 3: {"from": "matei94_pie@wyliodrin.com/c16f6698", "data": "evrika"}
 */
static void onMessage(redisAsyncContext *c, void *reply, void *privdata);


void init_communication() {
  pthread_create(&communication_thread, NULL, init_communication_routine, NULL);
  int i;
  for (i = 0; i < MAX_CONNECTIONS; i++) {
    connections[i].label = NULL;
  }
}


static void *init_communication_routine(void *args) {
  struct timeval timeout = {1, 500000};

  while (true) {
    c = redisConnectWithTimeout(REDIS_HOST, REDIS_PORT, timeout);
    if (c == NULL || c->err != 0) {
      fprintf(stderr, "redis connect error: %s\n", c->err != 0 ? c->errstr : "context is NULL");
      sleep(1);
    } else {
      start_subscriber();
      return NULL;
    }
  }
}


static void start_subscriber() {
  pthread_t t;
  pthread_create(&t, NULL, start_subscriber_routine, NULL);
  pthread_join(t, NULL);
}


static void *start_subscriber_routine(void *arg) {
  signal(SIGPIPE, SIG_IGN);
  struct event_base *base = event_base_new();

  ac = redisAsyncConnect(REDIS_HOST, REDIS_PORT);
  if (ac->err != 0) {
    fprintf(stderr, "redisAsyncConnect error: %s\n", ac->errstr);
    return NULL;
  }

  redisLibeventAttach(ac, base);
  redisAsyncSetConnectCallback(ac, connectCallback);
  redisAsyncSetDisconnectCallback(ac, disconnectCallback);
  redisAsyncCommand(ac, onMessage, NULL, "PSUBSCRIBE %s:*", CLIENT_CHANNEL);
  event_base_dispatch(base);

  return NULL;
}


static void connectCallback(const redisAsyncContext *c, int status) {
  if (status != REDIS_OK) {
    fprintf(stderr, "Connect error: %s\n", c->errstr);
  }
}


static void disconnectCallback(const redisAsyncContext *c, int status) {
  if (status != REDIS_OK) {
    fprintf(stderr, "Disconnect error: %s\n", c->errstr);
  }
}


static void onMessage(redisAsyncContext *c, void *reply, void *privdata) {
  redisReply *r = reply;
  int i;

  if (r != NULL) {
    if (r->elements >= 1 && strncmp(r->element[0]->str, "punsubscribe", 12) == 0) {
      redisAsyncDisconnect(c);
      return;
    }

    if (r->elements >= 1 && strncmp(r->element[0]->str, "pmessage", 8) == 0) {
      char *label = strchr(r->element[2]->str, ':');
      if (label != NULL) {
        label++;
      } else {
        fprintf(stderr, "Unexpected channel: %s\n", r->element[2]->str);
        return;
      }

      for (i = 0; i < MAX_CONNECTIONS; i++) {
        if (connections[i].label != NULL && strcmp(connections[i].label, label) == 0) {
          /* Get sender */
          char *colon = strchr(r->element[3]->str, ':');
          char *first_qm = strchr(colon, '\"');
          char *second_qm = strchr(first_qm + 1, '\"');
          char sender[second_qm - first_qm];
          snprintf(sender, second_qm - first_qm, "%s", first_qm + 1);

          /* Get data */
          colon = strchr(second_qm, ':');
          first_qm = strchr(colon, '\"');
          second_qm = strchr(first_qm + 1, '\"');
          char data[second_qm - first_qm];
          snprintf(data, second_qm - first_qm, "%s", first_qm + 1);

          connections[i].handler_function(sender, label, 0, data);
          return;
        }
      }
      fprintf(stderr, "There is no connection on label %s\n", label);
    }
  }
}


void open_connection(const char *label,
                     void (*handler_function)(const char *sender,
                                              const char *label,
                                              int error,
                                              const char *data)) {
  /* Sanity checks */
  if (label == NULL || strlen(label) == 0) {
    fprintf(stderr, "Null or empty label is not accepted\n");
    return;
  }
  if (handler_function == NULL) {
    fprintf(stderr, "Null handler_function is not accepted\n");
    return;
  }

  int conn_index;

  /* Update handler_function */
  for (conn_index = 0; conn_index < MAX_CONNECTIONS; conn_index++) {
    if (connections[conn_index].label != NULL &&
        strcmp(connections[conn_index].label, label) == 0) {
      connections[conn_index].handler_function = handler_function;
      return;
    }
  }

  for (conn_index = 0; conn_index < MAX_CONNECTIONS; conn_index++) {
    if (connections[conn_index].label == NULL) {
      break;
    }
  }
  if (conn_index == MAX_CONNECTIONS) {
    fprintf(stderr, "Maximum number of allowed connections\n");
    return;
  }

  connections[conn_index].label = strdup(label);
  connections[conn_index].handler_function = handler_function;
}


void close_connection(const char *label) {
  if (label == NULL || strlen(label) == 0) {
    fprintf(stderr, "Null or empty label is not accepted\n");
    return;
  }

  int conn_index;

  for (conn_index = 0; conn_index < MAX_CONNECTIONS; conn_index++) {
    if (connections[conn_index].label != NULL &&
        strcmp(connections[conn_index].label, label) == 0) {
      free(connections[conn_index].label);
      connections[conn_index].label = NULL;
      return;
    }
  }

  fprintf(stderr, "There is no open connection with label %s\n", label);
}


void send_message(const char *to, const char *label, const char *data) {
  int to_publish_size = strlen(to) + strlen(data) + 32;
  int pub_channel_size = strlen(label) + 32;
  char to_publish[to_publish_size];
  char pub_channel[pub_channel_size];

  snprintf(to_publish, to_publish_size, "{\"id\": \"%s\", \"data\": \"%s\"}", to, data);
  snprintf(pub_channel, pub_channel_size, "%s:%s", SERVER_CHANNEL, label);

  redisReply *reply = redisCommand(c, "PUBLISH %s %s", pub_channel, to_publish);
  if (reply == NULL) {
    fprintf(stderr, "Failed to publish on %s: %s\n", pub_channel, c->errstr);
  }
  freeReplyObject(reply);
}


void close_communication() {
  int i;

  for (i = 0; i < MAX_CONNECTIONS; i++) {
    if (connections[i].label != NULL) {
      free(connections[i].label);
    }
  }

  redisAsyncCommand(ac, NULL, NULL, "PUNSUBSCRIBE %s:*", CLIENT_CHANNEL);
  pthread_join(communication_thread, NULL);
}
