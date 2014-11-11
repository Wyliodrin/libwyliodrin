#ifndef COMMUNICATION__H
#define COMMUNICATION__H

#ifdef __cplusplus
extern "C" {
#endif

//sends messages
int initCommunication (int redis_port);
void openConnection (int communication_port, void (*f)(int err, char *data));
int sendMessage (char *id, int communication_port, char *data);
int closeCommunication ();

#ifdef __cplusplus
}
#endif

#endif