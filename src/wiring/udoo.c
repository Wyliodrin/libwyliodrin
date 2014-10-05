/**************************************************************************************************
 * This file contains the functions available on UDOO pin headers.
 * See documentation here: 
 *      http://arduino.cc/en/Reference/HomePage
 *      file:///home/andrei/Desktop/UDOO_Starting_Manual_beta0.4_11_28_2013.pdf
 *  
 * Follow "Wyliodrin Coding Style Convention" at:
 *      https://docs.google.com/document/d/14zRCX1KIwvQ1qEzWBVH-We0CkQmd5-kZb81bzvbIQKY/edit
 * 
 * CONTENT:
 * 1.  General configuration
 * 2.  Digital I/O
 * 3.  Analog I/O
 * 4.  PWMs: pwm1, pwm2, pwm3, pwm4
 * 5.  Advanced I/O
 * 6.  Time
 * 7.  Servo
 * 8.  I2C
 * 9.  SPIs: spi1, spi2, spi5   // TODO
 * 10. Communication (Serial, Stream)   // TODO
 * 11. UARTs: uart1, uart3, uart4, uart5   // TODO
 * 12. SD1   // TODO
 * 13. Spdif   // TODO
 * 14. Timer capture   // TODO
 * 15. Timer compare   // TODO
 * 16. WATCHDOG FUNCTIONALITIES: watchdog reset, watchdog out   // TODO
 * 17. Clock out   // TODO
 * 18. Digital audio   // TODO 
 *************************************************************************************************/

#ifdef UDOO

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include "wiring.h"
#include "udooConfig.h"
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <pthread.h>


/**************************************************************************************************
 * 1. General configuration
 *************************************************************************************************/

static int i2c_buses[MAX_UDOO_I2C_BUSES];
static int i2c_addresses[MAX_UDOO_I2C_BUSES];

t_firmata *initFirmata (t_firmata *firmata)
{
    #ifndef FIRMATA
        debug(" Sorry, you do not have access to Firmata!\n");
        printf(" returning NULL...\n");
        return NULL;
    #endif
    int i = 0;
    char str[] = "/dev/ttymxc3";
    firmata = firmata_new(str);     // init Firmata
    while (!firmata->isReady)       // wait until device is up
        firmata_pull(firmata);
    return firmata;
}

int wiringSetup ()
{
    #ifdef FIRMATA
        firmata = initFirmata(firmata);
    #endif
    #ifndef FIRMATA
        printf("\n Sorry, you cannot initialize Firmata!\n");
        printf(" Enable Firmata using -DFIRMATA=ON option in Cmake\n");
        printf(" With Firmata disabled you can access only:\n");
        printf("     - Digital I/O\n");
        printf("     - Advanced I/O\n");
        printf("     - Time functions\n");
        printf("     - I2C\n\n");
    #endif
    int id;
    for (id = 0; id <= MAX_UDOO_I2C_BUSES; id++) i2c_buses[id] = -1;
    return 0;
}

/*
pthread_mutex_t locki2c;

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

void releaseI2CId (int id)
{
    pthread_mutex_lock(&locki2c);
    i2c_buses[id] = -1;
    pthread_mutex_unlock(&locki2c);
}
*/

void pinReset (int pin)
{
    return;
}

/**************************************************************************************************
 * 2. Digital I/O
 *************************************************************************************************/

/**
 * Configures the specified pin to behave either as an input or an output
 * pin: the number of the pin whose mode you wish to set (gpio pin)
 * mode: INPUT or OUTPUT
 */
void pinMode (int pin, int mode) 
{
    if (mode != INPUT && mode != OUTPUT) {
        debug("Mode can be either INPUT or OUTPUT");
        return;
    }
    int gpioPin = pinTable[pin].gpio;
    gpioExport(gpioPin);
    if (mode == INPUT) {
        gpioSetDirInput(gpioPin);
    } else {
        gpioSetDirOutput(gpioPin);
    }
}

/**
 * Write a HIGH or a LOW value to a pin
 * pin: the pin number (gpio pin)
 * value: HIGH or LOW
 */
void digitalWrite (int pin, int value)
{
    int gpioPin = pinTable[pin].gpio;
    pinMode(pin, OUTPUT);
    gpioSetValue(gpioPin, value);
}

/** 
 * Reads the value from a specified digital pin, either HIGH or LOW
 * pin: the number of the digital pin you want to read
 */
int digitalRead (int pin)
{
    int gpioPin = pinTable[pin].gpio;
    pinMode(gpioPin, INPUT);
    // See defined error values in udooConfig.h for invalid/unexported pin that can be returned 
    return gpioGetValue(gpioPin);
}


/**************************************************************************************************
 * 3. Analog I/O 
 * 4. PWMs: PWM1, PWM2, PWM3, PWM4
 *************************************************************************************************/

/*
 * Reads the value from the specified analog pin
 * UDOO board contains 11 analog pins: A0 ... A11
 */
int analogRead (int pin)
{
    #ifndef FIRMATA
        printf(" Sorry, you do not have access to analogRead()\n");
        printf(" In order to use analogRead(), you must first enable Firmata.\n");
        return 0;
    #endif
    if (pin < A0 || pin > CANTX) {
        debug("Pin %d is not analog", pin);
        return NOT_ANALOG_PIN_ERROR;
    }
    if (firmata->pins[pin].mode != MODE_ANALOG)
        firmata_pinMode(firmata, pin, MODE_ANALOG);
    firmata_pull(firmata);
    return firmata->pins[pin].value;
}

/*
 * Writes an analog value (PWM wave) to a pin
 * Can be used to light a LED at varying brightnesses or drive a motor at various speed
 * After a call to analogWrite(), the pin will generate a steady square wave of the specified 
 * duty cycle until the next call to analogWrite().
 */
void analogWrite (int pin, int value)
{
    if (value > 255)
        value = 255;
    else if (value < 0)
        value = 0;
    #ifndef FIRMATA
        printf(" Sorry, you do not have access to analogWrite()\n");
        printf(" In order to use analogWrite() you must first enable Firmata\n");
        return;
    #endif
    if (firmata->pins[pin].mode != MODE_PWM)
        firmata_pinMode(firmata, pin, MODE_PWM);
    firmata_analogWrite(firmata, pin, value);
}


/**************************************************************************************************
 * 5. Advanced I/O
 *************************************************************************************************/

/*
 * Shifts in a byte of data one bit at a time.
 * Starts either from the most or least significant bit.
 * For each bit the clock pin is pulled high, the next bit is read from the data line, and then
 * the clock pin is taken low. 
 */
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
    uint8_t value = 0;
    uint8_t i;
    for (i = 0; i < 8; i++) {
        digitalWrite(clockPin, HIGH);
        // TODO: handle case if digitalRead() returns error codes
        if (bitOrder == LSBFIRST) {
            value |= digitalRead(dataPin) << i;
        } else {
            value |= digitalRead(dataPin) << (7 - i);
        }
        digitalWrite(clockPin, LOW);
    }
    return value;
}

/*
 * Shifts data out to a clocked source
 * Each bit is written in turn to a data pin after wich a clock pin is pulsed (taken high, 
 * the low), to indicate that the bit is available.
 */
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
    uint8_t i;
    for (i = 0; i < 8; i++) {
        if (bitOrder == LSBFIRST) {
            digitalWrite(dataPin, !!(val & (1 << i)));
        } else {
            digitalWrite(dataPin, !!(val & (1 << (7 - i))));
        }
        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
    }
}


/**************************************************************************************************
 * 6. Time
 *************************************************************************************************/

/*
 * Pauses the program for the amount of time (in milliseconds) specified as parameter
 * There are 1000 milliseconds in 1 second
 */
void delay (unsigned int ms)
{
/*
 * Suspend execution of the calling thread for (at least) ms microseconds
 */
    usleep(ms * 1000);
}

/*
 * Pauses the program for the amount of time (in microseconds) specified
 * as parameter
 */
void delayMicroseconds (unsigned int mcs)
{
    usleep(mcs);
}

/*
 * Returns the number of microseconds since the UDOO board began running the current program
 * This number will go back to zero after some time.
 * Note: there are 1000 microseconds in 1 millisecond and 1000000 microseconds in 1 second
 */

unsigned int micros (void)
{
/*    struct timespec tm;
    int ret = clock_gettime(CLOCK_REALTIME, &tm);
    if (ret == -1) {
        debug("Clock Time error");
        return CLOCK_TIME_ERROR;
    }
    return (unsigned long)(tm.tv_sec * 1000000L + tm.tv_nsec / 1000L);
*/
    return 0;
}

unsigned int millis (void)
{
//    return micros() / 1000;
    return 0;
}


/**************************************************************************************************
 * 8. I2C
 *************************************************************************************************/

int i2c_openadapter(uint8_t i2c_bus)
{
    if (i2c_bus < 0 || i2c_bus > 2) {
        debug("i2c_bus %d unknown", i2c_bus);
        return 0;
    }
    char filepath[32];
    snprintf(filepath, 32, "/dev/i2c-%u", i2c_bus);
    i2c_buses[i2c_bus] = open(filepath, O_RDWR);
    printf("file descriptor: %d\n", i2c_buses[i2c_bus]);
    if (i2c_buses < 0) {
        debug("Failed to open requested port %s", filepath);
        perror("i2c_openadapter");
    }
    return i2c_buses[i2c_bus];
}

int i2c_setslave(int i2c_fd, uint8_t addr)
{
    int res = ioctl (i2c_fd, I2C_SLAVE_FORCE, addr);
    if (res < 0)
        perror("i2c_setslave:");
    return 0;
}

/* \brief Write an immediate byte to the SMBus.
 * \param i2c_id     Bus id
 * \param byte       Byte value to write
 */
int i2c_writebyte(int i2c_fd, uint8_t byte)
{
    struct i2c_smbus_ioctl_data args;

    args.read_write = I2C_SMBUS_WRITE;
    args.command = byte;
    args.size = I2C_SMBUS_BYTE;
    args.data = NULL;

    if (ioctl(i2c_fd, I2C_SMBUS, &args) < 0) {
        perror("Failed to write one byte to i2c:");
    }
    return 0;
}


int i2c_writebytes(int i2c_fd, uint8_t *bytes, uint8_t length)
{
/*    union i2c_smbus_data data;
    int i;

    length = length - 1;
    if (length > I2C_SMBUS_I2C_BLOCK_MAX) {
        length = I2C_SMBUS_I2C_BLOCK_MAX;
    }
    for (i = 1; i <= length; i++) {
        data.block[i] = bytes[i];
    }
    data.block[0] = length;

    struct i2c_smbus_ioctl_data args;

    args.read_write = I2C_SMBUS_WRITE;
    args.command = bytes[0];
    args.size = I2C_SMBUS_BLOCK_DATA;
    args.data = &data;

    if (ioctl(i2c_fd, I2C_SMBUS, &args) < 0) {
        perror("Failed to write more bytes to i2c:");
    }
    return 0;
*/
    
    if (write(i2c_fd, bytes, length) != length) {
        perror("could not write more bytes to i2c bus");
        return -1;
    }
    return 0;
}

/*
 * \brief  Read an immediate byte from SMBus
 * \param  i2c_id   The Bus is
 * \return   -1 in case of error
 *           the databyte itself otherwise
 */ 
int i2c_readbyte(int i2c_fd)
{
    union i2c_smbus_data data;
    int rc;

    struct i2c_smbus_ioctl_data args;

    args.read_write = I2C_SMBUS_READ;
    args.command = I2C_NOCMD;
    args.size = I2C_SMBUS_BYTE;
    args.data = &data;

    rc = ioctl(i2c_fd, I2C_SMBUS, &args);
    if (rc < 0) {
        perror("Failed to write more bytes to i2c:");
        return -1;
    }
    return (0x0FF & data.byte);
}


int i2c_readbytes(int i2c_fd, uint8_t *buf, int length)
{
    // Read a max of I2C_SMBUS_I2C_BLOCK_MAX bytes
    union i2c_smbus_data data;
    int  i;
    int rc;

    struct i2c_smbus_ioctl_data args;

    args.read_write = I2C_SMBUS_READ;
    args.command = I2C_NOCMD;
    args.size = I2C_SMBUS_BLOCK_DATA;
    args.data = &data; 

    rc = ioctl(i2c_fd, I2C_SMBUS, &args);

    if (rc >= 0) {
        for (i = 1; i <= data.block[0]; i++) {
            buf[i - 1] = data.block[i];
        }
        length = data.block[0];
        return length;
    }
    return -1;
}

int i2c_readwrite (int i2c_id)
{
    return 0;
}

int i2c_closeadapter(int i2c_fd)
{
    int rc = close(i2c_fd);
    return rc;
}


/******************************************************************************************************
 * 9. SPI
 ******************************************************************************************************/

int spi_getadapter (uint32_t spi_bus_address)
{
    return 0;
}

int spi_openadapter (uint8_t spi_bus)
{
    return 0;
}
int spi_setmode (int spiId, unsigned short mode)
{
    return 0;
}

int spi_set_frequency (int spiId, int freq)
{
    return 0;
}

uint8_t spi_writebyte (int spiId, uint8_t byte)
{
    return 0;
}

unsigned char *spi_writebytes (int spiId, uint8_t *bytes, uint8_t length)
{
    return NULL;
}

int spi_lsb_mode (int spiId, unsigned char lsb)
{
    return 0;
}

int spi_bit_per_word (int spiId, unsigned int bits)
{
    return 0;
}

int spi_closeadapter (int spiId)
{
    return 0;
}

int serial_openadapter (char *serial_bus)
{
    return 0;
}

int serial_set_speed (int serial_id, int baud)
{
    return 0;
}

int serial_bytes_available (int serial_id)
{
    return 0;
}

int serial_closeadapter (int serial_id)
{
    return 0;
}

int serial_writebyte (int serial_id, uint8_t byte)
{
    return 0;
}

int serial_writebytes (int serial_id, uint8_t *bytes, uint8_t length)
{
    return 0;
}

uint8_t serial_readbyte (int serial_id)
{
    return 0;
}

int serial_readbytes (int serial_id, uint8_t *buf, int length)
{
    return 0;
}

int serial_flush (int serial_id)
{
    return 0;
}

  
#ifdef __cplusplus
}
#endif


#endif /* UDOO */
