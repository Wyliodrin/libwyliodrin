#ifndef Servo_h
#define Servo_h

#define MIN_ANGLE              0      // min angle
#define MAX_ANGLE            180      // max angle


#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo  
#define MAX_PULSE_WIDTH      2000     // the longest pulse sent to a servo 
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached

#define INVALID_SERVO        255

/* How the pins are connected to cypress in FabD */

#define MAX_NUMBER_OF_SERVOS  6   // same number of pins in PWM

#define MY_TRACE_PREFIX "ServoX86Lib"

#include <stdint.h>

#ifdef ARDUINOGALILEO
#include <mraa/pwm.h>

#define MAX_PERIOD    7968
#endif

typedef uint8_t byte;

class Servo
{
public:
  Servo();
  uint8_t attach(int pin);           // attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
  uint8_t attach(int pin, int min, int max); // as above but also sets min and max values for writes. 
  void detach();
  void write(int val);               // if value is < 200 its treated as an angle, otherwise as pulse width in microseconds 
  void writeMicroseconds(int value); // Write pulse width in microseconds 
  int read();                        // returns current pulse width as an angle between 0 and 180 degrees
  int readMicroseconds();            // returns current pulse width in microseconds for this servo (was read_us() in first release)
  bool attached();                   // return true if this servo is attached, otherwise false 
  void set48hz();                    // forces cypress to work in 47.8 hertz
  void set188hz();                   // forces cypress to work in 188 hertz (better angle resolution)
  
private:

   int min;                       // minimum is this value times 4 added to MIN_PULSE_WIDTH    
   int max;                       // maximum is this value times 4 added to MAX_PULSE_WIDTH   

   uint8_t index;                 // servo index


   static uint8_t counter;   // only for counting 

   int usecs;
   bool isAttached;
   byte pin;
   bool is188hz;
   int lastByteInDuty;          // to avoid jitter caused by analogWrite()
   int calcPulseTraveling (int value);
   #ifdef ARDUINOGALILEO
   int m_currentAngle;
   mraa_pwm_context m_pwmServoContext;
   #endif
};

#endif
