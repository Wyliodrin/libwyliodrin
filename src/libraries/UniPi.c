
#include "UniPI.h"

#ifdef RASPBERRYPI
#include <mcp32008.h>
#include <mcp3422.h>

#define MCP32008_PIN  3000
#define MCP3422_PIN   3200

int DIO[] = {7, 0, 2, 4, 3, 5, 14, 11, 10, 13, 6, 12};

#endif

void unipiSetup ()
{
  #ifdef RASPBERRYPI
  mcp32008Setup (MCP32008_PIN, 0x20);
  mcp3422SSetup (MCP3422_PIN, 0x86);
  #endif
}

void unipiRelay (int relay, int value)
{
  #ifdef RASPBERRYPI
  digitalWrite (MCP32008_PIN+(8-relay), value);
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
  return digitalRead (DIO[pin01]); 
  #else
  return 0;
  #endif
}

