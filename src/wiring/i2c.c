/**************************************************************************************************
 * I2C functions for Raspberry Pi and Beaglebone
 *
 * CONTENT
 * 1. Auxiliary
 * 2. I2C
 *************************************************************************************************/

#if defined(RASPBERRYPI) || defined(BEAGLEBONE)

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



/**************************************************************************************************
 * 1. Auxiliary
 *************************************************************************************************/

/**
 * I2C initialization
 */
void i2c_init ()
{
  int i;

  for (i = 0; i < MAX_I2C_PINS; i++) 
  {
    i2c_buses[i] = -1;
  }
}

/**
 * Return number or I2C buses
 */
int getI2CId ()
{
  int i;
  int id = -1;

  pthread_mutex_lock(&locki2c);
  for (i = 0; i < MAX_I2C_BUSES && id == -1; i++)
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

/**
 * Release I2C bus
 */
void releaseI2CId (int id)
{
  pthread_mutex_lock(&locki2c);
  i2c_buses[id] = -1;
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



/**************************************************************************************************
 * 2. I2C
 *************************************************************************************************/

/**
 * Open I2C adapter
 */
int i2c_openadapter(uint8_t i2c_bus)
{
  int id;
  char filepath[32];

  id = getI2CId();

  if (i2c_bus == 255)
  {
    #ifdef RASPBERRYPI
    if (piBoardRev() == 1)
    {
      i2c_bus = 0;
    }
    else
    {
      i2c_bus = 1;
    }
    #endif
    
    #ifdef BEAGLEBONE
    i2c_bus = 1;
    #endif
  }

  snprintf(filepath, 32, "/dev/i2c-%u", i2c_bus);
  if ((i2c_buses[id] = open(filepath, O_RDWR)) < 0) 
  {
    fprintf(stderr, "Failed to open requested i2c port %s\n", filepath);
    perror("bus");
  }
  
  return id;
}

/**
 * Close I2C adapter
 */
int i2c_closeadapter(int i2c_id)
{
  int rc = close(i2c_buses[i2c_id]);
  i2c_addresses[i2c_id] = -1;
  releaseI2CId (i2c_id);

  return rc;
}

/**
 * Set I2C slave
 */
int i2c_setslave(int i2c_id, uint8_t addr)
{
  ioctl(i2c_buses[i2c_id], I2C_SLAVE_FORCE, addr);
  return 0;
}

/**
 * Write byte on I2C bus
 */
int i2c_writebyte(int i2c_id, uint8_t byte)
{
  if(i2c_smbus_write_byte(i2c_buses[i2c_id], byte) < 0)
  {
    fprintf(stderr, "Failed to write to i2c\n");
    perror("i2c write");
  }

  return 0;
}

/**
 * Write bytes on I2C bus
 */
int i2c_writebytes(int i2c_id, uint8_t *bytes, uint8_t length){
  int i;
  int rc = 0;
  int fd = i2c_buses[i2c_id];

  if(i2c_smbus_write_i2c_block_data(i2c_buses[i2c_id], bytes[0], length-1, (uint8_t*) bytes+1) < 0)
  {
    fprintf(stderr, "Failed to write to i2c\n");
    perror ("i2c write bytes");
  }
  
  return rc;
}

/**
 * Read byte from I2C bus
 */
int i2c_readbyte(int i2c_id)
{
  int data = i2c_smbus_read_byte(i2c_buses[i2c_id]);
  
  if (data < 0) 
  {
    return -1;
  }
  else
  {
    return data;
  } 
}

/**
 * Read bytes on I2C bus
 */
int i2c_readbytes(int i2c_id, uint8_t *buf, int length)
{
  if(read(i2c_buses[i2c_id], buf, length) == length) {
    return length;
  }

  return 0;
}

/**
 * TODO
 */
int i2c_readwrite(int i2c_id)
{
  return -1;
}



#endif // defined(RASPBERRYPI) || defined(BEAGLEBONE)
