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
static bool is_connetion_in_progress = false;
static connection_t connections[MAX_CONNECTIONS];


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
static void onMessage(redisAsyncContext *c, void *reply, void *privdata);



void init_communication() {
  pthread_t t;
  pthread_create(&t, NULL, init_communication_routine, NULL);
  pthread_join(t, NULL);

  int i;
  for (i = 0; i < MAX_CONNECTIONS; i++) {
    connections[i].label = NULL;
  }
}


static void *init_communication_routine(void *args) {
  struct timeval timeout = {1, 500000};

  is_connetion_in_progress = true;

  while (true) {
    c = redisConnectWithTimeout(REDIS_HOST, REDIS_PORT, timeout);
    if (c == NULL || c->err != 0) {
      fprintf(stderr, "redis connect error: %s\n", c->err != 0 ? c->errstr : "context is NULL");
      sleep(1);
    } else {
      start_subscriber();

      is_connetion_in_progress = false;
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
  redisAsyncContext *c;

  signal(SIGPIPE, SIG_IGN);
  struct event_base *base = event_base_new();

  c = redisAsyncConnect(REDIS_HOST, REDIS_PORT);
  if (c->err != 0) {
    fprintf(stderr, "redisAsyncConnect error: %s\n", c->errstr);
    return NULL;
  }

  redisLibeventAttach(c, base);
  redisAsyncSetConnectCallback(c, connectCallback);
  redisAsyncCommand(c, onMessage, NULL, "PSUBSCRIBE %s:*", CLIENT_CHANNEL);
  event_base_dispatch(base);

  return NULL;
}


static void connectCallback(const redisAsyncContext *c, int status) {
  if (status != REDIS_OK) {
    fprintf(stderr, "connect error: %s\n", c->errstr);
  } else {
    printf("Connection success\n");
  }
}


static void onMessage(redisAsyncContext *c, void *reply, void *privdata) {
  printf("I got a message\n");

  redisReply *r = reply;
  int i;

  for (i = 0; i < r->elements; i++) {
    printf("%s\n", r->element[i]->str);
  }
}


void open_connection(const char *label,
                     void (*handler_function)(const char *sender,
                                             const char *label,
                                             int error,
                                             const char *data)) {
  int conn_index;

  for (conn_index = 0; conn_index < MAX_CONNECTIONS; conn_index++) {
    if (connections[conn_index].label != NULL) {
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


void send_message(const char *to, const char *label, const char *data) {
  int to_publish_size = strlen(to) + strlen(data) + 16;
  int pub_channel_size = strlen(label) + 32;
  char to_publish[to_publish_size];
  char pub_channel[pub_channel_size];

  snprintf(to_publish, to_publish_size, "{\"id\":%s, \"data\":%s }", to, data);
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
      connections[i].label = NULL;
    }
  }
}
