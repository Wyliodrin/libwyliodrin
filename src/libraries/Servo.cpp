#include "Servo.h"
#include "../wiring/wiring.h"
#include "../wiring/udooConfig.h"
#include <stdio.h>

#define trace_debug printf
#define trace_error printf

uint8_t Servo::counter = 0;   // init the counter here.. static...

// servoPinData_t Servo::pinData[6] = {{3,  GPORT0_BIT4_PWM7, false},
//                                     {5,  GPORT0_BIT1_PWM5, false},
//                                     {6,  GPORT1_BIT0_PWM6, false},
//                                     {9,  GPORT0_BIT3_PWM1, false},
//                                     {10, GPORT0_BIT0_PWM7, false},
//                                     {11, GPORT1_BIT1_PWM4, false}};

Servo::Servo()
{
  if (counter < MAX_NUMBER_OF_SERVOS) {
    this->index = ++counter;                    // assign a servo index to this instance
    lastByteInDuty = -1;
  } else {
    this->index = INVALID_SERVO;  // too many servos
  }
  #ifdef ARDUINOGALILEO
  m_currentAngle = 180;
  #endif
}

void Servo::set48hz()
{
    if (this->is188hz)
    {
      // only changes if is different freq
       this->is188hz = false;
       writeMicroseconds(90);
    }
}

void Servo::set188hz()
{
    if (!this->is188hz)
    {
      // only changes if is different freq
      this->is188hz = true;
      writeMicroseconds(90);
    }
}

uint8_t Servo::attach(int pin)
{
  return attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max)
{

  // let's check the boundaries
  if (min < MIN_PULSE_WIDTH) min = MIN_PULSE_WIDTH;
  if (max > MAX_PULSE_WIDTH) max = MAX_PULSE_WIDTH;

  if (this->index < MAX_NUMBER_OF_SERVOS) {

    // set as active
    this->pin = pin;
    this->min = min;
    this->max = max;
    this->is188hz = true;
    this->isAttached = true;

    #ifdef ARDUINOGALILEO
      m_pwmServoContext = mraa_pwm_init (this->pin);
      write (0);
    #endif

    #ifdef UDOO
      if (is_firmata_defined) {
        if (!firmata || !firmata->isReady)
            perror("servo.attach: firmata is not ready yet");
        else {
            firmata_pinMode(firmata, this->pin, MODE_SERVO);  
        }
      }
      else {
        printf(" Sorry, you do not have access to Servo()\n");
        printf(" In order to use Servo() functions, you must first enable Firmata\n");
        return -1;
      }
    #endif
  }

  //trace_debug("\nattached ok on pin:%d min:%d max:%d\n",pin, this->min, this->max);

  return this->index;
}

int Servo::calcPulseTraveling (int value) {
    // if bigger than the boundaries
    if (value > 180) {
        return max;
    }

    // if less than the boundaries
    if (value  < 0) {
        return min;
    }

    // the conversion
    return (int) ((float)min + ((float)value / 180) * ((float)max - (float)min));
}

void Servo::writeMicroseconds(int microsecs)
{
  printf ("Write microseconds is not supported\n");
}

void Servo::write(int val)
{

  // according to Arduino reference lib, if this angle will
  // be bigger than 200, it should be considered as microsenconds

  if (val < MIN_PULSE_WIDTH)
  {
    // yeah.. user is passing angles

    if (val  < 0)
      val  = 0;
    else if (val > 180)
      val = 180;

    #ifdef ARDUINOGALILEO
      int period = (max - min) / 180;
      
      int cycles = (int)(100.0 * ((float)abs (m_currentAngle - val) / 180));

      // int cycles = (int)(100.0 * ((float)angle / (float)m_maxAngle));

      mraa_pwm_enable (m_pwmServoContext, 1);
      for (int cycle = 0; cycle < cycles; cycle++) {
          mraa_pwm_period_us (m_pwmServoContext, MAX_PERIOD);
          mraa_pwm_pulsewidth_us (m_pwmServoContext, calcPulseTraveling(val));
      }
      mraa_pwm_enable (m_pwmServoContext, 0);

      // std::cout << "angle = " << angle << " ,pulse = " << calcPulseTraveling(angle) << ", cycles " << cycles << std::endl;
      
      m_currentAngle = val;
  #endif

  #ifdef UDOO
    if (is_firmata_defined) 
      if (val < 256)
        firmata_analogWrite(firmata, this->pin, val);
    else {
      printf(" Sorry, you do not have access to Servo\n");
      printf(" In order to access Servo() functions, you must first enable Firmata\n");
      return;
    }
  #endif

  }
  else
  {
    //trace_debug("it is microseconds:%d\n", val);
    // actually angle on this case it is microsencods 
    writeMicroseconds(val);
  }
}

int Servo::read()
{
  #ifdef ARDUINOGALILEO
  return this->m_currentAngle;
  #else
  return 0;
  #endif
}

int Servo::readMicroseconds()
{
  return this->usecs;
}

bool Servo::attached()
{
  return this->isAttached;
}


void Servo::detach()
{
    if (this->isAttached)
    {
        this->isAttached = false;        
        #ifdef UDOO
            if (is_firmata_defined)
                firmata_pinMode(firmata, this->pin, MODE_OUTPUT);
        #endif

        #ifndef UDOO
        pinMode(this->pin, OUTPUT);
        #endif
        this->lastByteInDuty = -1;
    }

}
