#ifndef __ANALOG_IO_H__
#define __ANALOG_IO_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Types used for the tables below */
typedef struct _PwmDescription
{
	uint32_t		ulPWMId;		// Identitiy in PWMLib as a PWM
	uint32_t		ulArduinoId;		// Arduino ID
	int			iHandleEnable;		// Persistent handle - open once - use many times
	int			iHandleDuty;		// Persistent handle - open once - use many times
} PwmDescription;

void pwmInit(void);
uint32_t analogRead(uint32_t);
void analogReadResolution(uint32_t res);
void analogReference(uint8_t mode);
void analogWrite(uint32_t, uint32_t);

int pin2pwmhandle_enable(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif /* __ANALOG_IO_H__ */
