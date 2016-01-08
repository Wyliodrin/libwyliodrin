#ifndef _DEBUG_H
#define _DEBUG_H



/*** INCLUDES ************************************************************************************/

#include <stdio.h>  /* fprintf  */
#include <errno.h>  /* errno    */
#include <string.h> /* strerror */

/*************************************************************************************************/



/*** DEBUG MACROS ********************************************************************************/

#ifdef DEBUG
  #define error(assertion, action, msg, ...)                                                      \
    do {                                                                                          \
      if (assertion) {                                                                            \
        fprintf(stderr, "[error in %s:%d] " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__);         \
        fflush(stderr);                                                                           \
        action;                                                                                   \
      }                                                                                           \
    } while (0)
#else
  #define error(assertion, action, msg, ...) /* Do nothing */
#endif

#ifdef DEBUG
  #define syserror(assertion, action, msg, ...)                                                   \
    do {                                                                                          \
      if (assertion) {                                                                            \
        fprintf(stderr, "[syserror in %s:%d] " msg ": %s\n", __FILE__, __LINE__, ##__VA_ARGS__,   \
                                                             strerror(errno));                    \
        fflush(stderr);                                                                           \
        action;                                                                                   \
      }                                                                                           \
    } while (0)
#else
  #define syserror(assertion, action, msg, ...) /* Do nothing */
#endif

/*************************************************************************************************/



#endif /* _DEBUG_H */
