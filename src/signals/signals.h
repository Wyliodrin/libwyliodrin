#ifndef _SIGNALS_H
#define _SIGNALS_H

#ifdef __cplusplus
extern "C" {
#endif

/*** API *****************************************************************************************/

/**
 * TODO
 */
int initSignal(int port, const char *pId, const char *sId, const char *userid);

/**
 * TODO
 */
int sendSignal(const char *name, double value);

/**
 * TODO
 */
int sendSignalAndFlag(const char *flag, const char *name, double value);

/**
 * TODO
 */
int sendSignalsAndFlag(const char *flag, const char *name, double value, ...);

/**
 * TODO
 */
int sendSignals(const char *name, double value, ...);

/**
 * TODO
 */
int putFlag(const char *sigal, const char *flag);

/**
 * TODO
 */
int sendSignalsList(const char **names, double *values, int elements);

/**
 * TODO
 */
int sendSignalsListAndFlag(const char *flag, const char **names, double *values, int elements);

/**
 * TODO
 */
int sendCoordinates(const char *name, double latitude, double longitude);

/**
 * TODO
 */
int sendCoordinatesAndFlag(const char *name, double latitude, double longitude, const char *flag);

/**
 * TODO
 */
int sendSignalXY(const char *name, double x, double y);

/**
 * TODO
 */
int sendSignalXYAndFlag(const char *name, double x, double y, const char *flag);


/*************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _SIGNALS_H */
