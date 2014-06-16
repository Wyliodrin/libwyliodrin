
#ifdef ARDUINOGALILEO

#include "wiring.h"

static maa_gpio_context gpio_pins[MAX_GPIO_PINS];
static maa_aio_context aio_pins[MAX_AIO_PINS];
static maa_pwm_context pwm_pins[MAX_PWM_PINS];
static maa_i2c_context i2c_buses[MAX_I2C_PINS];

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

int wiringSetup ()
{
	maa_init ();
}

void resetPin (int pin)
{
	if (gpio_pins[pin] != NULL)
	{
		maa_gpio_close (gpio_pins[pin]);
		gpio_pins[pin] = NULL;
	}
	if (pwm_pins[pin] != NULL)
	{
		maa_pwm_close (pwm_pins[pin]);
		pwm_pins[pin] = NULL;
	}
	if (aio_pins[pin] != NULL)
	{
		maa_aio_close (aio_pins[pin]);
		aio_pins[pin] = NULL;
	}
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
		gpio_pins[pin] = maa_gpio_init (pin);
	}
	if (mode == INPUT)
	{
		maa_gpio_dir (gpio_pins[pin], MAA_GPIO_IN);
	}
	else if (mode == OUTPUT)
	{
		maa_gpio_dir (gpio_pins[pin], MAA_GPIO_OUT);
	}
}

void digitalWrite (int pin, int value)
{
	outPin (pin);
	maa_gpio_write (gpio_pins[pin], value);
}

int digitalRead (int pin)
{
	inPin (pin);
	return maa_gpio_read (gpio_pins[pin]);
}

void analogWrite (int pin, int value)
{
	if (!isPwmPin (pin))
	{
		resetPin (pin);
		pwm_pins[pin] = maa_pwm_init (pin);
	}
	maa_pwm_write (pwm_pins[pin], value/255.0);
	maa_pwm_enable (pwm_pins[pin], 1);
}

int analogRead (int pin)
{
	pinValue (pin);
	if (!isAioPin (pin))
	{
		resetPin (pin);
		aio_pins[pin] = maa_aio_init (pin);
	}
	return maa_aio_read (aio_pins[pin]);
}

void delay (int milliseconds)
{
	usleep (milliseconds*1000);
}

void delayMicroseconds (int microseconds)
{
	usleep (microseconds);
}

unsigned long millis()
{
   return micros() / 1000;
}


unsigned long micros()
{

  struct timespec t;
  t.tv_sec = t.tv_nsec = 0;

  clock_gettime(CLOCK_REALTIME, &t);
  return (unsigned long)(t.tv_sec)*1000000L + t.tv_nsec / 1000L ;

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

int i2c_getadapter(uint32_t i2c_bus_address)
{
	// error, function not implemented
	return -1;
}

int i2c_openadapter(uint8_t i2c_bus)
{
	i2c_buses[i2c_bus] = maa_i2c_init (i2c_bus);
	return i2c_bus;
}

int i2c_setslave(int i2c_bus, uint8_t addr)
{
	return maa_i2c_address (i2c_buses[i2c_bus], addr);
}

int i2c_writebyte(int i2c_bus, uint8_t byte)
{
	return maa_i2c_write_byte (i2c_buses[i2c_bus], byte);
}

int i2c_writebytes(int i2c_bus, uint8_t *bytes, uint8_t length)
{
	return maa_i2c_write (i2c_buses[i2c_bus], bytes, length);
}

int i2c_readbyte(int i2c_bus)
{
	return maa_i2c_read_byte (i2c_buses[i2c_bus]);	
}

int i2c_readbytes(int i2c_bus, uint8_t *buf, int length)
{
	return maa_i2c_read (i2c_buses[i2c_bus], buf, length);
}

int i2c_readwrite(int i2c_bus)
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
	// return 0;
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
	// 	return -1;
}

#endif

