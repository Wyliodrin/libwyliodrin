#ifndef COMMUNICATION__H
#define COMMUNICATION__H

#ifdef __cplusplus
extern "C" {
#endif

//sends messages
int initCommunication(int redis_port);
int openConnection (int communication_port, void (*f)(int err, char *data));

#ifdef __cplusplus
}
#endif

#endif