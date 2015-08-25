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



static redisContext *c = NULL;
static bool is_connetion_in_progress = false;



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
  redisAsyncDisconnect(c);
}


void open_connection(const char *label,
                     void (*handler_function)(const char *sender,
                                             const char *label,
                                             int error,
                                             const char *data)) {

}


void send_message(const char *to, const char *label, const char *data) {

}


void close_communication() {

}
