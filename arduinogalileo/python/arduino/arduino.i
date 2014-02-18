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

// LCD
extern void lcdHome        (const int fd) ;
extern void lcdClear       (const int fd) ;
extern void lcdDisplay     (const int fd, int state) ;
extern void lcdCursor      (const int fd, int state) ;
extern void lcdCursorBlink (const int fd, int state) ;
extern void lcdSendCommand (const int fd, unsigned char command) ;
extern void lcdPosition    (const int fd, int x, int y) ;
extern void lcdCharDef     (const int fd, int index, unsigned char data [8]) ;
extern void lcdPutchar     (const int fd, unsigned char data) ;
extern void lcdPuts        (const int fd, const char *string) ;
extern void lcdPrintf      (const int fd, const char *message, ...) ;
extern int  lcdInit               (int rows, int cols, int bits, int rs, int strb,
    int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7) ;

 
