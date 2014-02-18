%module arduino
%{
#include <Arduino.h>
%}

%apply unsigned char { uint8_t };
%apply unsigned int { uint32_t};

extern void wiringSetup ();
extern void pinMode (uint8_t pin, uint8_t mode);
extern void digitalWrite (uint8_t pin, uint8_t value);
extern uint8_t digitalRead (uint8_t pin);
extern uint32_t analogRead (uint8_t pin);
extern void analogWrite (uint8_t pin, uint32_t value);
extern void delay (unsigned long value);
extern void delayMicroseconds (unsigned long value);
extern unsigned long micros();
extern unsigned long millis();
extern void shiftOut (uint8_t dataPin, uint8_t clockPin, uint8_t order, uint8_t value);
extern uint8_t shiftIn (uint8_t dataPin, uint8_t clockPin, uint8_t order);
 