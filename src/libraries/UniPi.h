#ifndef UNIPI_H
#define UNIPI_H

void unipiSetup ();
void unipiRelay (int relay, int value);
void unipiAnalogWrite (int value);
int unipiAnalogRead (int pin);
int unipiDigitalRead (int pin);

#endif
