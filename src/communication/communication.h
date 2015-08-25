#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H



/**
 * TODO: Description here
 */
void initCommunication();

/**
 * TODO: Description here
 */
void openConnection(const char *label,
                    void (*handler_function)(const char *sender,
                                             const char *label,
                                             int error,
                                             const char *data));

/**
 * TODO: Description here
 */
void sendMessage(const char *to, const char *label, const char *data);

/**
 * TODO: Description here
 */
void closeCommunication();



#endif /* _COMMUNICATION_H */
