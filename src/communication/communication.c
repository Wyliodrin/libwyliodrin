#include <stdio.h>

#include "communication.h"



void initCommunication() {
  printf("initCommunication");
}


void openConnection(const char *label,
                    void (*handler_function)(const char *sender,
                                             const char *label,
                                             int error,
                                             const char *data)) {

}


void sendMessage(const char *to, const char *label, const char *data) {

}


void closeCommunication() {

}
