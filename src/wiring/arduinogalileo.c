/**************************************************************************************************
 * Wiring functions for Arduino Galileo and Intel Edison
 *
 * CONTENT
 * 1. Pins
 * 2. Digital
 * 3. Analog
 * 4. Time
 * 5. Shift
 * 6. SPI
 * 7. I2C
 * 8. Serial
 *************************************************************************************************/

#if defined(ARDUINOGALILEO) || defined (EDISON)

#include "wiring.h"
#include <pthread.h>
#include <math.h>

static mraa_gpio_context gpio_pins [MAX_GPIO_PINS];
static mraa_aio_context  aio_pins  [MAX_AIO_PINS];
static mraa_pwm_context  pwm_pins  [MAX_PWM_PINS];
static mraa_spi_context  spi_buses [MAX_SPI_BUSES];
static mraa_i2c_context  i2c_buses [MAX_I2C_BUSES];

#define pinValue(pin)  (pin = (pin >= 14) ? (pin - 14) : pin)
#define isGpioPin(pin) (gpio_pins[pin] != NULL)
#define isAioPin(pin)  (aio_pins[pin] != NULL)
#define isPwmPin(pin)  (pwm_pins[pin] != NULL)
#define inPin(pin)     do { if(!isGpioPin(pin)) pinMode(pin, INPUT);  } while (0);
#define outPin(pin)    do { if(!isGpioPin(pin)) pinMode(pin, OUTPUT); } while (0);

pthread_mutex_t lockspi;
pthread_mutex_t locki2c;

int adc_raw_bits = 0;
int adc_power = 1;



/**************************************************************************************************
 * 1. Pins
 *************************************************************************************************/

void resetPin(int pin);
void pwmReset(int pin);

/**
 * Initial wiring setup
 */
int wiringSetup()
{
  mraa_init ();
  adc_raw_bits = mraa_adc_supported_bits();
  adc_power = pow(2, adc_raw_bits) - 1;
  pthread_mutex_init(&lockspi, NULL);
  pthread_mutex_init(&locki2c, NULL);
}

/**
 * Reset pin
 */
void pinReset(int pin)
{
  pinMode(pin, OUTPUT);
  resetPin(pin);
  pwmReset(pin);
}

/**
 * Reset PWM pin
 */
void pwmReset(pin)
{
  mraa_pwm_context p = mraa_pwm_init(pin);

  if(p) 
  {
    mraa_pwm_enable(p, 0);
    mraa_pwm_close(p);
  }
  else
  {
    perror ("pwmReset");
  }
}

/**
 * Reset pin
 */
void resetPin(int pin)
{
  if (gpio_pins[pin] != NULL)
  {
    mraa_gpio_close(gpio_pins[pin]);
    gpio_pins[pin] = NULL;
  }
  if (pwm_pins[pin] != NULL)
  {
    mraa_pwm_close(pwm_pins[pin]);
    pwm_pins[pin] = NULL;
  }
}

/**
 * Set pin mode
 */
void pinMode(int pin, int mode)
{
  if (!isGpioPin(pin))
  {
    resetPin(pin);
    gpio_pins[pin] = mraa_gpio_init(pin);
  }
  if(isGpioPin(pin))
  {
    if(mode == INPUT)
    {
      mraa_gpio_dir(gpio_pins[pin], MRAA_GPIO_IN);
    }
    else if(mode == OUTPUT)
    {
      mraa_gpio_dir(gpio_pins[pin], MRAA_GPIO_OUT);
    }
  }
  else
  {
    perror ("pinMode");
  }
}



/**************************************************************************************************
 * 2. Digital
 *************************************************************************************************/

/**
 * Read digital value on pin
 */
int digitalRead (int pin)
{
  inPin(pin);
  return mraa_gpio_read(gpio_pins[pin]);
}

/**
 * Write digital value on pin
 */
void digitalWrite(int pin, int value)
{
  outPin(pin);
  mraa_gpio_write(gpio_pins[pin], value);
}



/**************************************************************************************************
 * 3. Analog
 *************************************************************************************************/

/**
 * Read analog value from pin
 */
int analogRead(int pin)
{
  pinValue(pin);

  if(!isAioPin(pin))
  {
    resetPin (pin);
    aio_pins[pin] = mraa_aio_init(pin);
  }
  if(isAioPin (pin))
  {
    int adc = mraa_aio_read(aio_pins[pin]);
    if(adc_raw_bits != 10)
    {
      return (int)((float)adc*1023/adc_power);
    }
    return adc;
  }
  else
  {
    perror("analogRead");
    return 0;
  }
}

/**
 * Write analog value on pin. Value must be in [0, 255] interval
 */
void analogWrite(int pin, int value)
{
  if(!isPwmPin(pin))
  {
    resetPin(pin);
    pwm_pins[pin] = mraa_pwm_init(pin);
    if (pwm_pins[pin])
    {
      mraa_pwm_period_us(pwm_pins[pin], 1200);
      mraa_pwm_enable(pwm_pins[pin], 1);
    }
  }
  if(pwm_pins[pin])
  {
    mraa_pwm_write(pwm_pins[pin], value/255.0);
  }
  else
  {
    perror ("analogWrite");
  }
}



/**************************************************************************************************
 * 4. Time
 *************************************************************************************************/

/**
 * Return system time in milliseconds
 */
unsigned int millis()
{
  return micros() / 1000;
}

/**
 * Return system time in microseconds
 */
unsigned int micros()
{
  struct timespec t;
  t.tv_sec = t.tv_nsec = 0;

  clock_gettime(CLOCK_REALTIME, &t);
  return (unsigned long)(t.tv_sec)*1000000L + t.tv_nsec / 1000L ;
}

/**
 * Delay execution in milliseconds
 */
void delay(unsigned int milliseconds)
{
  usleep(milliseconds*1000);
}

/**
 * Delay execution in milcroseconds
 */
void delayMicroseconds (unsigned int microseconds)
{
  usleep(microseconds);
}



/**************************************************************************************************
 * 5. Shift
 *************************************************************************************************/

/**
 * Reads a pulse (either HIGH or LOW) on a pin. 
 *
 * For example, if <state> is HIGH, pulseIn() waits for the pin to go HIGH, starts timing,
 * then waits for the pin to go LOW and stops timing. 
 *
 * Returns the length of the pulse in microseconds. Gives up and returns 0 if no pulse 
 * starts within 1 second;
 */
unsigned long pulseIn(uint8_t pin, uint8_t state)
{
  long timeout = 1000000L;
  int timeoutFlag = 0;
  
  unsigned long time_a;
  unsigned long time_b;
  unsigned long timeBFlag = 0;
  unsigned long time_c;

  time_a = micros();
  while(digitalRead(pin) == state)
  {
    printf("%d ", digitalRead(pin));
    time_b = micros();
    timeBFlag = 1;
    if(time_b >= time_a)
    {
      if((time_b - time_a) > timeout)
      {
        timeoutFlag = 1;
        break;
      }
    }
    else
    {
      time_c = (0xFFFFFFFF - time_a) + time_b;
      if(time_c > timeout)
      {
        timeoutFlag = 1;
        break;
      }
    }
  }

  while((digitalRead(pin) != state) && !timeoutFlag)
  {
    printf ("%d ", digitalRead(pin));
    time_b = micros();
    timeBFlag = 1;
    if(time_b >= time_a)
    {
      if((time_b - time_a)>timeout)
      {
        timeoutFlag = 1;
        break;
      }
    }
    else
    {
      time_c = (0xFFFFFFFF - time_a) + time_b;
      if(time_c > timeout)
      {
        timeoutFlag = 1;
        break;
      }
    }
  }

  if((digitalRead(pin) == state) && !timeoutFlag)
  {
    printf ("%d ", digitalRead(pin));
    if(!timeBFlag)
    {
      time_b = micros();
    }
    time_a = time_b;
    while(digitalRead(pin) == state)
    {
    }
    time_b = micros();
    if(time_b > time_a)
    {
      return (time_b - time_a - 1);
    }
    else
    {
      time_c = (0xFFFFFFFF - time_a) + time_b - 1;
      return time_c;
    }
  }

  else
  {
    return 0;
  }
}

/**
 * Shifts in a byte of data one bit at a time. Starts from either the most (i.e. the leftmost) or 
 * least (rightmost) significant bit. For each bit, the clock pin is pulled high, the next bit is
 * read from the data line, and then the clock pin is taken low.
 * If you're interfacing with a device that's clocked by rising edges, you'll need to make sure
 * that the clock pin is low before the first call to shiftIn(), e.g. with a call to 
 * digitalWrite(clockPin, LOW).
 */
uint8_t shiftIn(uint8_t dPin, uint8_t cPin, uint8_t order)
{
  int8_t i;
  uint8_t value = 0;
 
  if (order == MSBFIRST)
  {
    for (i = 7; i >= 0; --i)
    {
      digitalWrite(cPin, HIGH) ;
      value |= digitalRead (dPin) << i ;
      digitalWrite(cPin, LOW) ;
    }
  }
  else
  {
    for (i = 0; i < 8; ++i)
    {
      digitalWrite(cPin, HIGH) ;
      value |= digitalRead(dPin) << i;
      digitalWrite(cPin, LOW) ;
    }
  }

  return value;
}

/**
 * Shifts out a byte of data one bit at a time. Starts from either the most (i.e. the leftmost)
 * or least (rightmost) significant bit. Each bit is written in turn to a data pin, after which a
 * clock pin is pulsed (taken high, then low) to indicate that the bit is available.
 */
void shiftOut(uint8_t dPin, uint8_t cPin, uint8_t order, uint8_t val)
{
  int8_t i;

  if (order == MSBFIRST)
  {
    for (i = 7; i >= 0; --i)
    {
      digitalWrite (dPin, val & (1 << i)) ;
      digitalWrite (cPin, HIGH) ;
      digitalWrite (cPin, LOW) ;
    }
  }
  else
  {
    for (i = 0; i < 8; ++i)
    {
      digitalWrite (dPin, val & (1 << i)) ;
      digitalWrite (cPin, HIGH) ;
      digitalWrite (cPin, LOW) ;
    }
  }
}



/**************************************************************************************************
 * 6. SPI
 *************************************************************************************************/

/**
 * Return SPI id
 */
int getSPIId()
{
  int i;
  int id = -1;

  pthread_mutex_lock(&lockspi);
  for (i = 0; i < MAX_SPI_BUSES && id == -1; i++)
  {
    if (spi_buses[i] == NULL)
    {
      id = i;
      spi_buses[id] = (void*)1;
    }
  }
  pthread_mutex_unlock(&lockspi);

  return id;
}

/**
 * Release SPI id
 */
void releaseSPIId(int id)
{
  pthread_mutex_lock(&lockspi);
  spi_buses[id] = NULL;
  pthread_mutex_unlock(&lockspi);
}

/**
 * TODO
 */
int spi_getadapter(uint32_t spi_bus_address)
{
  return -1;
}

/**
 * Open SPI adapter
 */
int spi_openadapter(uint8_t spi_bus)
{
  int spiId = getSPIId();

  if(spiId >= 0)
  {
    spi_buses[spiId] = mraa_spi_init(spi_bus);
  }

  return spiId;
}

/**
 * Close SPI adapter
 */
int spi_closeadapter(int spiId)
{
  mraa_spi_stop(spi_buses[spiId]);
  releaseSPIId(spiId);

  return 0;
}

/**
 * Set SPI mode
 */
int spi_setmode(int spiId, unsigned short mode)
{
  return mraa_spi_mode(spi_buses[spiId], mode);
}

/**
 * Set SPI frequency
 */
int spi_set_frequency(int spiId, int freq)
{
  return mraa_spi_frequency(spi_buses[spiId], freq);
}

/**
 * Write byte on SPI bus
 */
uint8_t spi_writebyte(int spiId, uint8_t byte)
{
  return mraa_spi_write(spi_buses[spiId], byte);
}

/** 
 * Write bytes on SPI bus
 */
unsigned char *spi_writebytes(int spiId, uint8_t *bytes, uint8_t length)
{
  return mraa_spi_write_buf(spi_buses[spiId], bytes, length);
}

/**
 * Set least significant bit mode
 */
int spi_lsb_mode(int spiId, unsigned char lsb)
{
  return mraa_spi_lsbmode(spi_buses[spiId], lsb);  
}

/**
 * Set number of bits per word
 */
int spi_bit_per_word(int spiId, unsigned int bits)
{
  return mraa_spi_bit_per_word(spi_buses[spiId], bits);
}



/**************************************************************************************************
 * 7. I2C
 *************************************************************************************************/

/**
 * Get I2C id
 */
int getI2CId()
{
  int i;
  int id = -1;

  pthread_mutex_lock(&locki2c);
  for (i=0; i < MAX_I2C_BUSES && id == -1; i++)
  {
    if (i2c_buses[i] == NULL)
    {
      id = i;
      i2c_buses[id] = (void*)1;
    }
  }
  pthread_mutex_unlock(&locki2c);

  return id;
}

/**
 * Release I2C id
 */
void releaseI2CId(int id)
{
  pthread_mutex_lock(&locki2c);
  i2c_buses[id] = NULL;
  pthread_mutex_unlock(&locki2c);
}

/**
 * TODO
 */
int i2c_getadapter(uint32_t i2c_bus_address)
{
  return -1;
}

/**
 * TODO
 */
int i2c_add_to_buf(uint8_t addr, uint8_t rw, uint8_t *value, int length)
{
  return -1;
}

/**
 * Open I2C adapter
 */
int i2c_openadapter(uint8_t i2c_bus)
{
  int i2cId = getI2CId ();

  if (i2cId >= 0)
  {
    if (i2c_bus == 255)
    {
      #ifdef ARDUINOGALILEO
      i2c_bus = 0;
      #endif
      #ifdef EDISON
      i2c_bus = 6;
      #endif
    }
    i2c_buses[i2cId] = mraa_i2c_init(i2c_bus);
  }

  return i2cId;
}

/**
 * Close I2C adapter
 */
int i2c_closeadapter(int i2cId)
{
  mraa_i2c_stop (i2c_buses[i2cId]);
  releaseI2CId (i2cId);
  return 0;
}

/**
 * Set I2C slave
 */
int i2c_setslave(int i2cId, uint8_t addr)
{
  return mraa_i2c_address (i2c_buses[i2cId], addr);
}

/**
 * Write byte on I2C bus
 */
int i2c_writebyte(int i2cId, uint8_t byte)
{
  return mraa_i2c_write_byte(i2c_buses[i2cId], byte);
}

/**
 * Write bytes on I2C bus
 */
int i2c_writebytes(int i2cId, uint8_t *bytes, uint8_t length)
{
  return mraa_i2c_write(i2c_buses[i2cId], bytes, length);
}

/**
 * Read byte from I2C bus
 */
int i2c_readbyte(int i2cId)
{
  return mraa_i2c_read_byte(i2c_buses[i2cId]); 
}

/**
 * Read bytes from I2C bys
 */
int i2c_readbytes(int i2cId, uint8_t *buf, int length)
{
  return mraa_i2c_read(i2c_buses[i2cId], buf, length);
}

/**
 * TODO
 */
int i2c_readwrite(int i2cId)
{
  return -1;
} 



/**************************************************************************************************
 * 8. Serial
 *************************************************************************************************/

/**
 * TODO
 */
int getSerialId()
{
  return -1;
}

/**
 * TODO
 */
void releaseSerial(int id) {}

/**
 * TODO
 */
int serial_openadapter(char *serial_bus)
{
  return -1;
}

/**
 * TODO
 */
int serial_closeadapter(int serial_id)
{
  return 0;
}

/**
 * TODO
 */
int serial_set_speed(int serial_id, int baud)
{
  return 0;
}

/**
 * TODO
 */
int serial_bytes_available(int serial_id)
{
  return 0;
}

/**
 * TODO
 */
int serial_writebyte(int serial_id, uint8_t byte)
{
  return 0;
}

/**
 * TODO
 */
int serial_writebytes(int serial_id, uint8_t *bytes, uint8_t length)
{
  return 0;
}

/**
 * TODO
 */
uint8_t serial_readbyte(int serial_id)
{
  return -1;
}

/**
 * TODO
 */
int serial_readbytes(int serial_id, uint8_t *buf, int length)
{
  return 0;
}

/**
 * TODO
 */
int serial_flush(int serial_id)
{
  return 0;
}



#endif // defined(ARDUINOGALILEO) || defined (EDISON)
