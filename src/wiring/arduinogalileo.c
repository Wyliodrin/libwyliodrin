
#ifdef ARDUINOGALILEO

#include "wiring.h"

static mraa_gpio_context gpio_pins[MAX_GPIO_PINS];
static mraa_aio_context aio_pins[MAX_AIO_PINS];
static mraa_pwm_context pwm_pins[MAX_PWM_PINS];
static mraa_spi_context spi_buses[MAX_SPI_PINS];
static mraa_i2c_context i2c_buses[MAX_I2C_PINS];

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

void resetPin (int pin);
void pwmReset (int pin);

int wiringSetup ()
{
	mraa_init ();
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
	if (mode == INPUT)
	{
		mraa_gpio_dir (gpio_pins[pin], MRAA_GPIO_IN);
	}
	else if (mode == OUTPUT)
	{
		mraa_gpio_dir (gpio_pins[pin], MRAA_GPIO_OUT);
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
	}
	if (pwm_pins[pin])
	{
		mraa_pwm_period_us(pwm_pins[pin], 1200);
		mraa_pwm_write (pwm_pins[pin], value/255.0);
		mraa_pwm_enable (pwm_pins[pin], 1);
	}
	else
	{
		printf ("Cannot PWM on pin %d\n"+pin);
	}
}

int analogRead (int pin)
{
	pinValue (pin);
	if (!isAioPin (pin))
	{
		resetPin (pin);
		aio_pins[pin] = mraa_aio_init (pin);
	}
	return mraa_aio_read (aio_pins[pin]);
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

int spi_getadapter(uint32_t spi_bus_address)
{
	// error, function not implemented
	return -1;
}

int spi_openadapter(uint8_t spi_bus)
{
	spi_buses[spi_bus] = mraa_spi_init (spi_bus);
	return spi_bus;
}

int spi_setmode(int spi_bus, unsigned short mode)
{
	return mraa_spi_mode (spi_buses[spi_bus], mode);
}

int spi_set_frequency(int spi_bus, int freq)
{
	return mraa_spi_frequency (spi_buses[spi_bus], freq);
}

uint8_t spi_writebyte(int spi_bus, uint8_t byte)
{
	return mraa_spi_write (spi_buses[spi_bus], byte);
}

unsigned char * spi_writebytes(int spi_bus, uint8_t *bytes, uint8_t length)
{
	return mraa_spi_write_buf (spi_buses[spi_bus], bytes, length);
}

int spi_lsb_mode(int spi_bus, unsigned char lsb)
{
	return mraa_spi_lsbmode (spi_buses[spi_bus], lsb);	
}

int spi_bit_per_word(int spi_bus, unsigned int bits)
{
	return mraa_spi_bit_per_word (spi_buses[spi_bus], bits);
}

int spi_closeadapter (int spi_bus)
{
	mraa_spi_stop (spi_buses[spi_bus]);
	spi_buses[spi_bus] = NULL;
	return 0;
}

int i2c_getadapter(uint32_t i2c_bus_address)
{
	// error, function not implemented
	return -1;
}

int i2c_openadapter(uint8_t i2c_bus)
{
	i2c_buses[i2c_bus] = mraa_i2c_init (i2c_bus);
	return i2c_bus;
}

int i2c_setslave(int i2c_bus, uint8_t addr)
{
	return mraa_i2c_address (i2c_buses[i2c_bus], addr);
}

int i2c_writebyte(int i2c_bus, uint8_t byte)
{
	return mraa_i2c_write_byte (i2c_buses[i2c_bus], byte);
}

int i2c_writebytes(int i2c_bus, uint8_t *bytes, uint8_t length)
{
	return mraa_i2c_write (i2c_buses[i2c_bus], bytes, length);
}

int i2c_readbyte(int i2c_bus)
{
	return mraa_i2c_read_byte (i2c_buses[i2c_bus]);	
}

int i2c_readbytes(int i2c_bus, uint8_t *buf, int length)
{
	return mraa_i2c_read (i2c_buses[i2c_bus], buf, length);
}

int i2c_closeadapter(int i2c_bus)
{
	mraa_i2c_stop (i2c_buses[i2c_bus]);
	i2c_buses[i2c_bus] = NULL;
	return 0;
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

