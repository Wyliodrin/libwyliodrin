
#ifndef SIGNALS__H
#define SIGNALS__H

#ifdef __cplusplus
extern "C" {
#endif

// timestamps and sends values
void sendSignal  (const char *name, double value);
void sendSignals  (const char *name, double values, ...);
void sendSignalsList  (const char *names[], double values[]);

#ifdef __cplusplus
}
#endif

#endif