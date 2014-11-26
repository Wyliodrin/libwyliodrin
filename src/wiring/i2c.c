#if defined(RASPBERRYPI) || (BEAGLEBONE)

#ifdef RASPBERRYPI
#include "raspberrypi.h"
#endif

#ifdef BEAGLEBONE
#include "beagleboneConfig.h"
#endif

#include "wiring.h"
#include <linux/i2c-dev.h>
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>

static int i2c_buses[MAX_I2C_PINS];
static int i2c_addresses[MAX_I2C_PINS];

pthread_mutex_t locki2c;

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
	// printf ("%d\n", i2c_bus);
	int id = getI2CId();
	// i2c_buses[id] = wiringPiI2CSetup (i2c_bus) ;
	#ifdef RASPBERRYPI
	if (i2c_bus==-1)
	{
		if (piBoardRev ()==1)
		{
			i2c_bus = 0;
		}
		else
		{
			i2c_bus = 1;
		}
	}
	#endif
	char filepath[32];
    snprintf(filepath, 32, "/dev/i2c-%u", i2c_bus);
    if ((i2c_buses[i2c_bus] = open(filepath, O_RDWR)) < 1) {
        fprintf(stderr, "Failed to open requested i2c port %s\n", filepath);
    }
	//perror ("set");
	return id;
}

int i2c_setslave(int i2c_id, uint8_t addr)
{
	// i2c_addresses[i2c_id] = addr;
	ioctl (i2c_buses[i2c_id], I2C_SLAVE_FORCE, addr);
	return 0;
}

int i2c_writebyte(int i2c_id, uint8_t byte)
{
	// wiringPiI2CWrite (i2c_buses[i2c_id], byte) ;
	// perror ("write");
	if (i2c_smbus_write_byte(i2c_buses[i2c_id], byte) < 0) {
        fprintf(stderr, "Failed to write to i2c\n");
    }
	return 0;
}

int i2c_writebytes(int i2c_id, uint8_t *bytes, uint8_t length)
{
	int rc = 0;
	int i;
	int fd = i2c_buses[i2c_id];
	// int addr = i2c_addresses[i2c_id];

	if (i2c_smbus_write_i2c_block_data(i2c_buses[i2c_id], bytes[0], length-1, (uint8_t*) bytes+1) < 0) {
        fprintf(stderr, "Failed to write to i2c\n");
    }
	// for (i=0; i<length; i++)
	// {
	// 	if(wiringPiI2CWrite(fd, bytes[i]) < 0)
	// 	{
	// 		// rc = -1;
	// 	}
	// 	perror ("write");
	// 	delay (10);
	// }
	return rc;
	//return mraa_i2c_write (i2c_buses[i2c_bus], bytes, length);
}

int i2c_readbyte(int i2c_id)
{
	// printf ("Read byte %d\n", i2c_id);
	int data;
	data = i2c_smbus_read_byte(i2c_buses[i2c_id]);
	// data = wiringPiI2CRead (i2c_buses[i2c_id]);
	if (data < 0) return -1;
	else return data;
	//return mraa_i2c_read_byte (i2c_buses[i2c_bus]);	
}

int i2c_readbytes(int i2c_id, uint8_t *buf, int length)
{
	//printf ("Read bytes %d\n", i2c_id);
	int rc = 0;
	int i;
	int data;
	int fd = i2c_buses[i2c_id];
	// int addr = i2c_addresses[i2c_id];

	if (read(i2c_buses[i2c_id], buf, length) == length) {
        return length;
    }
	// for (i=0; i<length; i++)
	// {
	// 	//printf ("a %d\n", addr);
	// 	data = wiringPiI2CRead(fd);
	// 	//perror ("readbytes");
	// 	if (data < 0)
	// 	{
	// 		rc = -1;
	// 	}
	// 	else
	// 	{
	// 		buf[i]=data;
	// 	}
	// 	// printf ("%d\n", data);
	// }
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

#endif
