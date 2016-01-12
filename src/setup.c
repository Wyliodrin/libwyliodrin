/*** INCLUDES ************************************************************************************/

#include <stdlib.h> /* getenv  */

#include "signals/signals.h" /* initSignal  */
#include "wiring/wiring.h"   /* wiringSetup */

/*************************************************************************************************/



/*** API IMPLEMENTATION **************************************************************************/

void wyliodrinSetup() {
  wiringSetup();

  char *val = getenv("wyliodrin_port");

  int port = 0;
  if (val != NULL) {
    port = atoi(val);
  }
  if (port == 0) {
    port = 6379;
  }

  char *projectId = getenv("wyliodrin_project");
  char *sessionId = getenv("wyliodrin_session");
  char *userid    = getenv("wyliodrin_userid");

  initSignal(port, projectId, sessionId, userid);
}

/*************************************************************************************************/
