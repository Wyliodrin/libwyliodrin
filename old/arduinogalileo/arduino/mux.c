/*
 * HwMUX.cpp
 *
 * High level abstraction for muxing across boards
 */

#include <Arduino.h>		// Contains types
#include <errno.h>			// -EINVAL and friends
#include <Mux.h>
#include "variant.h"		// Contains board specific data to drive muxing options
#include "sysfs.h"
#include <trace.h>

#define MY_TRACE_PREFIX "mux"

#if 0
	// valid values
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
#endif

/* If alternate function is enabled, skip to it */
void pin2alternate(PinDescription **p_ptr)
{
	PinDescription *p = *p_ptr;

	for (; p && p->iAlternate && p->pAlternate; p = p->pAlternate) {
//		trace_debug("%s: arduino_pin=%u: skipping to "
//			    "alternate function (gpio%d)", __func__,
//			    p->pAlternate->ulArduinoId,
//			    p->pAlternate->ulGPIOId);
	}

	*p_ptr = p;
}

/**
 * muxSelect
 *
 * Does the nasty business of iterating through the array matching GPIO ids and setting muxing based on tFunction against
 * given arduino_pin
 */
static int muxSelect(uint8_t arduino_pin, uint32_t tFunction)
{
	int i = 0, j = 0, k = 0, matched = 0;
	mux_sel_t sel;
	PinDescription *p = NULL;
	int ret = -EINVAL;

	for (i = 0; i < sizeof_g_APinDescription; i++) {
		p = &g_APinDescription[i];

		/* Find Arduino pin mapping */
		if (p->ulArduinoId == arduino_pin){

			/* If alternate function is enabled, skip to it */
			for (; p && p->iAlternate && p->pAlternate;
			     p = p->pAlternate) {
				trace_debug("%s: arduino_pin=%u: skipping to "
					    "alternate function (gpio%d)",
					    __func__,
					    p->pAlternate->ulArduinoId,
					    p->pAlternate->ulGPIOId);
			}
			if (p->ptMuxDesc == NULL){
				/* No muxing options for this pin */
				return 0;
			}

			/* Find possible mux entries */
			for ( j = 0; j < p->ulMuxDescEntries; j++){

				sel = p->ptMuxDesc[j];
				if (sel.tFunction & tFunction){

					/* Bit mask matched - set state as indicated */
					for (k = 0; k < sizeof_g_APinDescription; k++){
						if (g_APinDescription[k].ulGPIOId == sel.ulGPIOId){
							trace_info("%s: mux_sel gpio%u:=%u (arduino_pin=%u)",
								__func__, sel.ulGPIOId, sel.u8Value, arduino_pin);
							ret = sysfsGpioSet(g_APinDescription[k].iHandle, sel.u8Value);
							matched = 1;
							break;
						}
					}
				}
			}
			break;
		}
	}
	return matched ? ret : -EINVAL;
}

int muxSelectAnalogPin(uint8_t pin)
{
	int i = 0;

	if (pin >= NUM_ANALOG_INPUTS) {
		return -EINVAL;
	}

	//trace_debug("%s(%u): gpio%u:=%u", __func__, pin, mux.sel_id, mux.sel_val);

	i = muxSelect(mux_sel_analog[pin], FN_ANALOG);
	if (i < 0)
		return i;

	return 0;
}

int muxSelectUart(uint8_t interface)
{
	int i = 0, ret = 0;

	if (interface >= NUM_UARTS){
		return -EINVAL;
	}

	// Require both bins - to be described
	if (MUX_SEL_NONE == mux_sel_uart[interface][0] && MUX_SEL_NONE == mux_sel_uart[interface][0]) {
		return 0;	// No muxing to be done
	}

	for ( i = 0; i < MUX_DEPTH_UART; i++){
		ret = muxSelect(mux_sel_uart[interface][i], FN_UART);
		if (ret < 0)
			return ret;
	}
	return ret;
}

int muxSelectSpi(uint8_t interface)
{
	int i = 0, ret = -EINVAL;

	if (interface >= NUM_SPI){
		return -EINVAL;
	}

	for ( i = 0; i < MUX_DEPTH_SPI; i++){
		if (mux_sel_spi[interface][i] == MUX_SEL_NONE)
			continue;
		ret = muxSelect(mux_sel_spi[interface][i], FN_SPI);
		if (ret < 0)
			return ret;
	}
	return ret;
}

int muxSelectI2c(uint8_t interface)
{
	/* Selecting SCL will automatically route out SDA too.  */
	return muxSelect(SCL, FN_I2C);
}

int muxSelectDigitalPin(uint8_t pin)
{
	return muxSelect(pin, FN_GPIO_FIXED_INPUT | FN_GPIO_FIXED_OUTPUT | FN_GPIO_OPENDRAIN );
}

int muxInit(void)
{
	int i = 0, j = 0;
	mux_sel_t sel;
	PinDescription *p = NULL;

	/*
	 * Setup initial muxes as indicated by controller array
	 */
	for (i = 0; i < sizeof_g_APinDescription; i++) {
		p = &g_APinDescription[i];

		/* If alternate function is enabled, skip to it */
		for (; p && p->iAlternate && p->pAlternate; p = p->pAlternate) {
			trace_debug("%s: arduino_pin=%u: skipping to "
				    "alternate function (gpio%d)", __func__,
				    p->pAlternate->ulArduinoId,
				    p->pAlternate->ulGPIOId);
		}

		/* Skip past any GPIO entry that doesn't have a persistent handle or a relevant mux entry */
		if (p->iHandle < 0 || g_APinDescription[i].ptMuxDesc == NULL || g_APinDescription[i].ulInitialPinSel == NONE)
			continue;

		trace_debug("%s: setting up mux for gpio%d", __func__, p->ulGPIOId);

		if (p->ulInitialPinSel < g_APinDescription[i].ulMuxDescEntries){

			sel = p->ptMuxDesc[g_APinDescription[i].ulInitialPinSel];

			for (j = 0; j < sizeof_g_APinDescription; j++){
				if (g_APinDescription[j].ulGPIOId == sel.ulGPIOId){
					sysfsGpioSet(g_APinDescription[j].iHandle, sel.u8Value);
					trace_debug("%s: set gpio%u:=%u",
						__func__, sel.ulGPIOId, sel.u8Value);
					break;
				}
			}
		}

	}
	return 0;
}
