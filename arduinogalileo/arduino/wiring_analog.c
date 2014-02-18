/*
 Copyright (c) 2011 Arduino.  All right reserved.
 Copyright (c) 2013 Intel Corporation.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <trace.h>

#include "Arduino.h"
#include "pins_arduino.h"

#define MY_TRACE_PREFIX "wiring_analog"

/* By default, analogRead() should assume a 10-bit ADC resolution
 * However, this can be overriden using analogReadResolution()
 * The Kips Bay ADC supports a 12-bit resolution.
 * */
static uint32_t _readResolution = 10;
static uint32_t _writeResolution = 8;
static uint8_t analog_reference = DEFAULT;

static const int pin2pwm(uint8_t pin)
{
	uint32_t i;

	// Scan mappings
	for (i = 0; i < sizeof_g_APwmDescription; i++){
		if(g_APwmDescription[i].ulArduinoId == pin)
			return g_APwmDescription[i].ulPWMId;
	}

	// Indicate error
	return PIN_EINVAL;
}

int pin2pwmhandle_enable(uint8_t pin)
{
	uint32_t i;

	// Scan mappings
	for (i = 0; i < sizeof_g_APwmDescription; i++){
		if(g_APwmDescription[i].ulArduinoId == pin)
			return g_APwmDescription[i].iHandleEnable;
	}

	// Indicate error
	return PIN_EINVAL;
}

static int pin2pwmhandle_duty(uint8_t pin)
{
	uint32_t i;

	// Scan mappings
	for (i = 0; i < sizeof_g_APwmDescription; i++){
		if(g_APwmDescription[i].ulArduinoId == pin)
			return g_APwmDescription[i].iHandleDuty;
	}

	// Indicate error
	return PIN_EINVAL;
}

void analogReadResolution(uint32_t res)
{
	if (res >= 1 && res <= 32)
		_readResolution = res;
}

void analogWriteResolution(int res)
{
	_writeResolution = res;
}

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to)
{
	if (from == to)
		return value;
	if (from > to)
		return value >> (from-to);
	else
		return value << (to-from);
}

void analogReference(uint8_t mode)
{
	/* Retained for backward compatibilty, only DEFAULT is supported */
}

uint32_t analogRead(uint32_t ulPin)
{
	char path[50] = "";
	char strValue[16] = "";
	uint8_t channel;
	uint32_t ulValue;
	FILE *fp = NULL;

	if (ulPin < NUM_ANALOG_INPUTS)
		channel = ulPin;
	else
		channel = ulPin - A0;

	if (channel > NUM_ANALOG_INPUTS) {
		trace_error("Invalid analog input channel specified\n");
		return 0;
	}

	snprintf(path, sizeof(path), LINUX_ADC_FMT, channel);
	fp = fopen(path, "r");
	if (NULL == fp) {
		trace_error("Failed to read from analog input channel %u\n", channel);
		return 0;
	}

	muxSelectAnalogPin(channel);

	fread(strValue, sizeof(strValue), 1, fp);

	muxSelectDigitalPin(analogInputToDigitalPin(channel));

	fclose(fp);
	
	ulValue = atoi(strValue);

	/* Kips Bay has a 12-bit ADC. Adjust for desired resolution */
	ulValue = mapResolution(ulValue, ADC_RESOLUTION, _readResolution);

	return ulValue;
}

void analogWrite(uint32_t ulPin, uint32_t ulValue)
{
	int ret = 0;

	trace_debug("%s ulPin=%u ulValue=%u", __func__, ulPin, ulValue);

	if (! digitalPinHasPWM(ulPin))
	{
		trace_error("pin%u has no pwm", ulPin);
		return;
	}

	if (0 == g_APinState[ulPin].uCurrentPwm) {
		trace_debug("%s: turning on pwm%u", __func__, ulPin);
		turnOnPWM(ulPin);
	}

	ret = sysfsPwmEnable(pin2pwmhandle_enable(ulPin),
			     pin2pwmhandle_duty(ulPin),
			     ulValue);
	if (ret) {
		trace_error("%s: enable err=%d", __func__, ret);
	}
}

void pwmInit(void)
{
	int i = 0;
	int ret = 0;

	for (i = 0; i < sizeof_g_APwmDescription; i++) {
		ret = sysfsPwmExport(g_APwmDescription[i].ulPWMId,
				     &g_APwmDescription[i].iHandleEnable,
				     &g_APwmDescription[i].iHandleDuty);
		if (ret < 0) {
			trace_error("unable to open pwm%d",
				    g_APwmDescription[i].ulPWMId);
		}
		ret = sysfsPwmDisable(g_APwmDescription[i].iHandleEnable);
		if (ret < 0) {
			trace_error("unable to disable pwm%d",
				    g_APwmDescription[i].ulPWMId);
		}

		trace_debug("i=%d ulPWMId=%u, iHandleEnable=%p, iHandleDuty=%p",
			    i, g_APwmDescription[i].ulPWMId,
			    g_APwmDescription[i].iHandleEnable,
			    g_APwmDescription[i].iHandleDuty);
	}

	/* No need to set any muxing: done by pinMode.  */

	return 0;
}

#ifdef __cplusplus
}
#endif
