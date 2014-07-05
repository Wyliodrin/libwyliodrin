#ifdef RASPBERRYPI
#include <wiringPi.h>
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