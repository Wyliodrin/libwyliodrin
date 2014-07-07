#ifdef RASPBERRYPI
#include "wiring.h"
#include <pthread.h>
#include <string.h>

static int i2c_buses[MAX_I2C_PINS];
static int spi_buses[MAX_SPI_BUSES];
static int i2c_addresses[MAX_I2C_PINS];
static int spi_freq[MAX_SPI_BUSES];
static int spi_channels[MAX_SPI_BUSES];
#define SPI_DEFAULT_FREQ	1000000

int wiringSetup ()
{
	int i=0;
	wiringPiSetup();
	for (i=0; i<MAX_SPI_BUSES; i++) spi_buses[i]=-1;
	for (i=0; i<MAX_I2C_BUSES; i++) i2c_buses[i]=-1;
}

pthread_mutex_t lockspi;
pthread_mutex_t locki2c;

void pinReset (int pin)
{

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

int getI2CId ()
{
	int i;
	int id = -1;
	pthread_mutex_lock(&locki2c);
	for (i=0; i < MAX_I2C_BUSES && id == -1; i++)
	{
		if (i2c_buses[i] == -1)
		{
			id = i;
			i2c_buses[id] = 0;
		}
	}
	pthread_mutex_unlock(&locki2c);
	return id;
}

void releaseI2CId (int id)
{
	pthread_mutex_lock(&locki2c);
	i2c_buses[id] = -1;
	pthread_mutex_unlock(&locki2c);
}


int i2c_getadapter(uint32_t i2c_bus_address)
{
	// error, function not implemented
	return -1;
}

int i2c_openadapter(uint8_t i2c_bus)
{
	int id = getI2CId();
	i2c_buses[id] = wiringPiI2CSetup (i2c_bus) ;
	return id;
}

int i2c_setslave(int i2c_id, uint8_t addr)
{
	i2c_addresses[i2c_id] = addr;
	return 0;
}

int i2c_writebyte(int i2c_id, uint8_t byte)
{
	return wiringPiI2CWriteReg8 (i2c_buses[i2c_id], i2c_addresses[i2c_id], byte) ;
}

int i2c_writebytes(int i2c_id, uint8_t *bytes, uint8_t length)
{
	int rc = 0;
	int i;
	int fd = i2c_buses[i2c_id];
	int addr = i2c_addresses[i2c_id];

	for (i=0; i<length; i++)
	{
		if(wiringPiI2CWriteReg8(fd, addr, bytes[i]) != 0)
		{
			rc = -1;
		}
	}
	return rc;
	//return mraa_i2c_write (i2c_buses[i2c_bus], bytes, length);
}

int i2c_readbyte(int i2c_id)
{
	return wiringPiI2CReadReg8 (i2c_buses[i2c_id], i2c_addresses[i2c_id]);
	//return mraa_i2c_read_byte (i2c_buses[i2c_bus]);	
}

int i2c_readbytes(int i2c_id, uint8_t *buf, int length)
{
	int rc = 0;
	int i;
	int fd = i2c_buses[i2c_id];
	int addr = i2c_addresses[i2c_id];

	for (i=0; i<length; i++)
	{
		if(wiringPiI2CReadReg8(fd, addr, buf[i]) != 0)
		{
			rc = -1;
		}
	}
	return rc;
	//return mraa_i2c_read (i2c_buses[i2c_bus], buf, length);
}

int i2c_closeadapter(int i2c_id)
{
	// mraa_i2c_stop (i2c_buses[i2c_bus]);
	int rc = close(i2c_buses[i2c_id]);
	i2c_addresses[i2c_id] = -1;
	releaseI2CId (i2c_id);
	return rc;
}

int i2c_readwrite(int i2c_id)
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
	// 	return -1;
	return -1;
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
		fd = wiringSetup(spi_channels[spi_id], spi_freq[spi_id]);
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
		fd = wiringSetup(spi_channels[spi_id], spi_freq[spi_id]);
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
#endif