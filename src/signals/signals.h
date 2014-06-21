
#ifndef SIGNALS__H
#define SIGNALS__H

#ifdef __cplusplus
extern "C" {
#endif

// timestamps and sends values
int initSignal(int port, const char *pId, const char *sId, const char *userid);
int sendSignal  (const char *name, double value);
int sendSignalAndDebug (const char *text, const char *name, double value);
int sendSignalsAndDebug  (const char *text, const char *name, double value, ...);
int sendSignals  (const char *name, double value, ...);
// void sendSignalsList  (const char *names[], float values[]);

#ifdef __cplusplus
}
#endif

#endif