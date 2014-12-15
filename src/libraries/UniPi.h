#ifndef UNIPI_H
#define UNIPI_H

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

void unipiSetup ();
void unipiRelay (int relay, int value);
void unipiAnalogWrite (int value);
int unipiAnalogRead (int pin);
int unipiDigitalRead (int pin);

#ifdef __cplusplus /* If this is a C++ compiler, end C linkage */
}
#endif

#endif
