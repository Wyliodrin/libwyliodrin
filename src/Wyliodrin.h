#ifndef WYLIODRIN_H
#define WYLIODRIN_H

#define RASPBERRYPI 0
#define GALILEO 1
#define EDISON 2

#ifdef RASPBERRYPI
#define BOARD RASPBERRYPI
#endif

#ifdef GALILEO
#define BOARD GALILEO
#endif

#ifdef EDISON
#define BOARD EDISON
#endif

#include <stdlib.h>

// Signals library
#include <Wyliodrin/signals.h>

// Include the wiring library
#include <Wyliodrin/wiring.h>

// Plugins
#include <Wyliodrin/lcd.h>

#ifdef __cplusplus
extern "C" {
#endif

// Intialize wyliodrin
void wyliodrinSetup (void);

//  send signal value

#ifdef __cplusplus
}
#endif

#endif // WYLIODRIN_H

