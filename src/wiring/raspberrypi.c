#ifdef RASPBERRYPI
#include "wiring.h"
int wiringSetup ()
{
	wiringPiSetup();
}

static int i2c_buses[MAX_I2C_PINS];
static int i2c_addresses[MAX_I2C_PINS];


int i2c_getadapter(uint32_t i2c_bus_address)
{
	// error, function not implemented
	return -1;
}

int i2c_openadapter(uint8_t i2c_bus)
{
	i2c_buses[i2c_bus] = wiringPiI2CSetup (i2c_bus) ;
	return i2c_bus;
}

int i2c_setslave(int i2c_bus, uint8_t addr)
{
	i2c_addresses[i2c_bus] = addr;
	return 0;
}

int i2c_writebyte(int i2c_bus, uint8_t byte)
{
	return wiringPiI2CWriteReg8 (i2c_buses[i2c_bus], i2c_addresses[i2c_bus], byte) ;
}

int i2c_writebytes(int i2c_bus, uint8_t *bytes, uint8_t length)
{
	int rc = 0;
	int i;
	int fd = i2c_buses[i2c_bus];
	int addr = i2c_addresses[i2c_bus];

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

int i2c_readbyte(int i2c_bus)
{
	return wiringPiI2CReadReg8 (i2c_buses[i2c_bus], i2c_addresses[i2c_bus]);
	//return mraa_i2c_read_byte (i2c_buses[i2c_bus]);	
}

int i2c_readbytes(int i2c_bus, uint8_t *buf, int length)
{
	int rc = 0;
	int i;
	int fd = i2c_buses[i2c_bus];
	int addr = i2c_addresses[i2c_bus];

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

int i2c_closeadapter(int i2c_bus)
{
	// mraa_i2c_stop (i2c_buses[i2c_bus]);
	int rc = close(i2c_buses[i2c_bus]);
	i2c_buses[i2c_bus] = -1;
	i2c_addresses[i2c_bus] = -1;
	return rc;
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
	return 0;
}

int spi_setmode(int spi_bus, unsigned short mode)
{
	//return mraa_spi_mode (spi_buses[spi_bus], mode);
	return 0;
}

int spi_set_frequency(int spi_bus, int freq)
{
//	return mraa_spi_frequency (spi_buses[spi_bus], freq);
	return 0;
}

uint8_t spi_writebyte(int spi_bus, uint8_t byte)
{
	//return mraa_spi_write (spi_buses[spi_bus], byte);
	return 0;
}

unsigned char * spi_writebytes(int spi_bus, uint8_t *bytes, uint8_t length)
{
	//return mraa_spi_write_buf (spi_buses[spi_bus], bytes, length);
	return 0;
}

int spi_lsb_mode(int spi_bus, unsigned char lsb)
{
	//return mraa_spi_lsbmode (spi_buses[spi_bus], lsb);	
	return 0;
}

int spi_bit_per_word(int spi_bus, unsigned int bits)
{
	//return mraa_spi_bit_per_word (spi_buses[spi_bus], bits);
	return 0;
}

int spi_closeadapter (int spi_bus)
{
// 	mraa_spi_stop (spi_buses[spi_bus]);
// 	spi_buses[spi_bus] = NULL;
	return 0;
}
#endif