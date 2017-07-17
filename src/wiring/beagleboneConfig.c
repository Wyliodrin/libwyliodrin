/**************************************************************************************************
 * Author: Razvan Madalin MATEI <matei.rm94@gmail.com>
 * Date last modified: August 2014
 *
 * This file contains the definitions of all configuration functions for the BeagleBone Black.
 *
 * CONTENT
 * 1.Pins configuration table
 * 2.General
 * 3.Device Tree
 * 4.GPIO
 * 5.User LEDs
 * 6.PWM
 * 7.AIN
 * 8.I2C
 * 9.SPI
 *************************************************************************************************/

#ifdef BEAGLEBONE

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/i2c-dev.h>
#include <linux/spi/spidev.h>
#include "beagleboneConfig.h"

// PWM Constants
bool pwmInitialized    = false;
char pathCapemgr [128] = "";
char pathOcp     [128] = "";
pwmNode_t *pwmList     = NULL;

// AIN Constants
bool ainInitialized   = false;
char pathHelper [128] = "";



/**************************************************************************************************
 * 1.Pins configuration table
 *************************************************************************************************/

pin_t pinTable[] = {
  {"USR0"      , "USR0" , 53 , -1, -1,  1 },
  {"USR1"      , "USR1" , 54 , -1, -1,  1 },
  {"USR2"      , "USR2" , 55 , -1, -1,  1 },
  {"USR3"      , "USR3" , 56 , -1, -1,  1 },

  {"DGND"      , "P8_1" , 0  , -1, -1, -1 },
  {"DGND"      , "P8_2" , 0  , -1, -1, -1 },
  {"GPIO1_6"   , "P8_3" , 38 , -1, -1,  1 },
  {"GPIO1_7"   , "P8_4" , 39 , -1, -1,  1 },
  {"GPIO1_2"   , "P8_5" , 34 , -1, -1,  1 },
  {"GPIO1_3"   , "P8_6" , 35 , -1, -1,  1 },
  {"TIMER4"    , "P8_7" , 66 , -1, -1,  0 },
  {"TIMER7"    , "P8_8" , 67 , -1, -1,  0 },
  {"TIMER5"    , "P8_9" , 69 , -1, -1,  0 },
  {"TIMER6"    , "P8_10", 68 , -1, -1,  0 },
  {"GPIO1_13"  , "P8_11", 45 , -1, -1,  0 },
  {"GPIO1_12"  , "P8_12", 44 , -1, -1,  0 },
  {"EHRPWM2B"  , "P8_13", 23 ,  4, -1,  0 },
  {"GPIO0_26"  , "P8_14", 26 , -1, -1,  0 },
  {"GPIO1_15"  , "P8_15", 47 , -1, -1,  0 },
  {"GPIO1_14"  , "P8_16", 46 , -1, -1,  0 },
  {"GPIO0_27"  , "P8_17", 27 , -1, -1,  0 },
  {"GPIO2_1"   , "P8_18", 65 , -1, -1,  0 },
  {"EHRPWM2A"  , "P8_19", 22 ,  4, -1,  0 },
  {"GPIO1_31"  , "P8_20", 63 , -1, -1,  1 },
  {"GPIO1_30"  , "P8_21", 62 , -1, -1,  1 },
  {"GPIO1_5"   , "P8_22", 37 , -1, -1,  1 },
  {"GPIO1_4"   , "P8_23", 36 , -1, -1,  1 },
  {"GPIO1_1"   , "P8_24", 33 , -1, -1,  1 },
  {"GPIO1_0"   , "P8_25", 32 , -1, -1,  1 },
  {"GPIO1_29"  , "P8_26", 61 , -1, -1,  0 },
  {"GPIO2_22"  , "P8_27", 86 , -1, -1,  1 },
  {"GPIO2_24"  , "P8_28", 88 , -1, -1,  1 },
  {"GPIO2_23"  , "P8_29", 87 , -1, -1,  1 },
  {"GPIO2_25"  , "P8_30", 89 , -1, -1,  1 },
  {"UART5_CTSN", "P8_31", 10 , -1, -1,  1 },
  {"UART5_RTSN", "P8_32", 11 , -1, -1,  1 },
  {"UART4_RTSN", "P8_33", 9  , -1, -1,  1 },
  {"UART3_RTSN", "P8_34", 81 ,  2, -1,  1 },
  {"UART4_CTSN", "P8_35", 8  , -1, -1,  1 },
  {"UART3_CTSN", "P8_36", 80 ,  2, -1,  1 },
  {"UART5_TXD" , "P8_37", 78 , -1, -1,  1 },
  {"UART5_RXD" , "P8_38", 79 , -1, -1,  1 },
  {"GPIO2_12"  , "P8_39", 76 , -1, -1,  1 },
  {"GPIO2_13"  , "P8_40", 77 , -1, -1,  1 },
  {"GPIO2_10"  , "P8_41", 74 , -1, -1,  1 },
  {"GPIO2_11"  , "P8_42", 75 , -1, -1,  1 },
  {"GPIO2_8"   , "P8_43", 72 , -1, -1,  1 },
  {"GPIO2_9"   , "P8_44", 73 , -1, -1,  1 },
  {"GPIO2_6"   , "P8_45", 70 ,  3, -1,  1 },
  {"GPIO2_7"   , "P8_46", 71 ,  3, -1,  1 },

  {"DGND"      , "P9_1" , 0  , -1, -1, -1 },
  {"DGND"      , "P9_2" , 0  , -1, -1, -1 },
  {"VDD_3V3"   , "P9_3" , 0  , -1, -1, -1 },
  {"VDD_3V3"   , "P9_4" , 0  , -1, -1, -1 },
  {"VDD_5V"    , "P9_5" , 0  , -1, -1, -1 },
  {"VDD_5V"    , "P9_6" , 0  , -1, -1, -1 },
  {"SYS_5V"    , "P9_7" , 0  , -1, -1, -1 },
  {"SYS_5V"    , "P9_8" , 0  , -1, -1, -1 },
  {"PWR_BUT"   , "P9_9" , 0  , -1, -1, -1 },
  {"SYS_RESETn", "P9_10", 0  , -1, -1, -1 },
  {"UART4_RXD" , "P9_11", 30 , -1, -1,  0 },
  {"GPIO1_28"  , "P9_12", 60 , -1, -1,  0 },
  {"UART4_TXD" , "P9_13", 31 , -1, -1,  0 },
  {"EHRPWM1A"  , "P9_14", 50 ,  6, -1,  0 },
  {"GPIO1_16"  , "P9_15", 48 , -1, -1,  0 },
  {"EHRPWM1B"  , "P9_16", 51 ,  6, -1,  0 },
  {"I2C1_SCL"  , "P9_17", 5  , -1, -1,  0 },
  {"I2C1_SDA"  , "P9_18", 4  , -1, -1,  0 },
  {"I2C2_SCL"  , "P9_19", 13 , -1, -1,  1 },
  {"I2C2_SDA"  , "P9_20", 12 , -1, -1,  1 },
  {"UART2_TXD" , "P9_21", 3  ,  3, -1,  0 },
  {"UART2_RXD" , "P9_22", 2  ,  3, -1,  0 },
  {"GPIO1_17"  , "P9_23", 49 , -1, -1,  0 },
  {"UART1_TXD" , "P9_24", 15 , -1, -1,  0 },
  {"GPIO3_21"  , "P9_25", 117, -1, -1,  1 },
  {"UART1_RXD" , "P9_26", 14 , -1, -1,  0 },
  {"GPIO3_19"  , "P9_27", 115, -1, -1,  0 },
  {"SPI1_CS0"  , "P9_28", 113,  4, -1,  1 },
  {"SPI1_D0"   , "P9_29", 111,  1, -1,  1 },
  {"SPI1_D1"   , "P9_30", 112, -1, -1,  0 },
  {"SPI1_SCLK" , "P9_31", 110,  1, -1,  1 },
  {"VDD_ADC"   , "P9_32", 0  , -1, -1, -1 },
  {"AIN4"      , "P9_33", 204, -1,  4, -1 },
  {"GNDA_ADC"  , "P9_34", 0  , -1, -1, -1 },
  {"AIN6"      , "P9_35", 206, -1,  6, -1 },
  {"AIN5"      , "P9_36", 205, -1,  5, -1 },
  {"AIN2"      , "P9_37", 202, -1,  2, -1 },
  {"AIN3"      , "P9_38", 203, -1,  3, -1 },
  {"AIN0"      , "P9_39", 200, -1,  0, -1 },
  {"AIN1"      , "P9_40", 201, -1,  1, -1 },
  {"CLKOUT2"   , "P9_41", 20 , -1, -1,  0 },
  {"GPIO0_7"   , "P9_42", 7  ,  0, -1,  0 },
  {"DGND"      , "P9_43", 0  , -1, -1, -1 },
  {"DGND"      , "P9_44", 0  , -1, -1, -1 },
  {"DGND"      , "P9_45", 0  , -1, -1, -1 },
  {"DGND"      , "P9_46", 0  , -1, -1, -1 }};
  
pwm_t pwm_table[] = {
  { "ehrpwm2", 6, 1, 4, "ehrpwm.2:1", "EHRPWM2B", "48304000", "48304200", "P8_13"},
  { "ehrpwm2", 5, 0, 4, "ehrpwm.2:0", "EHRPWM2A", "48304000", "48304200", "P8_19"},
  { "ehrpwm1", 4, 1, 2, "ehrpwm.1:1", "EHRPWM1B", "48302000", "48302200", "P8_34"},
  { "ehrpwm1", 3, 0, 2, "ehrpwm.1:0", "EHRPWM1A", "48302000", "48302200", "P8_36"},
  { "ehrpwm2", 5, 0, 3, "ehrpwm.2:0", "EHRPWM2A", "48304000", "48304200", "P8_45"},
  { "ehrpwm2", 6, 1, 3, "ehrpwm.2:1", "EHRPWM2B", "48304000", "48304200", "P8_46"},
  { "ehrpwm1", 3, 0, 6, "ehrpwm.1:0", "EHRPWM1A", "48302000", "48302200", "P9_14"},
  { "ehrpwm1", 4, 1, 6, "ehrpwm.1:1", "EHRPWM1B", "48302000", "48302200", "P9_16"},
  { "ehrpwm0", 1, 1, 3, "ehrpwm.0:1", "EHRPWM0B", "48300000", "48300200", "P9_21"},
  { "ehrpwm0", 0, 0, 3, "ehrpwm.0:0", "EHRPWM0A", "48300000", "48300200", "P9_22"},
  {   "ecap2", 7, 0, 4, "ecap.2",     "ECAPPWM2", "48304000", "48304100", "P9_28"},
  { "ehrpwm0", 1, 1, 1, "ehrpwm.0:1", "EHRPWM0B", "48300000", "48300200", "P9_29"},
  { "ehrpwm0", 0, 0, 1, "ehrpwm.0:0", "EHRPWM0A", "48300000", "48300200", "P9_31"},
  {   "ecap0", 2, 0, 0, "ecap.0",     "ECAPPWM0", "48300000", "48300100", "P9_42"},
  { NULL, 0, 0, 0, NULL, NULL, NULL, NULL, NULL }
};



/**************************************************************************************************
 * 2.General
 *************************************************************************************************/

/**
 * Function used for testing the correctness of install
 */
void beagleTest() {
  printf("Hello from beagleboneConfig.c! Magic number: 42\n");
}

/**
 * Builds in fullPath the full path of the file prefix* from directory dirPath
 */
result_t buildPath(const char *dirPath, const char *prefix, char *fullPath, int pathLen) {
  DIR *dir;
  struct dirent *entry;
  char* foundString;

  if((dir = opendir(dirPath)) != NULL) {
    while((entry = readdir(dir)) != NULL) {
      // Enforce that the prefix must be the first part of the file
      foundString = strstr(entry->d_name, prefix);

      if(foundString != NULL && (entry->d_name - foundString) == 0) {
        snprintf(fullPath, pathLen, "%s/%s", dirPath, entry->d_name);
        closedir(dir);
        return SUCCESS;
      }
    }
    
    debug("There is not a file named %s* in directory %s", prefix, dirPath);
    closedir(dir);
    return ERROR;
  } else {
    debug("Could not open directory %s", dirPath);
    return ERROR;
  }
}

/**
 * Returns the gpio pin number by position in pinTable or 0xFF if position is invalid
 */
byte getGpioByPos(const byte pos) {
  if (!(0 <= pos && pos < NO_PINS)) {
    debug("Invalid position %d. pos should be in [0, NO_PINS) interval.", pos);
    return -1;
  }

  pin_t *aux;

  aux = pinTable + pos;

  return aux->gpio;
}

/**
 * Returns the gpio pin number by name or 0xFF if doesn't exists
 */
byte getGpioByName(const char *name) {
  int i;
  pin_t *aux;

  aux = pinTable;
  for(i = 0; i < NO_PINS; i++) {
    if(strcmp(aux->name, name) == 0) {
      return aux->gpio;
    }
    aux++;
  }

  debug("There is no pin named %s", name);
  return -1;
}

/**
 * Returns the gpio pin number by key or 0xFF if doesn't exists
 */
byte getGpioByKey(const char *key) {
  int i;
  pin_t *aux;

  aux = pinTable;
  for(i = 0; i < NO_PINS; i++) {
    if(strcmp(aux->key, key) == 0) {
      return aux->gpio;
    }
    aux++;
  }

  debug("There is no pin with the key %s", key);
  return -1;
}

/**
 * Returns the name of pin by gpio or NULL if doesn't exists
 */
const char* getNameByGpio(const byte gpio) {
  int i;
  pin_t *aux;

  aux = pinTable;
  for(i = 0; i < NO_PINS; i++) {
    if(aux->gpio == gpio) {
      return aux->name;
    }
    aux++;
  }

  debug("There is no pin with gpio %d", gpio);
  return NULL;
}

/**
 * Returns the key of pin by gpio or NULL if doesn't exists
 */
const char* getKeyByGpio(const byte gpio) {
  int i;
  pin_t *aux;

  aux = pinTable;
  for(i = 0; i < NO_PINS; i++) {
    if(aux->gpio == gpio) {
      return aux->key;
    }
    aux++;
  }

  debug("There is no pin with gpio %d", gpio);
  return NULL;
}



/**************************************************************************************************
 * 3.Device Tree
 *************************************************************************************************/

/**
 * Loads a device tree
 */
result_t loadDeviceTree(const char *name) {
  FILE *pFile;
  char pathSlots [128];
  char line      [256];

  if(strcmp(pathCapemgr, "") == 0) {
    if(buildPath("/sys/devices/platform", "bone_capemgr", pathCapemgr, sizeof(pathCapemgr)) == ERROR) {
      debug("Could not build path to bone_capemgr");
      return ERROR;
    }
  }

  snprintf(pathSlots, sizeof(pathSlots), "%s/slots", pathCapemgr);

  if((pFile = fopen(pathSlots, "r+")) == NULL) {
    debug("Could not open file %s", pathSlots);
    return ERROR;
  }

  while(fgets(line, sizeof(line), pFile)) {
    // Device Tree already loaded
    if(strstr(line, name)) {
      fclose(pFile);
      return SUCCESS;
    }
  }

  fprintf(pFile, "%s", name);
  fclose(pFile);
  return SUCCESS;
}

/**
 * Unloads a device tree
 */
result_t unloadDeviceTree(const char *name) {
  FILE *pFile;
  char pathSlots    [128];
  char slotsDirPath [128];
  char line         [256];
  char *p;

  if(strcmp(pathCapemgr, "") == 0) {
    if(buildPath("/sys/devices", "bone_capemgr", pathCapemgr, sizeof(pathCapemgr)) == ERROR) {
      debug("Could not build path to bone_capemgr");
      return ERROR;
    }
  }

  snprintf(pathSlots, sizeof(pathSlots), "%s/slots", pathCapemgr);

  if((pFile = fopen(pathSlots, "r+")) == NULL) {
    debug("Could not open file %s", pathSlots);
    return ERROR;
  }

  while (fgets(line, sizeof(line), pFile)) {
    // Device is loaded, let's unload it
    if (strstr(line, name)) {
      // Stop at first colon
      p = strstr(line, ":");
      strncpy(p, "\0", 1);

      // Trim leading whitespace
      p = line;
      while(isspace(*p)) {
        p++;
      }

      fprintf(pFile, "-%s", p);

      fclose(pFile);
      return SUCCESS;
    }
  }

  // Device not loaded
  fclose(pFile);
  return SUCCESS;
}

/**************************************************************************************************
 * 4.GPIO
 *************************************************************************************************/

/**
 * Returns false if the pin gpio is not valid or true otherwise
 */
bool gpioIsValid(const byte gpio) {
  int i;
  pin_t *aux;

  aux = pinTable;
  for(i = 0; i < NO_PINS; i++) {
    if(aux->gpio == gpio) {
      return true;
    }
    aux++;
  }

  return false;
}

/**
 * Checks if GPIO pin is exported
 */
bool gpioIsExported(const byte gpio) {
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

  return open(buf, O_WRONLY) > 0;
}

/**
 * Exports GPIO pin
 */
result_t gpioExport(const byte gpio) {
  int fd;
  char buf[MAX_BUF];

  if((fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY)) < 0) {
    debug("Could not open file" SYSFS_GPIO_DIR "/export");
    return ERROR;
  }

  snprintf(buf, sizeof(buf), "%d", gpio);
  write(fd, buf, sizeof(buf));

  close(fd);
  return SUCCESS;
}

/**
 * Unexports GPIO pin
 */
result_t gpioUnexport(const byte gpio) {
  int fd;
  char buf[MAX_BUF];

  if((fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY)) < 0) {
    debug("Could not open file " SYSFS_GPIO_DIR "/unexport");
    return ERROR;
  }

  snprintf(buf, sizeof(buf), "%d", gpio);
  write(fd, buf, sizeof(buf));

  close(fd);
  return SUCCESS;
}

/**
 * Sets direction of GPIO pin
 */
result_t gpioSetDir(const byte gpio, const byte dir) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/direction", gpio);

  if((fd = open(buf, O_WRONLY)) < 0) {
    debug("Could not open file " SYSFS_GPIO_DIR "/gpio%d/direction", gpio);
    return ERROR;
  }

  if(dir == INPUT) {
    write(fd, "in", 3);
  } else if(dir == OUTPUT) {
    write(fd, "out", 4);
  } else {
    debug("Direction can be either INPUT or OUTPUT");
    close(fd);
    return ERROR;
  }

  close(fd);
  return SUCCESS;
}

/**
 * Returns direction of GPIO pin INPUT, OUTPUT or 0xFF in case of failure
 */
byte gpioGetDir(const byte gpio) {
  int fd;
  char buf[MAX_BUF];
  char ch;

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/direction", gpio);

  if((fd = open(buf, O_RDONLY)) < 0) {
    debug("Could not open file " SYSFS_GPIO_DIR "/gpio%d/direction", gpio);
    return -1;
  }

  memset(buf, 0, sizeof(buf));
  read(fd, buf, sizeof(buf));
  close(fd);

  if(strncmp(buf, "in", 2) == 0) {
    return INPUT;
  } else if(strncmp(buf, "out", 3) == 0) {
    return OUTPUT;
  } else {
    debug("Unknown direction %s", buf);
    return -1;
  }
}

/**
 * Sets value of GPIO pin
 */
result_t gpioSetValue(const byte gpio, const byte value) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

  if((fd = open(buf, O_WRONLY)) < 0) {
    debug("Could not open file " SYSFS_GPIO_DIR "/gpio%d/value", gpio);
    return ERROR;
  }

  if(value == LOW) {
    write(fd, "0", 2);
  } else if(value == HIGH) {
    write(fd, "1", 2);
  } else {
    debug("Value can be either LOW or HIGH");
    close(fd);
    return ERROR;
  }

  close(fd);
  return SUCCESS;
}

/**
 * Returns value of GPIO pin LOW, HIGH or 0xFF in case of failure
 */
byte gpioGetValue(const byte gpio) {
  int fd;
  char buf[MAX_BUF];
  char ch;

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

  if((fd = open(buf, O_RDONLY)) < 0) {
    debug("Could not open file " SYSFS_GPIO_DIR "/gpio%d/value", gpio);
    return -1;
  }

  read(fd, &ch, 1);
  close(fd);

  if(ch == '0') {
    return LOW;
  } else if(ch == '1') {
    return HIGH;
  } else {
    debug("Unknown value %c", ch);
    return -1;
  }
}

/**
 * Set active_low of GPIO pin
 */
result_t gpioSetActiveLow(const byte gpio, const byte value) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/active_low", gpio);

  if((fd = open(buf, O_WRONLY)) < 0) {
    debug("Could not open file " SYSFS_GPIO_DIR "/gpio%d/active_low", gpio);
    return ERROR;
  }

  if(value == 0) {
    write(fd, "0", 2);
  } else if(value == 1) {
    write(fd, "1", 2);
  } else {
    debug("Value can be either 0 or 1");
    close(fd);
    return ERROR;
  }

  close(fd);
  return SUCCESS;
}

/**
 * Returns active_low value of GPIO pin or 0xFF in case of failure
 */
byte gpioGetActiveLow(const byte gpio) {
  int fd;
  char buf[MAX_BUF];
  char ch;

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/active_low", gpio);

  if((fd = open(buf, O_RDONLY)) < 0) {
    debug("Could not open file " SYSFS_GPIO_DIR "/gpio%d/active_low", gpio);
    return -1;
  }

  read(fd, &ch, 1);
  close(fd);

  if(ch == '0') {
    return 0;
  } else if(ch == '1') {
    return 1;
  } else {
    debug("Unknown value %c", ch);
    return -1;
  }
}

/**
 * Sets edge of GPIO pin
 */
result_t gpioSetEdge(const byte gpio, const byte edge) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

  if ((fd = open(buf, O_WRONLY)) < 0) {
    debug("Could not open file " SYSFS_GPIO_DIR "/gpio%d/edge", gpio);
    return ERROR;
  }

  switch(edge) {
    case NONE:
      write(fd, "none", 5);
      break;
    case RISING:
      write(fd, "rising", 7);
      break;
    case FALLING:
      write(fd, "falling", 8);
      break;
    case BOTH:
      write(fd, "both", 5);
      break;
    default:
      debug("Edge can be either NONE, RISING, FALLING or BOTH");
      close(fd);
      return ERROR;
  }

  close(fd);
  return SUCCESS;
}

/**
 * Returns edge of GPIO pin or 0xFF in case of failure
 */
byte gpioGetEdge(const byte gpio) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

  if ((fd = open(buf, O_RDONLY)) < 0) {
    debug("Could not open file " SYSFS_GPIO_DIR "/gpio%d/edge", gpio);
    return -1;
  }

  memset(buf, 0, sizeof(buf));
  read(fd, buf, sizeof(buf));
  close(fd);

  if(strncmp(buf, "none", 4) == 0) {
    return NONE;
  } else if(strncmp(buf, "rising", 6) == 0) {
    return RISING;
  } else if(strncmp(buf, "falling", 7) == 0) {
    return FALLING;
  } else if(strncmp(buf, "both", 4) == 0) {
    return BOTH;
  } else {
    debug("Unknown edge %s", buf);
    return -1;
  }
}



/**************************************************************************************************
 * 5.User LEDs
 *************************************************************************************************/

/**
 * Check if GPIO pin is a valid used led
 */
bool ledIsValid(const byte gpio) {
  return 53 <= gpio && gpio <= 56;
}

/**
 * Sets user led trigger
 */
result_t ledSetTrigger(const byte gpio, const byte trigger) {
  if(!ledIsValid(gpio)) {
    debug("Invalid gpio %d. Value of gpio should be in [53, 56] interval", gpio);
    return ERROR;
  }

  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_LEDS_DIR "/beaglebone:green:usr%d/trigger", gpio - 53);

  if((fd = open(buf, O_WRONLY)) < 0) {
    debug("Could not open file " SYSFS_LEDS_DIR "/beaglebone:green:usr%d/trigger", gpio - 53);
    return ERROR;
  }

  switch(trigger) {
    case NONE:
      write(fd, "none", 5);
      break;
    case NAND_DISK:
      write(fd, "nand-disk", 10);
      break;
    case MMC0:
      write(fd, "mmc0", 5);
      break;
    case MMC1:
      write(fd, "mmc1", 5);
      break;
    case TIMER:
      write(fd, "timer", 6);
      break;
    case ONESHOT:
      write(fd, "oneshot", 8);
      break;
    case HEARTBEAT:
      write(fd, "heartbeat", 9);
      break;
    case BACKLIGHT:
      write(fd, "backlight", 10);
      break;
    case GPIO:
      write(fd, "gpio", 5);
      break;
    case CPU0:
      write(fd, "cpu0", 5);
      break;
    case DEFAULT_ON:
      write(fd, "default-on", 11);
      break;
    case TRANSIENT:
      write(fd, "transient", 10);
      break;
    default:
      debug("Invalid trigger %d", trigger);
      close(fd);
      return ERROR;
  }

  close(fd);
  return SUCCESS;
}

/**
 * Sets led value LOW or HIGH
 */
result_t ledSetValue(const byte gpio, const byte value) {
  if(!ledIsValid(gpio)) {
    debug("Invalid gpio %d. Value of gpio should be in [53, 56] interval", gpio);
    return ERROR;
  }

  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_LEDS_DIR "/beaglebone:green:usr%d/brightness", gpio - 53);

  if((fd = open(buf, O_WRONLY)) < 0) {
    debug("Could not open file " SYSFS_LEDS_DIR "/beaglebone:green:usr%d/brightness", gpio - 53);
    return ERROR;
  }

  if(value == LOW) {
    write(fd, "0", 2);
  } else if(value == HIGH) {
    write(fd, "1", 2);
  } else {
    debug("Value can be either INPUT or OUTPUT. Invalid value %d", value);
    close(fd);
    return ERROR;
  }

  close(fd);
  return SUCCESS;
}

/**
 * Returns value of gpio-led LOW or HIGH or 0xFF in case of failure
 */
byte ledGetValue(const byte gpio) {
  if(!ledIsValid(gpio)) {
    debug("Invalid gpio %d. Value of gpio should be in [53, 56] interval", gpio);
    return ERROR;
  }

  int fd;
  char buf[MAX_BUF];
  char ch;

  snprintf(buf, sizeof(buf), SYSFS_LEDS_DIR "/beaglebone:green:usr%d/brightness", gpio - 53);

  if((fd = open(buf, O_RDONLY)) < 0) {
    debug("Could not open file " SYSFS_LEDS_DIR "/beaglebone:green:usr%d/brightness", gpio - 53);
    return -1;
  }

  read(fd, &ch, 1);
  close(fd);

  if(ch == '0') {
    return LOW;
  } else if(ch == '1') {
    return HIGH;
  } else {
    debug("Unknown value %c", ch);
    return -1;
  }
}

/**
 * Resets an USER LED
 */
result_t ledReset(const byte gpio) {
  if(!ledIsValid(gpio)) {
    debug("Invalid gpio %d. Value of gpio should be in [53, 56] interval", gpio);
    return ERROR;
  }

  switch(gpio - 53) {
    case 0:
      ledSetTrigger(gpio, HEARTBEAT);
      break;
    case 1:
      ledSetTrigger(gpio, MMC0);
      break;
    case 2:
      ledSetTrigger(gpio, CPU0);
      break;
    case 3:
      ledSetTrigger(gpio, MMC1);
      break;
    default:
      debug("This should not have happened");
      return ERROR;
  }

  return SUCCESS;
}



/**************************************************************************************************
 * 6.PWM
 *************************************************************************************************/

/**
 * Checks if pin GPIO is a valid PWM
 */
bool pwmIsValid(const byte gpio) {
  switch(gpio) {
    case 23:  // P8_13
    case 22:  // P8_19
    case 81:  // P8_34
    case 80:  // P8_36
    case 70:  // P8_45
    case 71:  // P8_46
    case 50:  // P9_14
    case 51:  // P9_16
    case 3:   // P9_21
    case 2:   // P9_22
    case 113: // P9_28
    case 111: // P9_21
    case 110: // P9_31
    case 7:   // P9_42
      return true;
    default:
      return false;
  }
}

/**
 * Checks if pwm pin is enabled
 */
bool pwmIsEnabled(const byte gpio) {
  return pwmGetPin(getKeyByGpio(gpio)) != NULL;
}

/**
 * PWM Initialization
 */
void pwmInit() {
  if(!pwmInitialized) {
    /*loadDeviceTree("am33xx_pwm");*/
    buildPath("/sys/devices/platform", "ocp", pathOcp, sizeof(pathOcp));
    pwmInitialized = true;
    /* do nothing for new beaglebones, only build pathOcp*/
  }
}

/**
 * Returns the node key from pwmList or NULL in case it doesn't exist
 */
pwmNode_t* pwmGetPin(const char *key) {
  pwmNode_t *aux = pwmList;

  while(aux != NULL) {
    if(strcmp(aux->key, key) == 0) {
      return aux;
    }
    aux = aux->next;
  }

  return NULL;
}

/**
 * Gets the row with the directories from the PWM table
 */
result_t get_pwm_by_key(const char *key, pwm_t **pwm)
{
    pwm_t *p;
    // Loop through the table
    for (p = pwm_table; p->key != NULL; ++p) {
        if (strcmp(p->key, key) == 0) {
            // Return the pwm_t struct
            *pwm = p;
            return SUCCESS;
        }
    }
    return ERROR;
}

/**
 * Enables PWM for pin key
 */
result_t pwmEnable(const char *key) {
  if(pwmGetPin(key) != NULL) {
    // PWM pin already enabled
    return SUCCESS;
  }

  //char slotsFragment[16];
  //char pwmTestFragment[16];
  char pathPwmTest[100];
  pwmNode_t *aux;
  pwmNode_t *newNode;

  if(!pwmInitialized) {
    pwmInit();
  }
  
  FILE *f = NULL;
  char pwm_dev_path[45]; // "/sys/devices/platform/ocp/48300000.epwmss"
  char pwm_addr_path[60]; // "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm"
  char pwm_addr_path2[90]; // "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm/pwm"
  char pwm_chip_path[90]; // "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm/pwm/pwmchip0"
  char pwm_export_path[90]; // "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm/pwm/pwmchip0/export"
  char pwm_path[90]; // "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm/pwm/pwmchip0/pwm1"
  char duty_path[90]; // "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm/pwm/pwmchip0/pwm1/duty_cycle"
  char period_path[90];
  char polarity_path[90];
  char enable_path[90];
  pwm_t *p;
  get_pwm_by_key(key,&p);
  
  if(buildPath(pathOcp, p->chip, pwm_dev_path, sizeof(pwm_dev_path)) == ERROR) {
    debug("Could not build path to %s", p->chip);
    return ERROR;
  }
  if(buildPath(pwm_dev_path, p->addr, pwm_addr_path, sizeof(pwm_addr_path)) == ERROR) {
    debug("Could not build path to %s", p->addr);
    return ERROR;
  }
  if(buildPath(pwm_addr_path, "pwm", pwm_addr_path2, sizeof(pwm_addr_path2)) == ERROR) {
    debug("Could not build path to %s", "pwm");
    return ERROR;
  }
  if(buildPath(pwm_addr_path2, "pwmchip", pwm_chip_path, sizeof(pwm_chip_path)) == ERROR) {
    debug("Could not build path to %s", "pwmchip");
    return ERROR;
  }
  snprintf(pwm_path, sizeof(pwm_path), "%s/pwm%d", pwm_chip_path, p->index);
  
  /* Exporting pwm0 or pwm1 */
  snprintf(pwm_export_path, sizeof(pwm_export_path), "%s/export", pwm_chip_path);
  f = fopen(pwm_export_path, "w");
  if (f == NULL) { // Can't open the export file
    debug("Could not build path to %s", "pwm/pwmchip");
    return ERROR;
  }
  fprintf(f, "%d", p->index);
  fclose(f);
  

  newNode = (pwmNode_t*) malloc(sizeof(pwmNode_t));
  newNode->key = key;
  newNode->pathPwmTest = strdup(pwm_path);
  newNode->next = NULL;

  // Empty pwmList
  if(pwmList == NULL) {
    pwmList = newNode;
    return SUCCESS;
  }

  aux = pwmList;
  while(aux->next != NULL) {
    aux = aux->next;
  }
  aux->next = newNode;

  return SUCCESS;
}

/**
 * Disables PWM pin
 */
result_t pwmDisable(const char *key) {
  /*char fragment[16];

  sprintf(fragment, "bone_pwm_%s", key);
  return unloadDeviceTree(fragment);*/
}

/**
 * Sets PWM Period in nanoseconds
 */
result_t pwmSetPeriod(const char *key, ulong period) {
  pwmNode_t *aux;
  char buf[16];
  char pathPeriod[90];
  int fdPeriod;

  if((aux = pwmGetPin(key)) == NULL) {
    debug("PWM %s not enabled yet", key);
    return ERROR;
  }

  snprintf(pathPeriod, sizeof(pathPeriod), "%s/period", aux->pathPwmTest);
  if((fdPeriod = open(pathPeriod, O_WRONLY)) < 0) {
    debug("Could not open file %s", pathPeriod);
    return ERROR;
  }

  snprintf(buf, sizeof(buf), "%lu", period);
  write(fdPeriod, buf, sizeof(buf));

  close(fdPeriod);
  return SUCCESS;
}

/**
 * Returns PWM period or 0xFF in case of failure
 */
ulong pwmGetPeriod(const char *key) {
  pwmNode_t *aux;
  char buf[16];
  char pathPeriod[90];
  int fdPeriod;

  if((aux = pwmGetPin(key)) == NULL) {
    debug("PWM %s not enabled yet", key);
    return -1;
  }

  snprintf(pathPeriod, sizeof(pathPeriod), "%s/period", aux->pathPwmTest);
  if((fdPeriod = open(pathPeriod, O_RDONLY)) < 0) {
    debug("Could not open file %s", pathPeriod);
    return -1;
  }

  read(fdPeriod, buf, sizeof(buf));

  close(fdPeriod);
  return atol(buf);
}

/**
 * Sets PWM pin duty in nanoseconds
 */
result_t pwmSetDuty(const char *key, const ulong duty) {
  pwmNode_t *aux;
  char buf[16];
  char pathDuty[90];
  int fdDuty;

  if((aux = pwmGetPin(key)) == NULL) {
    debug("PWM %s not enabled yet", key);
    return ERROR;
  }

  snprintf(pathDuty, sizeof(pathDuty), "%s/duty_cycle", aux->pathPwmTest);
  if((fdDuty = open(pathDuty, O_WRONLY)) < 0) {
    debug("Could not open file %s", pathDuty);
    return ERROR;
  }

  snprintf(buf, sizeof(buf), "%lu", duty);
  write(fdDuty, buf, sizeof(buf));

  close(fdDuty);
  return SUCCESS;
}

/**
 * Returns PWM duty or 0xFFFFFFFF in case of failure
 */
ulong pwmGetDuty(const char *key) {
  pwmNode_t *aux;
  char buf[20];
  char pathDuty[90];
  int fdDuty;

  if((aux = pwmGetPin(key)) == NULL) {
    debug("PWM %s not enabled yet", key);
    return -1;
  }

  snprintf(pathDuty, sizeof(pathDuty), "%s/duty_cycle", aux->pathPwmTest);
  if((fdDuty = open(pathDuty, O_RDONLY)) < 0) {
    debug("Could not open file %s", pathDuty);
    return -1;
  }

  read(fdDuty, buf, sizeof(buf));

  close(fdDuty);
  return atol(buf);
}

/**
 * Sets PWM Polarity
 */
result_t pwmSetPolarity(const char *key, const byte polarity) {
  pwmNode_t *aux;
  char buf[16];
  char pathPolarity[90];
  int fdPolarity;
  int len;

  if((aux = pwmGetPin(key)) == NULL) {
    debug("PWM %s not enabled yet", key);
    return ERROR;
  }

  snprintf(pathPolarity, sizeof(pathPolarity), "%s/polarity", aux->pathPwmTest);
  if((fdPolarity = open(pathPolarity, O_WRONLY)) < 0) {
    debug("Could not open file %s", pathPolarity);
    return ERROR;
  }

  snprintf(buf, sizeof(buf), "%d", polarity);
  write(fdPolarity, buf, sizeof(buf));

  close(fdPolarity);
  return SUCCESS;
}

/**
 * Returns PWM polarity or oxFF in case of failure
 */
byte pwmGetPolarity(const char *key) {
  pwmNode_t *aux;
  char buf[20];
  char pathPolarity[90];
  int fdPolarity;

  if((aux = pwmGetPin(key)) == NULL) {
    debug("PWM %s not enabled yet", key);
    return -1;
  }

  snprintf(pathPolarity, sizeof(pathPolarity), "%s/polarity", aux->pathPwmTest);
  if((fdPolarity = open(pathPolarity, O_RDONLY)) < 0) {
    debug("Could not open file %s", pathPolarity);
    return -1;
  }

  read(fdPolarity, buf, sizeof(buf));

  close(fdPolarity);
  return atoi(buf);
}

/**
 * Sets PWM run
 */
result_t pwmSetRun(const char* key, const byte run) {
  pwmNode_t *aux;
  char buf[16];
  char pathRun[90];
  int fdRun;
  int len;

  if((aux = pwmGetPin(key)) == NULL) {
    debug("PWM %s not enabled yet", key);
    return ERROR;
  }

  snprintf(pathRun, sizeof(pathRun), "%s/run", aux->pathPwmTest);
  if((fdRun = open(pathRun, O_RDWR)) < 0) {
    debug("Could not open file %s", pathRun);
    return ERROR;
  }

  len = snprintf(buf, sizeof(buf), "%d", run);
  write(fdRun, buf, len);

  close(fdRun);
  return SUCCESS;
}

/**
 * Returns PWM run or 0xFF in case of failure
 */
byte pwmGetRun(const char* key) {
  pwmNode_t *aux;
  char buf[20];
  char pathRun[90];
  int fdRun;

  if((aux = pwmGetPin(key)) == NULL) {
    debug("PWM %s not enabled yet", key);
    return -1;
  }

  snprintf(pathRun, sizeof(pathRun), "%s/run", aux->pathPwmTest);
  if((fdRun = open(pathRun, O_RDONLY)) < 0) {
    debug("Could not open file %s", pathRun);
    return -1;
  }

  read(fdRun, buf, sizeof(buf));

  close(fdRun);
  return atoi(buf);
}



/**************************************************************************************************
 * 7.AIN
 *************************************************************************************************/

/**
 * AIN Initialization
 */
void ainInit() {
  if(!ainInitialized) {
    loadDeviceTree("BB-ADC");
    buildPath("/sys/bus/iio/devices", "iio", pathHelper, sizeof(pathHelper));
    ainInitialized = true;
  }
}

/**
 * Checks if gpio value represents an AIN pin
 */
bool ainIsValid(const byte gpio) {
  return 200 <= gpio && gpio <= 206;
}

/**
 * Returns value of AIN pin
 */
int ainGetValue(const byte gpio) {
  int  fdAin;
  char buf[16];
  char pathAin[128];

  if(!ainInitialized) {
    ainInit();
  }

  snprintf(pathAin, sizeof(pathAin), "%s/in_voltage%d_raw", pathHelper, gpio - 200);

  if((fdAin = open(pathAin, O_RDONLY)) < 0) {
    debug("Could not open file %s", pathAin);
    return ERROR;
  }

  read(fdAin, buf, sizeof(buf));
  close(fdAin);
  return atoi(buf);
}



/**************************************************************************************************
 * 8.I2C
 *************************************************************************************************/

/**
 * Enables I2C-2 Bus
 */
result_t i2cEnableI2C2() {
  return loadDeviceTree("BB-I2C1");
}

/**
 * Opens one of the 3 I2C buses and returns the associated file descriptor
 */
int i2cOpenBus(byte bus) {
  if(!(0 <= bus && bus <= 2)) {
    debug("Wrong bus value: %d. <bus> value must be in [0, 2] interval", bus);
    return -1;
  }

  int fd;
  char filename[16];

  sprintf(filename, "/dev/i2c-%d", bus);

  if((fd = open(filename, O_RDWR)) < 0) {
    debug("Could not open file %s", filename);
    return -1;
  }

  return fd;
}

/**
 * Initialize Communication
 */
result_t i2cInitComm(int fd, int address) {
  if(ioctl(fd, I2C_SLAVE, address) < 0) {
    debug("Error initializing communication with device");
    return ERROR;
  }

  return SUCCESS;
}

/**
 * Reads from I2C bus
 */
result_t i2cRead(int fd, char *data, int length) {
  if(read(fd, data, length) != length) {
    debug("Could not read from I2C bus");
    return ERROR;
  }

  return SUCCESS;
}

/**
 * Write to I2C bus
 */
result_t i2cWrite(int fd, char *data, int length) {
  if(write(fd, data, length) != length) {
    debug("Could not write to I2C bus");
    return ERROR;
  }

  return SUCCESS;
}



/**************************************************************************************************
 * 9.SPI
 *************************************************************************************************/

/**
 * Enable SPIDEV0
 */
result_t spiEnableSPIDEV0() {
  return loadDeviceTree("BB-SPIDEV0");
}

/**
 * Enable SPIDEV1
 * Warning: YOU CANNNOT USE SPI1 WITHOUT DISABLING THE HDMI INTERFACE
 */
result_t spiEnableSPIDEV1() {
  return loadDeviceTree("BB-SPIDEV1");
}

/**
 * Opens SPI bus and returns the associated file descriptor
 */
int spiOpenBus(byte bus, byte chipselect) {
  int fd;
  char filename[16];

  sprintf(filename, "/dev/spidev%d.%d", bus, chipselect);

  if((fd = open(filename, O_RDWR)) < 0) {
    debug("Could not open file %s", filename);
    return -1;
  }

  return fd;
}

/**
 * Sets SPI mode
 */
result_t spiSetMode(int fd, byte mode) {
  if(ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
    debug("Could not set mode %d on bus with file descriptor %d", mode, fd);
    return ERROR;
  }

  return SUCCESS;
}

/**
 * Returns SPI mode
 */
byte spiGetMode(int fd) {
  byte mode;

  if(ioctl(fd, SPI_IOC_RD_MODE, &mode) < 0) {
    debug("Could not get mode on bus with file descriptor %d", fd);
    return -1;
  }

  return mode;
}

/**
 * Sets SPI bits per word
 */
result_t spiSetBitsPerWord(int fd, byte bits) {
  if(ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
    debug("Could not set bits per word %d on bus with file descriptor %d", bits, fd);
    return ERROR;
  }

  return SUCCESS;
}

/**
 * Sets SPI bits per word
 */
byte spiGetBitsPerWord(int fd) {
  byte bits;

  if(ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) {
    debug("Could not get bits per word on bus with file descriptor %d", fd);
    return -1;
  }

  return bits;
}

/**
 * Sets SPI Max Speed
 */
result_t spiSetMaxSpeed(int fd, uint speed) {
  if(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
    debug("Could not set max speed %d on bus with file descriptor %d", speed, fd);
    return ERROR;
  }

  return SUCCESS;
}

/**
 * Gets SPI max speed
 */
uint spiGetMaxSpeed(int fd) {
  uint speed;

  if(ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0) {
    debug("Could not get max speed on bus with file descriptor %d", fd);
    return -1;
  }

  return speed;
}



#ifdef __cplusplus
}
#endif

#endif // BEAGLEBONE
