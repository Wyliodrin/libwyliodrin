#ifdef SERVER
#include <stdio.h>
#include <time.h>
#include "wiring.h"


int getSerialId()
{
	return -1;
}

void releaseSerial(int id)
{
	
}

int getSPIId ()
{
	return -1;
}

void releaseSPIId (int id)
{

}

int getI2CId ()
{
	return -1;
}

void releaseI2CId (int id)
{
	
}

void resetPin (int pin);
void pwmReset (int pin);

int wiringSetup ()
{

}

void pinReset (int pin)
{

}

void pwmReset (pin)
{
}

void resetPin (int pin)
{
	
}

void pinMode (int pin, int mode)
{
	
}

void digitalWrite (int pin, int value)
{
	printf ("digitalWrite is not available for server\n");
}

int digitalRead (int pin)
{
	printf ("digitalRead is not available for server\n");
	return 0;
}

void analogWrite (int pin, int value)
{
	printf ("analogWrite is not available for server\n");
}

int analogRead (int pin)
{
	printf ("analogRead is not available for server\n");
	return 0;
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
	printf ("pulseIn is not available for server\n");
  return 0;
}

uint8_t shiftIn (uint8_t dPin, uint8_t cPin, uint8_t order)
{
	printf ("shiftIn is not available for server\n");
  return 0;
}

void shiftOut (uint8_t dPin, uint8_t cPin, uint8_t order, uint8_t val)
{
  printf ("shiftOut is not available for server\n");
}

int spi_getadapter(uint32_t spi_bus_address)
{
	// error, function not implemented
	return -1;
}

int spi_openadapter(uint8_t spi_bus)
{
	return -1;
}

int spi_setmode(int spiId, unsigned short mode)
{
	return -1;
}

int spi_set_frequency(int spiId, int freq)
{
	return -1;
}

uint8_t spi_writebyte(int spiId, uint8_t byte)
{
	return 0;
}

unsigned char * spi_writebytes(int spiId, uint8_t *bytes, uint8_t length)
{
	return NULL;
}

int spi_lsb_mode(int spiId, unsigned char lsb)
{
	return -1;
}

int spi_bit_per_word(int spiId, unsigned int bits)
{
	return -1;
}

int spi_closeadapter (int spiId)
{
	return 0;
}

int i2c_getadapter(uint32_t i2c_bus_address)
{
	// error, function not implemented
	return -1;
}

int i2c_openadapter(uint8_t i2c_bus)
{

	return -1;
}

int i2c_setslave(int i2cId, uint8_t addr)
{
	return -1;
}

int i2c_writebyte(int i2cId, uint8_t byte)
{
	return -1;
}

int i2c_writebytes(int i2cId, uint8_t *bytes, uint8_t length)
{
	return -1;
}

int i2c_readbyte(int i2cId)
{
	return -1;
}

int i2c_readbytes(int i2cId, uint8_t *buf, int length)
{
	return -1;
}

int i2c_closeadapter(int i2cId)
{
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
