/**************************************************************************************************
 * RaspberryPi functions
 *
 * CONTENT
 * 1. Auxiliary
 * 2. SPI
 * 3. Serial
 *************************************************************************************************/

#ifdef RASPBERRYPI
#include "wiring.h"
#include "raspberrypi.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#define SPI_DEFAULT_FREQ     4000000
#define SERIAL_DEFAULT_SPEED 9600

struct serial_bus
{
  char *bus;
  int speed;
  int fd;
};

pthread_mutex_t lockspi;
pthread_mutex_t lockserial;

static int spi_buses[MAX_SPI_BUSES];
static int spi_freq[MAX_SPI_BUSES];
static int spi_channels[MAX_SPI_BUSES];
static struct serial_bus *serial_buses[MAX_SERIAL_BUSES];



/**************************************************************************************************
 * 1. Auxiliary
 *************************************************************************************************/

/**
 * Initialization
 */
int wiringSetup()
{
  int i = 0;
  const char *pins_numbering = getenv("PINS_NUMBERING");
  const char *grovepi = getenv("GROVE_PI");
  int addr_grovepi = -1;

  if (pins_numbering != NULL && strncmp (pins_numbering, "GPIO", 5) == 0)
  {
    wiringPiSetupGpio();
  }
  else if (pins_numbering != NULL && strncmp (pins_numbering, "PHYSICAL", 9) == 0)
  {
    wiringPiSetupPhys();
  }
  else
  {
    wiringPiSetup ();
  }

  if (grovepi)
  {
    sscanf (grovepi, "%d", &addr_grovepi);
  }

  if (addr_grovepi > -1)
  {
    grovepiSetup (addr_grovepi, 4);
  }

  for (i = 0; i < MAX_SPI_BUSES; i++) 
  {
    spi_buses[i]=-1;
  }

  for (i = 0; i < MAX_SERIAL_BUSES; i++) 
  {
    serial_buses[i] = NULL;
  }

  i2c_init();
}

/**
 * Reset pin
 */
void pinReset(int pin) {}

/**
 * Initialize I2C
 */
void i2c_init();

/**
 * Reads a pulse (either HIGH or LOW) on <pin>
 */
unsigned long pulseIn(uint8_t pin, uint8_t state)
{
  printf ("pulseIn is not implemented for Raspberry Pi");
  return 0;
}

/**
 * Return serial id
 */
int getSerialId()
{
  int i;
  int id = -1;
  struct serial_bus sb;

  pthread_mutex_lock(&lockserial);
  for(i = 0; i < MAX_SERIAL_BUSES && id == -1; i++)
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

/**
 * Release serial <id>
 */
void releaseSerial(int id)
{
  pthread_mutex_lock(&lockserial);
  free(serial_buses[id]);
  serial_buses[id] = NULL;
  pthread_mutex_unlock(&lockserial);
}

/**
 * Return SPI id
 */
int getSPIId()
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

/**
 * Release SPI <id>
 */
void releaseSPIId(int id)
{
  pthread_mutex_lock(&lockspi);
  spi_buses[id] = -1;
  pthread_mutex_unlock(&lockspi);
}



/**************************************************************************************************
 * 10. SPI
 *************************************************************************************************/

/**
 * TODO
 */
int spi_getadapter(uint32_t spi_bus_address)
{
  return -1;
}

/**
 * Open <spi_bus>
 */
int spi_openadapter(uint8_t spi_bus)
{
  int fd;
  int id = getSPIId();

  spi_freq[id] = SPI_DEFAULT_FREQ;
  spi_channels[id] = spi_bus;
  fd = wiringPiSPISetup (spi_bus, SPI_DEFAULT_FREQ);
  spi_buses[id] = fd;

  return id;
}

/**
 * Close SPI adapter
 */
int spi_closeadapter (int spi_id)
{
  releaseSPIId(spi_id);
  return 0;
}

/**
 * Set SPI mode
 */
int spi_setmode(int spi_id, unsigned short mode)
{
  return -1;
}

/**
 * Set SPI frequency
 */
int spi_set_frequency(int spi_id, int freq)
{
  if(spi_buses[spi_id] != -1)
  {
    close(spi_buses[spi_id]);
    spi_buses[spi_id] = -2;
    spi_freq[spi_id] = freq;    
    return 0;
  }

  return -1;
}

/**
 * Write SPI byte
 */
uint8_t spi_writebyte(int spi_id, uint8_t byte)
{
  int fd;
  uint8_t new_byte = byte;

  if(spi_buses[spi_id] == -1)
  {
    return -1;
  }
  else if (spi_buses[spi_id] == -2)
  {
    fd = wiringPiSPISetup(spi_channels[spi_id], spi_freq[spi_id]);
    spi_buses[spi_id] = fd;
  }

  wiringPiSPIDataRW(spi_channels[spi_id], &new_byte, 1);
  return new_byte;
}

/**
 * Write SPI bytes
 */
unsigned char *spi_writebytes(int spi_id, uint8_t *bytes, uint8_t length)
{
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

/**
 * TODO
 */
int spi_lsb_mode(int spi_bus, unsigned char lsb)
{ 
  return -1;
}

/**
 * TODO
 */
int spi_bit_per_word(int spi_bus, unsigned int bits)
{
  return -1;
}



/**************************************************************************************************
 * 10. Serial
 *************************************************************************************************/

/**
 * Open serial adapter
 */
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

/**
 * Close serial adapter
 */
int serial_closeadapter(int serial_id)
{
  struct serial_bus *sb = serial_buses[serial_id];

  if(!sb)
  {
    return -1;
  }
  if(sb->fd > 0)
  {
    serialClose(sb->fd);
  }

  return 0;
}

/**
 * Set speed on serial bus 
 */
int serial_set_speed(int serial_id, int baud)
{
  struct serial_bus *sb = serial_buses[serial_id];

  serialClose(sb->fd);
  sb->speed = baud;
  sb->fd = -1;

  return 0;
}

/**
 * TODO
 */
int serial_bytes_available(int serial_id)
{
  struct serial_bus *sb = serial_buses[serial_id];
  if(!sb)
  {
    return -1;
  }
  if(sb->fd < 0)
  {
    sb->fd = serialOpen(sb->bus, sb->speed);
  }

  return serialDataAvail(sb->fd);
}

/**
 * Write byte on serial bus
 */
int serial_writebyte(int serial_id, uint8_t byte)
{
  struct serial_bus *sb = serial_buses[serial_id];

  if(!sb)
  {
    return -1;
  }
  if(sb->fd < 0)
  {
    sb->fd = serialOpen(sb->bus, sb->speed);
  }
  serialPutchar (sb->fd, byte) ;

  return 0;
}

/**
 * Wryte bytes on serial bus
 */
int serial_writebytes(int serial_id, uint8_t *bytes, uint8_t length)
{
  struct serial_bus *sb = serial_buses[serial_id];
  int i;

  if(!sb) {
    return -1;
  }
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

/**
 * Read byte from serial bus
 */
uint8_t serial_readbyte(int serial_id)
{
  struct serial_bus *sb = serial_buses[serial_id];
  if(!sb)
  {
    return -1;
  }
  if(sb->fd < 0)
  {
    sb->fd = serialOpen(sb->bus, sb->speed);
  }

  return serialGetchar (sb->fd);
}

/**
 * Read bytes from serial bus
 */
int serial_readbytes(int serial_id, uint8_t *buf, int length)
{
  struct serial_bus *sb = serial_buses[serial_id];
  int i;
  uint8_t c;

  if(!sb) {
    return -1;
  }
  if(sb->fd < 0)
  {
    sb->fd = serialOpen(sb->bus, sb->speed);
  }
  for (i=0; i<length; i++)
  {
    c = serialGetchar(sb->fd);
    if(c != -1)
    {
      buf[i] = c;
    }
    else
    {
      return i;
    }
  }

  return length;
}

/**
 * Flush serial bus
 */
int serial_flush(int serial_id)
{
  struct serial_bus *sb = serial_buses[serial_id];
  if(!sb) {
    return -1;
  }
  if(sb->fd < 0)
  {
    sb->fd = serialOpen(sb->bus, sb->speed);
  }
  serialFlush (sb->fd);
  return 0;
}



#endif // RASPBERRYPI
