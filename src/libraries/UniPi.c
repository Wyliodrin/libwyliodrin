
#include "UniPi.h"

#ifdef RASPBERRYPI
#include <mcp23008.h>
#include <mcp3422.h>

#define MCP23008_PIN  3000
#define MCP3422_PIN   3200

int DIO[] = {7, 0, 2, 4, 3, 5, 14, 11, 10, 13, 6, 12};

#endif

#ifdef __cplusplus /* If this is a C++ compiler, end C linkage */
}
#endif

void unipiSetup ()
{
  #ifdef RASPBERRYPI
  mcp23008Setup (MCP23008_PIN, 0x20);
  // mcp3422Setup (MCP3422_PIN, 0x86, 0, 0);
  #endif
}

void unipiRelay (int relay, int value)
{
  #ifdef RASPBERRYPI
  printf ("relay %d\n", MCP23008_PIN+(8-relay));
  digitalWrite (MCP23008_PIN+(8-relay), value);
  #endif
}

void unipiAnalogWrite (int value)
{
  #ifdef RASPBERRYPI
  analogWrite (1, value);
  #endif
}

int unipiAnalogRead (int pin)
{
  #ifdef RASPBERRYPI
  return analogRead (MCP3422_PIN+pin);
  #else
  return 0;
  #endif
}

int unipiDigitalRead (int pin)
{
  #ifdef RASPBERRYPI
  return digitalRead (DIO[pin-1]); 
  #else
  return 0;
  #endif
}

#ifdef __cplusplus /* If this is a C++ compiler, end C linkage */
}
#endif
