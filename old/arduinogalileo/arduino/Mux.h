/*
 * Copyright(c) 2013 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Contact Information:
 * Intel Corporation
 */

#ifndef __MUX_H__
#define __MUX_H__

#include <Arduino.h>		// Contains types
#include <wiring_digital.h>

#ifdef __cplusplus
extern "C" {
#endif

int muxInit(void);
int muxSelectDigitalPin(uint8_t pin);
int muxSelectAnalogPin(uint8_t pin);
int muxSelectUart(uint8_t interface);
int muxSelectSpi(uint8_t interface);
int muxSelectI2c(uint8_t interface);

void pin2alternate(PinDescription **p_ptr);

#ifdef __cplusplus
}
#endif

#endif /* __MUX_H__ */

