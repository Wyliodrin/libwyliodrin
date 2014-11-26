#ifdef RASPBERRYPI
#include "wiring.h"
#include <linux/i2c-dev.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

struct serial_bus
{
	char *bus;
	int speed;
	int fd;
};

static int i2c_buses[MAX_I2C_PINS];
static int spi_buses[MAX_SPI_BUSES];
static int i2c_addresses[MAX_I2C_PINS];
static int spi_freq[MAX_SPI_BUSES];
static int spi_channels[MAX_SPI_BUSES];
static struct serial_bus *serial_buses[MAX_SERIAL_BUSES];
#define SPI_DEFAULT_FREQ	4000000
#define SERIAL_DEFAULT_SPEED	9600

int wiringSetup ()
{
	int i=0;
	const char *pins_numbering = getenv ("PINS_NUMBERING");
	if (pins_numbering != NULL && strncmp (pins_numbering, "GPIO", 5)==0)
	{
		wiringPiSetupGpio();
	}
	else
	if (pins_numbering != NULL && strncmp (pins_numbering, "PHYSICAL", 9)==0)
	{
		wiringPiSetupPhys();
	}
	else
	{
		wiringPiSetup ();
	}
	for (i=0; i<MAX_SPI_BUSES; i++) spi_buses[i]=-1;
	for (i=0; i<MAX_I2C_BUSES; i++) i2c_buses[i]=-1;
	for (i=0; i<MAX_SERIAL_BUSES; i++) serial_buses[i]=NULL;
}

pthread_mutex_t lockspi;
pthread_mutex_t locki2c;
pthread_mutex_t lockserial;

void pinReset (int pin)
{

}

int getSerialId()
{
	int i;
	int id = -1;
	struct serial_bus sb;
	pthread_mutex_lock(&lockserial);
	for(i=0; i < MAX_SERIAL_BUSES && id == -1; i++)
	{
		if(serial_buses[i] == NULL)
		{
			id = i;
			struct serial_bus *sb = malloc(sizeof(struct serial_bus));
			serial_buses[id] = sb;
		}
	}
	pthread_mutex_unlock(&lockserial);
	return id;
}

void releaseSerial(int id)
{
	pthread_mutex_lock(&lockserial);
	free(serial_buses[id]);
	serial_buses[id] = NULL;
	pthread_mutex_unlock(&lockserial);
}

int getSPIId ()
{
	int i;
	int id = -1;
	pthread_mutex_lock(&lockspi);
	for (i=0; i < MAX_SPI_BUSES && id == -1; i++)
	{
		if (spi_buses[i] == -1)
		{
			id = i;
			spi_buses[id] = 0;
		}
	}
	pthread_mutex_unlock(&lockspi);
	return id;
}

void releaseSPIId (int id)
{
	pthread_mutex_lock(&lockspi);
	spi_buses[id] = -1;
	pthread_mutex_unlock(&lockspi);
}

int spi_getadapter(uint32_t spi_bus_address)
{
	// error, function not implemented
	return -1;
}

int spi_openadapter(uint8_t spi_bus)
{
	// spi_buses[spi_bus] = mraa_spi_init (spi_bus);
	// return spi_bus;
	int fd;
	int id = getSPIId();
	spi_freq[id] = SPI_DEFAULT_FREQ;
	spi_channels[id] = spi_bus;
	fd = wiringPiSPISetup (spi_bus, SPI_DEFAULT_FREQ);
	spi_buses[id] = fd;
	return id;
}

int spi_setmode(int spi_id, unsigned short mode)
{
	//return mraa_spi_mode (spi_buses[spi_bus], mode);
	return -1;
}

int spi_set_frequency(int spi_id, int freq)
{
//	return mraa_spi_frequency (spi_buses[spi_bus], freq);
	if(spi_buses[spi_id] != -1)
	{
		close(spi_buses[spi_id]);
		spi_buses[spi_id] = -2;
		spi_freq[spi_id] = freq;		
		return 0;
	}
	return -1;
}

uint8_t spi_writebyte(int spi_id, uint8_t byte)
{
	//return mraa_spi_write (spi_buses[spi_bus], byte);
	//int fd = wiringPiSPISetup (int channel, int speed);
	int fd;
	uint8_t new_byte = byte;
	if(spi_buses[spi_id] == -1)
		return -1;
	else if (spi_buses[spi_id] == -2)
	{
		fd = wiringPiSPISetup(spi_channels[spi_id], spi_freq[spi_id]);
		spi_buses[spi_id] = fd;
	}
	wiringPiSPIDataRW (spi_channels[spi_id], &new_byte, 1);
	return new_byte;
}

unsigned char * spi_writebytes(int spi_id, uint8_t *bytes, uint8_t length)
{
	//return mraa_spi_write_buf (spi_buses[spi_bus], bytes, length);
	//return 0;
	int fd;
	unsigned char *new_byte = malloc(length * sizeof(unsigned char));
	memcpy(new_byte, bytes, length);
	if (spi_buses[spi_id] == -2)
	{
		fd = wiringPiSPISetup(spi_channels[spi_id], spi_freq[spi_id]);
		spi_buses[spi_id] = fd;
	}
	wiringPiSPIDataRW (spi_channels[spi_id], new_byte, length);
	return new_byte;
}

int spi_lsb_mode(int spi_bus, unsigned char lsb)
{
	//return mraa_spi_lsbmode (spi_buses[spi_bus], lsb);	
	return -1;
}

int spi_bit_per_word(int spi_bus, unsigned int bits)
{
	//return mraa_spi_bit_per_word (spi_buses[spi_bus], bits);
	return -1;
}

int spi_closeadapter (int spi_id)
{
// 	mraa_spi_stop (spi_buses[spi_bus]);
// 	spi_buses[spi_bus] = NULL;
	releaseSPIId(spi_id);
	return 0;
}

int serial_openadapter(char *serial_bus)
{
	int id = getSerialId();
	struct serial_bus *sb = serial_buses[id];
	sb->bus = serial_bus;
	sb->speed = SERIAL_DEFAULT_SPEED;
	int fd = serialOpen(sb->bus, sb->speed);
	sb->fd = fd;
	return id;

}
int serial_set_speed(int serial_id, int baud)
{
	struct serial_bus *sb = serial_buses[serial_id];
	serialClose(sb->fd);
	sb->speed = baud;
	sb->fd = -1;
	return 0;
}
int serial_bytes_available(int serial_id)
{
	//TODO verific sau las sa dea segfault?
	struct serial_bus *sb = serial_buses[serial_id];
	if(!sb)
		return -1;
	if(sb->fd < 0)
	{
		sb->fd = serialOpen(sb->bus, sb->speed);
	}
	return serialDataAvail(sb->fd);
}
int serial_closeadapter(int serial_id)
{
	struct serial_bus *sb = serial_buses[serial_id];
	if(!sb)
		return -1;
	if(sb->fd > 0)
	{
		serialClose(sb->fd);
	}	
	return 0;
}
int serial_writebyte(int serial_id, uint8_t byte)
{
	struct serial_bus *sb = serial_buses[serial_id];
	if(!sb)
		return -1;
	if(sb->fd < 0)
	{
		sb->fd = serialOpen(sb->bus, sb->speed);
	}
	serialPutchar (sb->fd, byte) ;
	return 0;
}
int serial_writebytes(int serial_id, uint8_t *bytes, uint8_t length)
{
	struct serial_bus *sb = serial_buses[serial_id];
	int i;
	if(!sb)
		return -1;
	if(sb->fd < 0)
	{
		sb->fd = serialOpen(sb->bus, sb->speed);
	}
	for (i=0; i<length; i++)
	{
		serialPutchar (sb->fd, bytes[i]);
	}
	return 0;
}
uint8_t serial_readbyte(int serial_id)
{
	struct serial_bus *sb = serial_buses[serial_id];
	if(!sb)
		return -1;
	if(sb->fd < 0)
	{
		sb->fd = serialOpen(sb->bus, sb->speed);
	}
	return serialGetchar (sb->fd);
}
int serial_readbytes(int serial_id, uint8_t *buf, int length)
{
	struct serial_bus *sb = serial_buses[serial_id];
	int i;
	uint8_t c;
	if(!sb)
		return -1;
	if(sb->fd < 0)
	{
		sb->fd = serialOpen(sb->bus, sb->speed);
	}
	for (i=0; i<length; i++)
	{
		c = serialGetchar(sb->fd);
		if(c != -1)
			buf[i] = c;
		else
			return i;
	}
	return length;
}
int serial_flush(int serial_id)
{
	struct serial_bus *sb = serial_buses[serial_id];
	if(!sb)
		return -1;
	if(sb->fd < 0)
	{
		sb->fd = serialOpen(sb->bus, sb->speed);
	}
	serialFlush (sb->fd);
	return 0;
}
#endif
