#ifdef ARDUINOYUN



/*** INCLUDES ************************************************************************************/

#include <sys/types.h> /* open */
#include <sys/stat.h>  /* open */
#include <fcntl.h>     /* open */

#include <time.h>    /* time         */
#include <stdint.h>  /* integers     */
#include <unistd.h>  /* usleep, read */
#include <stdbool.h> /* booleans     */
#include <stdlib.h>  /* system call to lingpio */

#include "debug.h"
#include "wiring.h"

/*************************************************************************************************/



/*** DEFINES *************************************************************************************/

#define GPIO_PATH "/sys/class/gpio"
#define PWM_PATH "/sys/class/pwm"

#define MIN_PIN 2
#define MAX_PIN 13

/*************************************************************************************************/



/*** STATIC FUNCTIONS DECLARATIONS ***************************************************************/

static int set_gpio_direction(int pin, int mode);
static int set_gpio_value(int pin, int value);

/*************************************************************************************************/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



/*** API IMPLEMENTATION **************************************************************************/

int wiringSetup() {
  int status = system("lingpio");
  error(status != 0, return, "failed to export pins");

  return 0;
}



void pinMode(int pin, int mode) {
  error(!(((MIN_PIN <= pin) && (pin <= MAX_PIN)) || (pin == 7), return, "invalid pin");
  error(mode != INPUT && mode != OUTPUT, return, "mode can be either INPUT or OUTPUT");

  /* Try to open value */
  char buf[64];
  snprintf(buf, 64, GPIO_PATH "/D%d/value", pin);
  int fd = open(buf, O_RDONLY);

  /* Export if can't open value */
  if (fd != -1) {
    close(fd);
    error(1, return, "Can't open value")
  }

  /* Set direction */
  int set_gpio_direction_rc = set_gpio_direction(pin, mode);
  error(set_gpio_direction_rc != 0, return, "Can't set to pin %d direction %d", pin, mode);
}


void digitalWrite(int pin, int value) {
  error(!(((MIN_PIN <= pin) && (pin <= MAX_PIN)) || (pin == 7), return, "invalid pin");
  error(value != LOW && value != HIGH, return, "value can be either LOW or HIGH");

  set_gpio_value(pin, value);
}

////////////////////////////////////////
int digitalRead(int pin) {
  error(!(((MIN_PIN <= pin) && (pin <= MAX_PIN)) || (pin == 7), return, "invalid pin");

  /* Open value */
  char buf[64];
  snprintf(buf, 64, GPIO_PATH "/D%d/value", pin);
  int fd = open(buf, O_RDONLY);
  syserror(fd == -1, return -1, "can't open " GPIO_PATH "/D%d/value", pin);

  /* Read value */
  char ch;
  int read_rc = read(fd, &ch, 1);
  close(fd);
  error(read_rc != 1, return -1, "invalid read from " GPIO_PATH "/gpio%d/value", pin);

  if (ch == '0') {
    return LOW;
  } else if (ch == '1') {
    return HIGH;
  }

  error(true, /* Do nothing */, "invalid value found in " GPIO_PATH "/gpio%d/value", pin);
  return -1;
}

//TBD
void analogWrite(int pin, int value) {
  error(true, return, "analogWrite not implemented yet");
}

//TBD
//0-1023
int analogRead(int pin) {
  error(true, return -1, "analogRead not implemented yet");

  return 0;
}

//TBD
//#ce intoarce placa
int analogReadRaw(int pin) {
  error(true, return -1, "analogReadRaw not implemented yet");

  return 0;
}

//TBD
//ca mai sus
void analogWriteRaw(int pin, int value) {
  error(true, return, "analogWriteRaw not implemented yet");
}

//TBD
/* Advanced I/O */
//ma uit 8 digital write
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
  error(true, return, "shiftOut not implemented yet");
}

//TBD
//inversul de mai sus
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
  error(true, return 1, "shiftIn not implemented yet");

  return 0;
}


unsigned long pulseIn(uint8_t pin, uint8_t state) {
  error(true, return 1, "pulseIn not implemented yet");

  return 0;
}


/* Time */
void delay(unsigned int milliseconds) {
  usleep(milliseconds * 1000);
}


void delayMicroseconds(unsigned int microseconds) {
  usleep(microseconds);
}


unsigned int millis(void) {
  return micros() / 1000;;
}


unsigned int micros(void) {
  struct timespec t;

  t.tv_sec  = 0;
  t.tv_nsec = 0;

  clock_gettime(CLOCK_REALTIME, &t);

  return (unsigned long)(t.tv_sec) * 1000000L + t.tv_nsec / 1000L ;
}


int spi_getadapter(uint32_t spi_bus_address) {
  error(true, return 1, "spi_getadapter not implemented yet");

  return 0;
}


int spi_openadapter(uint8_t spi_bus) {
  error(true, return 1, "spi_openadapter not implemented yet");

  return 0;
}


int spi_setmode(int spiId, unsigned short mode) {
  error(true, return 1, "spi_setmode not implemented yet");

  return 0;
}


int spi_set_frequency(int spiId, int freq) {
  error(true, return 1, "spi_set_frequency not implemented yet");

  return 0;
}


uint8_t spi_writebyte(int spiId, uint8_t byte) {
  error(true, return 1, "spi_writebyte not implemented yet");

  return 0;
}


unsigned char * spi_writebytes(int spiId, uint8_t *bytes, uint8_t length) {
  error(true, return NULL, "pulseIn not implemented yet");

  return NULL;
}


int spi_lsb_mode(int spiId, unsigned char lsb) {
  error(true, return 1, "spi_lsb_mode not implemented yet");

  return 0;
}


int spi_bit_per_word(int spiId, unsigned int bits) {
  error(true, return 1, "spi_bit_per_word not implemented yet");

  return 0;
}


int spi_closeadapter(int spiId) {
  error(true, return 1, "spi_closeadapter not implemented yet");

  return 0;
}


int i2c_openadapter(uint8_t i2c_bus) {
  // error(true, return 1, "i2c_openadapter not implemented yet");

  return 0;
}


int i2c_setslave(int i2cId, uint8_t addr) {
  error(true, return 1, "i2c_setslave not implemented yet");

  return 0;
}


int i2c_writebyte(int i2cId, uint8_t byte) {
  error(true, return 1, "i2c_writebyte not implemented yet");

  return 0;
}


int i2c_writebytes(int i2cId, uint8_t *bytes, uint8_t length) {
  error(true, return 1, "i2c_writebytes not implemented yet");

  return 0;
}


int i2c_readbyte(int i2cId) {
  error(true, return 1, "i2c_readbyte not implemented yet");

  return 0;
}


int i2c_closeadapter(int i2cId) {
  error(true, return 1, "i2c_closeadapter not implemented yet");

  return 0;
}


int i2c_readbytes(int i2cId, uint8_t *buf, int length) {
  error(true, return 1, "i2c_readbytes not implemented yet");

  return 0;
}


int i2c_readwrite(int i2cId) {
  error(true, return 1, "i2c_readwrite not implemented yet");

  return 0;
}


int serial_openadapter(char *serial_bus) {
  error(true, return 1, "serial_openadapter not implemented yet");

  return 0;
}


int serial_set_speed(int serial_id, int baud) {
  error(true, return 1, "serial_set_speed not implemented yet");

  return 0;
}


int serial_bytes_available(int serial_id) {
  error(true, return 1, "serial_bytes_available not implemented yet");

  return 0;
}


int serial_closeadapter(int serial_id) {
  error(true, return 1, "serial_closeadapter not implemented yet");

  return 0;
}


int serial_writebyte (int serial_id, uint8_t byte) {
  error(true, return 1, "serial_writebyte not implemented yet");

  return 0;
}


int serial_writebytes(int serial_id, uint8_t *bytes, uint8_t length) {
  error(true, return 1, "serial_writebytes not implemented yet");

  return 0;
}


uint8_t serial_readbyte(int serial_id) {
  error(true, return 1, "serial_readbyte not implemented yet");

  return 0;
}


int serial_readbytes(int serial_id, uint8_t *buf, int length) {
  error(true, return 1, "serial_readbytes not implemented yet");

  return 0;
}


int serial_flush(int serial_id) {
  error(true, return 1, "serial_flush not implemented yet");

  return 0;
}

/*************************************************************************************************/



/*** STATIC FUNCTIONS IMPLEMENTATION *************************************************************/


static int set_gpio_direction(int pin, int mode) {
  error(!(((MIN_PIN <= pin) && (pin <= MAX_PIN)) || (pin == 7), return -1, "invalid pin");
  error(mode != INPUT && mode != OUTPUT, return -1, "mode can be either INPUT or OUTPUT");

  char buf[64];
  snprintf(buf, 64, GPIO_PATH "/D%d/direction", pin);
  int fd = open(buf, O_WRONLY);
  syserror(fd == -1, return -1, "Can't open " GPIO_PATH "/D%d/direction", pin);

  if (mode == INPUT) {
    write(fd, "in", 3);
  } else {
    write(fd, "out", 4);
  }

  close(fd);

  return 0;
}


static int set_gpio_value(int pin, int value) {
  error(!(((MIN_PIN <= pin) && (pin <= MAX_PIN)) || (pin == 7), return -1, "invalid pin");
  error(value != LOW && value != HIGH, return -1, "value can be either LOW or HIGH");

  char buf[64];
  snprintf(buf, 64, GPIO_PATH "/D%d/value", pin);
  int fd = open(buf, O_WRONLY);
  syserror(fd == -1, return -1, "Can't open " GPIO_PATH "/D%d/value", pin);

  if (value == LOW) {
    write(fd, "0", 2);
  } else {
    write(fd, "1", 2);
  }

  close(fd);

  return 0;
}

/*************************************************************************************************/



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* UDOONEO */
