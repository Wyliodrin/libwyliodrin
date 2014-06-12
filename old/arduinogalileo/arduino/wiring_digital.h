#ifndef _WIRING_DIGITAL_
#define _WIRING_DIGITAL_

#include <stdint.h>
#include "variant.h"		// For pin description structure
#include "fast_gpio_sc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NONE					0xFFFFFFFF
#define	FN_GPIO_FIXED_INPUT		0x01
#define FN_GPIO_FIXED_OUTPUT	0x02
#define FN_GPIO_OPENDRAIN		0x04
#define FN_PWM					0x08
#define FN_I2C					0x10
#define FN_ANALOG				0x20
#define FN_UART					0x40
#define FN_SPI					0x80
#define FN_PULLUP				0x100
#define FN_MUX					0x200	// Used to mux functions in/out
#define FN_RESERVED				0x400	// Reserved - not to be touched by sketch layer

#define GPIO_DRIVE_PULLUP			0
#define GPIO_DRIVE_STRONG			1
#define GPIO_DRIVE_HIZ				2

#define PIN_EINVAL 0xFFFFFFFF

// Describes an individual MUX setting
typedef struct mux_select {
	uint32_t 		ulGPIOId;	// GPIOLib ID that controls the mux
	uint8_t 		u8Value;		// HIGH or LOW
	uint32_t		tFunction;	// Function in the HIGH or LOW state
}mux_sel_t;

#define MAX_GPIO_PATH 0x200

/* Types used for the tables below */
typedef struct _PinDescription
{
	uint32_t		ulGPIOId;			// Identitiy in GPIOLib as a GPIO
	uint32_t		ulGPIOAlias;			// Alias pin
	uint32_t		ulFastIOBit;			// The bit to write in fast IO mode - if supported
	uint32_t		ulArduinoId;			// Arduino ID if any
	uint32_t		ulInitialPinSel;		// Initial Mux descriptor to apply to this pin
	uint32_t		ulFixedState;			// Fixed to HIGH, LOW, NONE
	mux_sel_t		*ptMuxDesc;			// Describes possible muxes on this pin
	uint32_t		ulMuxDescEntries;		// sizeof ptMuxDesc
	uint32_t		tCurrentType;			// Current State/function of pin
	int			iHandle;			// Persistent handle - open once - use many times
	int			iAlternate;			// If set, switch to the alternate func (pAlternate)
	struct _PinDescription	*pAlternate;			// Pointer to alternate function (NULL if none)
	char			sPath[MAX_GPIO_PATH];	// Path to GPIO - required to poll/select on a gpio in user-space
} PinDescription;

#define MUX_SIZE(x) sizeof(x)/sizeof(mux_sel_t)

typedef struct _PinState
{
	uint32_t	uCurrentPwm;	/* True if currently used as PWM */
	uint32_t	uCurrentInput;	/* True if currently input */
} PinState;

void pinInit(void);
void pinMode(uint8_t pin, uint8_t mode);
int pinModeIRQ(uint8_t pin, int8_t mode);
void digitalWrite(register uint8_t pin, register uint8_t val);
int digitalRead(uint8_t pin);

// interrupt.c dependencies
int pin2handle(uint8_t pin);
int pinHandleReopen(uint8_t index);
int pinGetIndex(uint8_t pin);
char * pin2path(uint8_t pin);
int gpio2gpiohandle(uint32_t gpio);
int pin2gpiohandle(uint8_t pin);

#ifdef __cplusplus
} // extern "C"
#endif

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.
//
#define digitalPinToPort(P)        ( g_APinDescription[P].pPort )
#define digitalPinToBitMask(P)     ( g_APinDescription[P].ulPin )
#define digitalPinToTimer(P)       (  )
#define portOutputRegister(port)   ( &(port->PIO_ODSR) )
#define portInputRegister(port)    ( &(port->PIO_PDSR) )

#endif /* _WIRING_DIGITAL_ */

