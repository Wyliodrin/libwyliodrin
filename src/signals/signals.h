
#ifndef SIGNALS__H
#define SIGNALS__H

#ifdef __cplusplus
extern "C" {
#endif

// timestamps and sends values
int initSignal(int port, const char *pId, const char *sId, const char *userid);
void sendSignal  (const char *name, float value);
// void sendSignals  (const char *name, float values, ...);
// void sendSignalsList  (const char *names[], float values[]);

#ifdef __cplusplus
}
#endif

#endif