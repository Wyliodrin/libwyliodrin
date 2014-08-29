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
 * 8.  I2C     // IN PROGRESS
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

//#ifdef UDOO

/*#ifdef __cplusplus
extern "C" {
#endif
*/

#include <time.h>
#include "wiring.h"
#include "udooConfig.h"
#include <linux/i2c-dev.h>
#include <pthread.h>

#ifdef __FIRMATA__
#include "firmata.h"
#endif


/**************************************************************************************************
 * 1. General configuration
 *************************************************************************************************/

static int i2c_buses[MAX_UDOO_BUSES];
static int i2c_addresses[MAX_UDOO_BUSES];

#ifdef __FIRMATA__

t_firmata *initFirmata (t_firmata *firmata)
{
    int i = 0;
    char str[] = "/dev/ttymxc3";
    firmata = firmata_new(str);     // init Firmata
    while (!firmata->isReady)       // wait until device is up
        firmata_pull(firmata);
    return firmata;
}

t_firmata *firmata = initFirmata(firmata);

#endif

int wiringSetup ()
{
    // TODO
    int id;
    for (id = 0; id <= MAX_UDOO_BUSES; id++) i2c_buses[id] = -1;
    return 0;
}

// pthread_mutex_t locki2c;

// int getI2CId ()
// {
//     int i;
//     int id = -1;
//     pthread_mutex_lock(&locki2c);
//     for (i = 0; i < MAX_I2C_BUSES && id == -1; i++)
//     {
//         if (i2c_buses[i] == -1)
//         {
//             id = i;
//             i2c_buses[id] = 0;
//         }
//     }
//     pthread_mutex_unlock(&locki2c);
//     return id;
// }

// void releaseI2CId (int id)
// {
//     pthread_mutex_lock(&locki2c);
//     i2c_buses[id] = -1;
//     pthread_mutex_unlock(&locki2c);
// }


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


#ifdef __FIRMATA__

/*
 * Reads the value from the specified analog pin
 * UDOO board contains 11 analog pins: A0 ... A11
 */
int analogRead (int pin)
{
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
    if (firmata->pins[pin].mode != MODE_PWM)
        firmata_pinMode(firmata, pin, MODE_PWM);
    firmata_analogWrite(firmata, pin, value);
}

#endif


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
/*
unsigned long micros (void)
{
    struct timespec tm;
    int ret = clock_gettime(CLOCK_REALTIME, &tm);
    if (ret == -1) {
        debug("Clock Time error");
        return CLOCK_TIME_ERROR;
    }
    return (unsigned long)(tm.tv_sec * 1000000L + tm.tv_nsec / 1000L);
}

unsigned long milis (void)
{
    return micros() / 1000;
}
*/


/**************************************************************************************************
 * 7. Servo
 *************************************************************************************************/

#ifdef __FIRMATA__

/* IMPORTANT!!!
 * how servo should be called:
 * t_servo *servo = servo_attach(pin)
 * servo_write(servo, value)
 */

t_servo *servo_attach (int pin)
{
    t_servo *myServo;
    if (!firmata || !firmata->isReady) {
        perror("servo_new: Firmata is not ready\n");
        return NULL;
    }
    myServo = (t_servo*)malloc(sizeof(t_servo));
    if (!myServo) {
        perror("servo_new: malloc failed\n");
        return NULL;
    }
    myServo->firmata = firmata;
    myServo->pin = pin;
    firmata_pinMode(myServo->firmata, pin, MODE_SERVO);
    return myServo;
}

int servo_write (t_servo *servo, int value)
{
    return (firmata_analogWrite(servo->firmata, servo->pin, value));
}

#endif


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
    if (i2c_buses < 0) {
        debug("Failed to open requested port %s", filepath);
        perror("i2c_openadapter");
    }
    return 0;
}

int i2c_setslave(int i2c_id, uint8_t addr)
{
    i2c_addresses[i2c_id] = addr;
    int res = ioctl (i2c_buses[i2c_id], I2C_SLAVE_FORCE, addr);
    if (res < 0)
        perror("i2c_setslave:");
    return 0;
}

/* \brief Write an immediate byte to the SMBus.
 * \param i2c_id     Bus id
 * \param byte       Byte value to write
 */
int i2c_writebyte(int i2c_id, uint8_t byte)
{
    struct i2c_smbus_ioctl_data args;

    args.read_write = I2C_SMBUS_WRITE;
    args.command = byte;
    args.size = I2C_SMBUS_BYTE;
    args.data = NULL;

    if (ioctl(i2c_buses[i2c_id], I2C_SMBUS, &args) < 0) {
        perror("Failed to write one byte to i2c:");
    }
    return 0;
}

int i2c_writebytes(int i2c_id, uint8_t *bytes, uint8_t length)
{
    union i2c_smbus_data data;
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

    if (ioctl(i2c_buses[i2c_id], I2C_SMBUS, &args) < 0) {
        perror("Failed to write more bytes to i2c:");
    }
    return 0;
}

int i2c_readbyte(int i2c_id);
int i2c_readbytes(int i2c_id, uint8_t *buf, int length);
int i2c_closeadapter(int i2c_id);


/*  
#ifdef __cplusplus
}
#endif
*/

//#endif /* UDOO */
