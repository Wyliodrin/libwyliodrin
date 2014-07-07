/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * Copyright (c) 2013 Intel Corporation

 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include <sys/stat.h> 
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include "../wiring/wiring.h"

#include "SPI.h"

#define MY_TRACE_PREFIX "SPI"

 #define trace_error printf

/* For Arduino, this is assumed to be 4MHz, using SPI_CLOCK_DEV4
 * Sticking to this for backward compatibility */
#define SPI_CLK_DEFAULT_HZ 4000000

#define SPI_SS_GPIO_PIN   10

SPIClass SPI;

/* Constructor - establish defaults */
SPIClass::SPIClass()
{
  /* reflect Arduino's default behaviour where possible */
  this->mode = SPI_MODE0;
  this->bitOrder = MSBFIRST;
  this->clkDiv = SPI_CLOCK_DIV4;
}

void SPIClass::begin()
{
  /* Set the pin mux, for the SCK, MOSI and MISO pins ONLY
   *
   * Leave the SS pin in GPIO mode (the application will control it)
   * but set it's direction to output and initially high
   */

   this->fd = spi_openadapter (0);

  /* Load default/last configuration */
  this->setClockDivider(this->clkDiv);
  this->setBitOrder(this->bitOrder);
  this->setDataMode(this->mode);
}


void SPIClass::end()
 {
  if (this->fd >= 0)
    spi_closeadapter (this->fd);
}

void SPIClass::setBitOrder(uint8_t bitOrder)
{
  uint8_t lsbFirst = 0;
  
  if (bitOrder == LSBFIRST) {
    lsbFirst = 1;
  }
  
  spi_lsb_mode (this->fd, lsbFirst);

  this->bitOrder = bitOrder;
}

void SPIClass::setDataMode(uint8_t mode)
{
  uint8_t linuxSpiMode = 0;

  switch(mode) {
  case SPI_MODE0:
    linuxSpiMode = SPI_MODE0;
    break;
  case SPI_MODE1:
    linuxSpiMode = SPI_MODE1;
    break;
  case SPI_MODE2:
    linuxSpiMode = SPI_MODE2;
    break;
  case SPI_MODE3:
    linuxSpiMode = SPI_MODE3;
    break;
  default:
    trace_error("Invalid SPI mode specified\n");
    return;
  }

  spi_setmode (this->fd, linuxSpiMode);

  this->mode = mode;
}

void SPIClass::setClockDivider(uint8_t clkDiv)
{
  uint32_t maxSpeedHz = SPI_CLK_DEFAULT_HZ;

  /* Adjust the clock speed relative to the default divider of 4 */
  switch(clkDiv)
  {
  case SPI_CLOCK_DIV2:
    maxSpeedHz = SPI_CLK_DEFAULT_HZ << 1;
    break;
  case SPI_CLOCK_DIV4:
    /* Do nothing */
    break;
  case SPI_CLOCK_DIV8:
    maxSpeedHz = SPI_CLK_DEFAULT_HZ >> 1;
    break;
  case SPI_CLOCK_DIV16:
    maxSpeedHz = SPI_CLK_DEFAULT_HZ >> 2;
    break;
  case SPI_CLOCK_DIV32:
    maxSpeedHz = SPI_CLK_DEFAULT_HZ >> 3;
    break;
  case SPI_CLOCK_DIV64:
    maxSpeedHz = SPI_CLK_DEFAULT_HZ >> 4;
    break;
  case SPI_CLOCK_DIV128:
    maxSpeedHz = SPI_CLK_DEFAULT_HZ >> 5;
    break;
  default:
    trace_error("Invalid SPI mode specified\n");
    return;
  }

 spi_set_frequency (this->fd, maxSpeedHz);

  this->clkDiv = clkDiv;
}

uint8_t SPIClass::transfer(uint8_t txData)
{
  char s = spi_writebyte (this->fd, txData);
  return s;
}

void SPIClass::transferBuffer(uint8_t *txData,
            uint8_t *rxData,
            uint32_t len)
{
  uint8_t * data = spi_writebytes (this->fd, txData, len);
  memcpy (rxData, data, len);
  free (data);
}

void SPIClass::attachInterrupt() {
  trace_error("SPI slave mode is not currently supported\n");
}

void SPIClass::detachInterrupt() {
  /* Do nothing */
}

