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
 * 3.  Advanced I/O
 * 4.  Time
 * 5.  Communication (Serial, Stream)
 * 6.  UARTs: uart1, uart3, uart4, uart5
 * 7.  SD1
 * 8.  SPIs: spi1, spi2, spi5
 * 9.  I2C1
 * 10. Spdif
 * 11. Timer capture
 * 12. Timer compare
 * 13. WATCHDOG FUNCTIONALITIES: watchdog reset, watchdog out
 * 14. Clock out
 * 15. PWMs: pwm1, pwm2, pwm3, pwm4
 * 16. Digital audio 
 *************************************************************************************************/

#ifdef UDOO

#ifdef __cplusplus
extern "C" {
#endif


#include <time.h>
#include "wiring.h"
#include "udooConfig.h"
// Includes Atmel CMSIS
#include <chip.h> 


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
    gpioExport(pin);
    if (mode == INPUT) {
        gpioSetDirInput(pin);
    } else {
        gpioSetDirOutput(pin);
    }
}

/**
 * Write a HIGH or a LOW value to a pin
 * pin: the pin number (gpio pin)
 * value: HIGH or LOW
 */

void digitalWrite (int pin, int value)
{
    gpioSetValue(pin, value);
}

/** 
 * Reads the value from a specified digital pin, either HIGH or LOW
 * pin: the number of the digital pin you want to read
 */

int digitalRead (int pin)
{
    // See defined error values in udooConfig.h for invalid/unexported pin that can be returned 
    return gpioGetValue(pin);
}



/**************************************************************************************************
 3. Analog I/O
 *************************************************************************************************/

static int _readResolution = 10;   // 10 bit resolution
static int _writeResolution = 8;   // 8 bit resolution

static inline uint32_t mapResolution (uint32_t value, uint32_t from, uint32_t to) {
    if (from == to)
        return value;
    if (from > to)
        return value >> (from - to);
    else
        return value << (to - from);
}

eAnalogReference analog_reference = AR_DEFAULT;

/*
 * \brief Configures the reference voltage used for analog input (i.e. the value used as top of the 
 * input range)
 * \param arefMode should be set to AR_DEFAULT
 */
void analogReference (eAnalogReference arefMode)
{
    analog_reference = arefMode;
}

uint32_t analogRead (uint32_t myPin)
{
    uint32_t myPinValue = 0;
    uint32_t myPinChannel;

    if (myPin < A0 || myPin > CANTX) {
        debug("%d is not an Analog Pin", myPin);
        return NOT_ANALOG_PIN_ERROR;
    }

        
    myPinChannel = analogPin[myPin].ADCChannelNumber;
    switch (analogPin[myPin].analogChannel) {
        // handling ADC 12  bits channels
        case ADC0:
        case ADC1:
        case ADC2:
        case ADC3:
        case ADC4:
        case ADC5:
        case ADC7:
        case ADC8:
        case ADC9:
        case ADC10:
        case ADC11:

                // Enable the corresponding channel
                adc_enable_channel(ADC, myPinChannel);

                // Start the ADC (analog digital converter)
                adc_start();

                /* Wait for end of conversion && check the ADC conversion status
                 * ADC_ISR_DRDY is a constant that suggest data is ready
                 * ISR = Interrupt Status Register
                 */
                while ( (adc_get_status(ADC) & ADC_ISR_DRDY) != ADC_ISR_DRDY) 
                    ;

                // Read the value
                myPinValue = adc_get_latest_value(ADC);
                myPinValue = mapResolution(myPinValue, ADC_RESOLUTION, _readResolution);

                // Disable the corresponding channel
                adc_disable_channel(ADC, myPinChannel);

                break;

        default:
                myPinValue = 0;
                break;
    }
    return myPinValue;
}

#ifdef __cplusplus
}
#endif

#endif /* UDOO */
