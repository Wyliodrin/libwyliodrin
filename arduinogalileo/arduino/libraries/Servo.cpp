#include "Servo.h"
#include <trace.h>

uint8_t Servo::counter = 0;   // init the counter here.. static...


Servo::Servo()
{
  if (counter < MAX_NUMBER_OF_SERVOS) {
    this->index = ++counter;                    // assign a servo index to this instance
    lastByteInDuty = -1;
  } else {
    this->index = INVALID_SERVO;  // too many servos
  }

}

void Servo::set48hz()
{
    if (this->is188hz)
    {
      // only changes if is different freq
       this->is188hz = false;
       writeMicroseconds(DEFAULT_PULSE_WIDTH);
    }
}

void Servo::set188hz()
{
    if (!this->is188hz)
    {
      // only changes if is different freq
      this->is188hz = true;
      writeMicroseconds(DEFAULT_PULSE_WIDTH);
    }
}

uint8_t Servo::attach(int16_t pin)
{
  
  return attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max)
{
  // need to validate the pin
  uint8_t list_index = 0;
  bool    is_valid_pin = false;

  // let's check the boundaries
  if (min < MIN_PULSE_WIDTH) min = MIN_PULSE_WIDTH;
  if (max > MAX_PULSE_WIDTH) max = MAX_PULSE_WIDTH;

  trace_debug("******* pin:%d min:%d max:%d\n", pin, min, max);

  for (list_index = 0; list_index < sizeof(pinData)/sizeof(servoPinData_t); list_index++) 
  {
      if (pinData[list_index].pin == pin) 
      {
	  is_valid_pin = true;
          break;
      }
  }

  if (!is_valid_pin)
  {
      trace_error("invalid pin");    
      return INVALID_SERVO;
  }

  if (this->index < MAX_NUMBER_OF_SERVOS) {

    // set as active
    pinData[list_index].isActive = true;
    this->pin = pin;
    this->min = min;
    this->max = max;
    this->is188hz = true;
    this->isAttached = true;

    pinMode(pin, OUTPUT);
    analogWrite(pin, 1);

    writeMicroseconds(DEFAULT_PULSE_WIDTH);

  }

  trace_debug("\nattached ok on pin:%d min:%d max:%d\n",pin, this->min, this->max);

  return this->index;
}


/* The "train" requested by servo motors is 
   the minimal of 20ms. Our PWM allow us to have
   41.7Hz that means 23.98 ms.
   However the cypress does not offer a good angle
   resolution on this frequency and the user
   has option to operates the servos in 188Hz */

void Servo::prepare_pin(uint8_t pin)
{
    
  extern TwoWire Wire;

    Wire.begin();

    // let's use this function only to select the bit port
    // the datasheet is a little confusing regading this set

    analogWrite(pin, 1);

    /* ToDo: set property and avoid analogWrite()
    Wire.beginTransmission(CYPRESS_I2C_ADDRESS);
    Wire.write(0x28);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.beginTransmission(CYPRESS_I2C_ADDRESS);
    Wire.write(0x18);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.beginTransmission(CYPRESS_I2C_ADDRESS);
    Wire.write(0x1A);
    Wire.write(0x03);
    Wire.endTransmission();
    */
    
    // Select programmable PWM CLK source to 367.7 Hz
    Wire.beginTransmission(CYPRESS_I2C_ADDRESS);
    Wire.write(0x29);
    Wire.write(0x04);
    Wire.endTransmission();
    
    
    // Rising edge register
    Wire.beginTransmission(CYPRESS_I2C_ADDRESS);
    Wire.write(0x2a);
    Wire.write(0xff);
    Wire.endTransmission();
    
    
    // Set divider to get 47.4Hz freq.
    Wire.beginTransmission(CYPRESS_I2C_ADDRESS);
    Wire.write(0x2C);

    if (this->is188hz)
       Wire.write(0x02);
    else
       Wire.write(0x09);

    Wire.endTransmission();
    
        
}

byte Servo::transform_cypress_duty_cycle_byte(int microsecs)
{
 
  /* the max division of 23ms (100% duty cycle) is 
     255 units (bytes 0 to 255 according regs 0x2B
     
     So, the maximum units for 2ms is 22.17 

     The principle it is used for 188Hz thar correspond
     a period of 5.319ms.

     So, the max byte available for different frequency
     is in maximum time of 2ms or 2.4ms in 8 bits resolution 
     is:

         = max_duty/1000 * (1/freq)/255
         = max_duty*255/(1000/freq)
     
     */
 
  int freq =  (this->is188hz) ? 188:43.4;
  int max_byte = MAX_PULSE_WIDTH*255*freq/1000000L;

  if(this->min=1000) Serial.println(max_byte);

  byte b_duty = map(microsecs, 0, MAX_PULSE_WIDTH, 0, max_byte);
  
  return b_duty;
}


void Servo::writeMicroseconds(int microsecs)
{
   
  int byteDuty = transform_cypress_duty_cycle_byte(microsecs);

  if (this->lastByteInDuty == byteDuty)
    return;

  this->lastByteInDuty = byteDuty;
  
  prepare_pin(this->pin);

  // checking the boundaries
  if (microsecs < this->min) microsecs = this->min;
  if (microsecs > this->max) microsecs = this->max;

  // Set duty cycle
  Wire.beginTransmission(CYPRESS_I2C_ADDRESS);
  Wire.write(0x2b);
  Wire.write(byteDuty);
  Wire.endTransmission();

  // update last microseconds passed
  this->usecs = microsecs;
  
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

    trace_debug("it is an angle:%d  this->min:%d  this->max:%d\n", val, this->min, this->max);
    writeMicroseconds(map(val, MIN_ANGLE, MAX_ANGLE, this->min, this->max));
  }
  else
  {
    trace_debug("it is microseconds:%d\n", val);
    // actually angle on this case it is microsencods 
    writeMicroseconds(val);
  }
}

int Servo::read()
{
  return map(this->usecs, this->min, this->max, MIN_ANGLE, MAX_ANGLE);
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
        pinMode(this->pin, OUTPUT);
        this->lastByteInDuty = -1;
    }

}
