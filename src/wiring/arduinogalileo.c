
#if defined(ARDUINOGALILEO) || defined (EDISON)

#include "wiring.h"
#include <pthread.h>
#include <math.h>

static mraa_gpio_context gpio_pins[MAX_GPIO_PINS];
static mraa_aio_context aio_pins[MAX_AIO_PINS];
static mraa_pwm_context pwm_pins[MAX_PWM_PINS];
static mraa_spi_context spi_buses[MAX_SPI_BUSES];
static mraa_i2c_context i2c_buses[MAX_I2C_BUSES];

// struct i2c_msg {
// 	__u16 addr;	/* slave address			*/
// 	unsigned short flags;		
// #define I2C_M_TEN	0x10	/* we have a ten bit chip address	*/
// #define I2C_M_RD	0x01
// #define I2C_M_NOSTART	0x4000
// #define I2C_M_REV_DIR_ADDR	0x2000
// #define I2C_M_IGNORE_NAK	0x1000
// #define I2C_M_NO_RD_ACK		0x0800
// 	short len;		/* msg length				*/
// 	char *buf;		/* pointer to msg data			*/
// };

// struct i2c_msg i2c_buf[2];
// unsigned int i2c_buf_count = 0;

pthread_mutex_t lockspi;
pthread_mutex_t locki2c;

int adc_raw_bits = 0;
int adc_power = 1;

int getSerialId()
{
	return -1;
}

void releaseSerial(int id)
{
	
}

int getSPIId ()
{
	int i;
	int id = -1;
	pthread_mutex_lock(&lockspi);
	for (i=0; i < MAX_SPI_BUSES && id == -1; i++)
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

void releaseSPIId (int id)
{
	pthread_mutex_lock(&lockspi);
	spi_buses[id] = NULL;
	pthread_mutex_unlock(&lockspi);
}

int getI2CId ()
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

void releaseI2CId (int id)
{
	pthread_mutex_lock(&locki2c);
	i2c_buses[id] = NULL;
	pthread_mutex_unlock(&locki2c);
}

void resetPin (int pin);
void pwmReset (int pin);

int wiringSetup ()
{
	mraa_init ();
	adc_raw_bits = mraa_adc_raw_bits();
	adc_power = pow (2, adc_raw_bits)-1;
	pthread_mutex_init(&lockspi, NULL);
	pthread_mutex_init(&locki2c, NULL);
}

void pinReset (int pin)
{
	pinMode (pin, OUTPUT);
	resetPin (pin);
	pwmReset (pin);
}

void pwmReset (pin)
{
	mraa_pwm_context p = mraa_pwm_init (pin);
	if (p) 
	{
		mraa_pwm_enable (p, 0);
		mraa_pwm_close (p);
	}
	else
	{
		perror ("pwmReset");
	}
}

void resetPin (int pin)
{
	if (gpio_pins[pin] != NULL)
	{
		mraa_gpio_close (gpio_pins[pin]);
		gpio_pins[pin] = NULL;
	}
	if (pwm_pins[pin] != NULL)
	{
		mraa_pwm_close (pwm_pins[pin]);
		pwm_pins[pin] = NULL;
	}
	// if (aio_pins[pin] != NULL)
	// {
	// 	mraa_aio_close (aio_pins[pin]);
	// 	aio_pins[pin] = NULL;
	// }
}

#define pinValue(pin)  (pin = (pin >= 14)?(pin - 14):pin)

#define isGpioPin(pin)   (gpio_pins[pin] != NULL)
#define isAioPin(pin)   (aio_pins[pin] != NULL)
#define isPwmPin(pin)   (pwm_pins[pin] != NULL)
#define inPin(pin) do { if (!isGpioPin(pin)) pinMode (pin, INPUT); } while (0);
#define outPin(pin) do { if (!isGpioPin(pin)) pinMode (pin, OUTPUT); } while (0);

void pinMode (int pin, int mode)
{
	if (!isGpioPin(pin))
	{
		resetPin (pin);
		gpio_pins[pin] = mraa_gpio_init (pin);
	}
	if (isGpioPin(pin))
	{
		if (mode == INPUT)
		{
			mraa_gpio_dir (gpio_pins[pin], MRAA_GPIO_IN);
		}
		else if (mode == OUTPUT)
		{
			mraa_gpio_dir (gpio_pins[pin], MRAA_GPIO_OUT);
		}
	}
	else
	{
		perror ("pinMode");
	}
}

void digitalWrite (int pin, int value)
{
	outPin (pin);
	mraa_gpio_write (gpio_pins[pin], value);
}

int digitalRead (int pin)
{
	inPin (pin);
	return mraa_gpio_read (gpio_pins[pin]);
}

void analogWrite (int pin, int value)
{
	if (!isPwmPin (pin))
	{
		resetPin (pin);
		pwm_pins[pin] = mraa_pwm_init (pin);
		if (pwm_pins[pin])
		{
			mraa_pwm_period_us(pwm_pins[pin], 1200);
			mraa_pwm_enable (pwm_pins[pin], 1);
		}
	}
	if (pwm_pins[pin])
	{
		mraa_pwm_write (pwm_pins[pin], value/255.0);
	}
	else
	{
		perror ("analogWrite");
	}
}

int analogRead (int pin)
{
	pinValue (pin);
	// printf ("%p\n", aio_pins[pin]);
	if (!isAioPin (pin))
	{
		resetPin (pin);
		aio_pins[pin] = mraa_aio_init (pin);
		// printf ("%p\n", aio_pins[pin]);
	}
	if (isAioPin (pin))
	{
		int adc = mraa_aio_read (aio_pins[pin]);
		if (adc_raw_bits != 10)
		{
			return (int)((float)adc*1023/adc_power);
		}
		return adc;
	}
	else
	{
		perror ("analogRead");
		return 0;
	}
}

void delay (unsigned int milliseconds)
{
	usleep (milliseconds*1000);
}

void delayMicroseconds (unsigned int microseconds)
{
	usleep (microseconds);
}

unsigned int millis()
{
   return micros() / 1000;
}


unsigned int micros()
{

  struct timespec t;
  t.tv_sec = t.tv_nsec = 0;

  clock_gettime(CLOCK_REALTIME, &t);
  return (unsigned long)(t.tv_sec)*1000000L + t.tv_nsec / 1000L ;

}

unsigned long pulseIn(uint8_t pin, uint8_t state)
{
  long timeout = 1000000L;
  int timeoutFlag = 0;
  
  unsigned long time_a;
  unsigned long time_b;
  unsigned long timeBFlag = 0;
  unsigned long time_c;

  time_a = micros();
  
  //wait for previous pulse to end or timeout
  while(digitalRead(pin) == state)
  {
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
    {//micros() overflow
      time_c = (0xFFFFFFFF - time_a) + time_b;
      if(time_c > timeout)
      {
        timeoutFlag = 1;
        break;
      }
    }
  }

  //wait for pin to go to target value or timeout
  while((digitalRead (pin) != state) && !timeoutFlag)
  {
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
    {//micros() overflow
      time_c = (0xFFFFFFFF - time_a) + time_b;
      if(time_c > timeout)
      {
        timeoutFlag = 1;
        break;
      }
    }
  }

  //determine pulse length
  if((digitalRead(pin) == state) && !timeoutFlag)
  {
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
    {//micros() overflow
      time_c = (0xFFFFFFFF - time_a) + time_b - 1;
      return time_c;
    }
  }
  else
  {
    return 0;
  }
}

uint8_t shiftIn (uint8_t dPin, uint8_t cPin, uint8_t order)
{
  uint8_t value = 0 ;
  int8_t  i ;
 
  if (order == MSBFIRST)
    for (i = 7 ; i >= 0 ; --i)
    {
      digitalWrite (cPin, HIGH) ;
      value |= digitalRead (dPin) << i ;
      digitalWrite (cPin, LOW) ;
    }
  else
    for (i = 0 ; i < 8 ; ++i)
    {
      digitalWrite (cPin, HIGH) ;
      value |= digitalRead (dPin) << i ;
      digitalWrite (cPin, LOW) ;
    }

  return value;
}

void shiftOut (uint8_t dPin, uint8_t cPin, uint8_t order, uint8_t val)
{
  int8_t i;

  if (order == MSBFIRST)
    for (i = 7 ; i >= 0 ; --i)
    {
      digitalWrite (dPin, val & (1 << i)) ;
      digitalWrite (cPin, HIGH) ;
      digitalWrite (cPin, LOW) ;
    }
  else
    for (i = 0 ; i < 8 ; ++i)
    {
      digitalWrite (dPin, val & (1 << i)) ;
      digitalWrite (cPin, HIGH) ;
      digitalWrite (cPin, LOW) ;
    }
}

int spi_getadapter(uint32_t spi_bus_address)
{
	// error, function not implemented
	return -1;
}

int spi_openadapter(uint8_t spi_bus)
{
	int spiId = getSPIId ();
	if (spiId >= 0)
	{
		spi_buses[spiId] = mraa_spi_init (spi_bus);
	}
	return spiId;
}

int spi_setmode(int spiId, unsigned short mode)
{
	return mraa_spi_mode (spi_buses[spiId], mode);
}

int spi_set_frequency(int spiId, int freq)
{
	return mraa_spi_frequency (spi_buses[spiId], freq);
}

uint8_t spi_writebyte(int spiId, uint8_t byte)
{
	return mraa_spi_write (spi_buses[spiId], byte);
}

unsigned char * spi_writebytes(int spiId, uint8_t *bytes, uint8_t length)
{
	return mraa_spi_write_buf (spi_buses[spiId], bytes, length);
}

int spi_lsb_mode(int spiId, unsigned char lsb)
{
	return mraa_spi_lsbmode (spi_buses[spiId], lsb);	
}

int spi_bit_per_word(int spiId, unsigned int bits)
{
	return mraa_spi_bit_per_word (spi_buses[spiId], bits);
}

int spi_closeadapter (int spiId)
{
	mraa_spi_stop (spi_buses[spiId]);
	releaseSPIId (spiId);
	return 0;
}

int i2c_getadapter(uint32_t i2c_bus_address)
{
	// error, function not implemented
	return -1;
}

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
		i2c_buses[i2cId] = mraa_i2c_init (i2c_bus);
	}
	return i2cId;
}

int i2c_setslave(int i2cId, uint8_t addr)
{
	return mraa_i2c_address (i2c_buses[i2cId], addr);
}

int i2c_writebyte(int i2cId, uint8_t byte)
{
	return mraa_i2c_write_byte (i2c_buses[i2cId], byte);
}

int i2c_writebytes(int i2cId, uint8_t *bytes, uint8_t length)
{
	return mraa_i2c_write (i2c_buses[i2cId], bytes, length);
}

int i2c_readbyte(int i2cId)
{
	return mraa_i2c_read_byte (i2c_buses[i2cId]);	
}

int i2c_readbytes(int i2cId, uint8_t *buf, int length)
{
	return mraa_i2c_read (i2c_buses[i2cId], buf, length);
}

int i2c_closeadapter(int i2cId)
{
	mraa_i2c_stop (i2c_buses[i2cId]);
	releaseI2CId (i2cId);
	return 0;
}

int i2c_readwrite(int i2cId)
{
	// struct i2c_rdwr_ioctl_data packets;
	// packets.msgs = i2c_buf;
	// packets.nmsgs = i2c_buf_count;
	// if (ioctl(i2c_fd, I2C_RDWR, &packets) < 0) {
	// 	perror("Unable to send data");
	// 	i2c_buf_count = 0;
	// 	return -1;
	// }
	// i2c_buf_count = 0;
	return -1;
}

int i2c_add_to_buf(uint8_t addr, uint8_t rw, uint8_t *value, int length)
{
	// if(i2c_buf_count < 2) {
	// 	i2c_buf[i2c_buf_count].addr = addr;
	// 	i2c_buf[i2c_buf_count].flags = rw ? I2C_M_RD : 0;
	// 	i2c_buf[i2c_buf_count].len = length;
	// 	i2c_buf[i2c_buf_count].buf = (char *)value;
	// 	return ++i2c_buf_count;
	// } else
	return -1;
}

int serial_openadapter(char *serial_bus)
{
	return -1;

}
int serial_set_speed(int serial_id, int baud)
{
	return 0;
}
int serial_bytes_available(int serial_id)
{
	return 0;
}
int serial_closeadapter(int serial_id)
{
	return 0;
}
int serial_writebyte(int serial_id, uint8_t byte)
{
	return 0;
}
int serial_writebytes(int serial_id, uint8_t *bytes, uint8_t length)
{
	return 0;
}
uint8_t serial_readbyte(int serial_id)
{
	return -1;
}
int serial_readbytes(int serial_id, uint8_t *buf, int length)
{
	return 0;
}
int serial_flush(int serial_id)
{
	return 0;
}
#endif

