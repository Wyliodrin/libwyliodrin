/**************************************************************************************************
 * Wyliodrin include and setup
 *************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "signals/signals.h"

static int sock = -1;

/**
 * Wyliodrin initialization setup
 */
void wyliodrinSetup()
{
  wiringSetup();

  char * val = getenv("wyliodrin_port");
  int port = 0;

  if(val != NULL)
  {
    port = atoi(val);
  }
  if(port == 0)
  {
    port = 6379;
  }

  char *projectId = getenv("wyliodrin_project");
  char *sessionId = getenv("wyliodrin_session");
  char *userid = getenv("wyliodrin_userid");
  
  initSignal(port, projectId, sessionId, userid);
}
