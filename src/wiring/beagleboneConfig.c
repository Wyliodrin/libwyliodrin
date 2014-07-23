/**************************************************************************************************
 * Author: Razvan Madalin MATEI <matei.rm94@gmail.com>
 * Date last modified: July 2014
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
#include "beagleboneConfig.h"

// PWM Constants
byte pwmInitialized    = 0;
char pathCapemgr [128] = "";
char pathOcp     [128] = "";
pwmNode_t *pwmList     = NULL;



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
  {"AIN4"      , "P9_33", 0  , -1,  4, -1 },
  {"GNDA_ADC"  , "P9_34", 0  , -1, -1, -1 },
  {"AIN6"      , "P9_35", 0  , -1,  6, -1 },
  {"AIN5"      , "P9_36", 0  , -1,  5, -1 },
  {"AIN2"      , "P9_37", 0  , -1,  2, -1 },
  {"AIN3"      , "P9_38", 0  , -1,  3, -1 },
  {"AIN0"      , "P9_39", 0  , -1,  0, -1 },
  {"AIN1"      , "P9_40", 0  , -1,  1, -1 },
  {"CLKOUT2"   , "P9_41", 20 , -1, -1,  0 },
  {"GPIO0_7"   , "P9_42", 7  ,  0, -1,  0 },
  {"DGND"      , "P9_43", 0  , -1, -1, -1 },
  {"DGND"      , "P9_44", 0  , -1, -1, -1 },
  {"DGND"      , "P9_45", 0  , -1, -1, -1 },
  {"DGND"      , "P9_46", 0  , -1, -1, -1 }};



/**************************************************************************************************
 * 2.General
 *************************************************************************************************/

/**
 * Builds in fullPath the full path of the file prefix* from directory dirPath
 */
result_t buildPath(const char *dirPath, const char *prefix, char *fullPath, int fullPathLen) {
  DIR *dir;
  struct dirent *entry;
  char* foundString;

  if((dir = opendir(dirPath)) != NULL) {
    while((entry = readdir(dir)) != NULL) {
      // Enforce that the prefix must be the first part of the file
      foundString = strstr(entry->d_name, prefix);

      if(foundString != NULL && (entry->d_name - foundString) == 0) {
        snprintf(fullPath, fullPathLen, "%s/%s", dirPath, entry->d_name);
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
    if(buildPath("/sys/devices", "bone_capemgr", pathCapemgr, sizeof(pathCapemgr)) == ERROR) {
      debug("Could not build path to %s", pathCapemgr);
      return ERROR;
    }
  }
  snprintf(pathSlots, sizeof(pathSlots), "%s/slots", pathCapemgr);

  if((pFile = fopen(pathSlots, "r+")) == NULL) {
    debug("Could not open file %s", pathSlots);
    return ERROR;
  }

  while (fgets(line, sizeof(line), pFile)) {
    // Device Tree already loaded
    if (strstr(line, name)) {
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
  byte slotsLine;

  if(strcmp(pathCapemgr, "") == 0) {
    if(buildPath("/sys/devices", "bone_capemgr", pathCapemgr, sizeof(pathCapemgr)) == ERROR) {
      debug("Could not build path to %s", pathCapemgr);
    }
  }
  snprintf(pathSlots, sizeof(pathSlots), "%s/slots", pathCapemgr);

  if((pFile = fopen(pathSlots, "r+")) == NULL) {
    debug("Could not open file %s", pathSlots);
    return ERROR;
  }

  slotsLine = 0;
  while (fgets(line, sizeof(line), pFile)) {
    // Device is loaded, let's unload it
    if (strstr(line, name)) {
      fprintf(pFile, "-%d", slotsLine);
      fclose(pFile);
      return SUCCESS;
    }

    slotsLine++;
  }

  // Device not loaded
  fclose(pFile);
  return SUCCESS;
}

/**************************************************************************************************
 * 4.GPIO
 *************************************************************************************************/

/**
 * Function used for testing the correctness of the install
 */
void beagleTest() {
  printf("Hello from beagleboneConfig.c! Magic number: 11\n");
}

/**
 * Returns the gpio pin number by name or -1 in case of failure
 */
byte getGpioByName(const char *name) {
  int i;
  pin_t *aux;

  aux = pinTable;
  for(i = 0; i < sizeof(pinTable)/sizeof(pinTable[0]); i++) {
    if(strcmp(aux->name, name) == 0) {
      return aux->gpio;
    }

    aux++;
  }

  debug("There is no pin named %s", name);
  return 0;
}

/**
 * Returns the gpio pin number by key or -1 in case of failure
 */
byte getGpioByKey(const char *key) {
  int i;
  pin_t *aux;

  aux = pinTable;
  for(i = 0; i < sizeof(pinTable)/sizeof(pinTable[0]); i++) {
    if(strcmp(aux->key, key) == 0) {
      return aux->gpio;
    }

    aux++;
  }

  debug("There is no pin with the key %s", key);
  return 0;
}

/**
 * Returns 0 if the pin gpio is not valid or 1 otherwise
 */
byte gpioIsValid(byte gpio) {
  int i;
  pin_t *aux;

  aux = pinTable;
  for(i = 0; i < sizeof(pinTable)/sizeof(pinTable[0]); i++) {
    if(aux->gpio == gpio) {
      return 1;
    }

    aux++;
  }

  return 0;
}

/**
 * Returns 0 if the pin gpio is not exported or 1 otherwise
 */
byte gpioIsExported(byte gpio) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

  fd = open(buf, O_WRONLY);

  if(fd < 0) { // Not exported
    return 0;
  } else {     // Exported
    return 1;
  }
}

/**
 * Exports pin
 */
void gpioExport(byte gpio) {
  int fd, len;
  char buf[MAX_BUF];

  fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
  if(fd < 0) {
    perror("gpioExport");
    return;
  }

  len = snprintf(buf, sizeof(buf), "%d", gpio);
  write(fd, buf, len);
  close(fd);
}

/**
 * Unexports pin
 */
void gpioUnexport(byte gpio) {
  int fd, len;
  char buf[MAX_BUF];

  fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
  if(fd < 0) {
    perror("gpioUnexport");
    return;
  }

  len = snprintf(buf, sizeof(buf), "%d", gpio);
  write(fd, buf, len);
  close(fd);
}

/**
 * Sets direction of pin
 */
void gpioSetDir(byte gpio, byte dir) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/direction", gpio);

  fd = open(buf, O_WRONLY);
  if(fd < 0) {
    perror("gpioSetDir");
    return;
  }

  if(dir == INPUT) {
    write(fd, "in", 3);
  } else if(dir == OUTPUT) {
    write(fd, "out", 4);
  } else {
    debug("Direction can be either INPUT or OUTPUT");
  }

  close(fd);
}

/**
 * Returns direction of pin INPUT or OUTPUT
 */
byte gpioGetDir(byte gpio) {
  int fd;
  char buf[MAX_BUF];
  char ch;

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/direction", gpio);

  fd = open(buf, O_RDONLY);
  if(fd < 0) {
    perror("gpioGetDir");
    return -1;
  }

  memset(buf, 0, MAX_BUF);
  read(fd, buf, MAX_BUF);
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
 * Sets value of pin
 */
void gpioSetValue(byte gpio, byte value) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

  fd = open(buf, O_WRONLY);
  if(fd < 0) {
    perror("gpioSetValue");
    return;
  }

  if(value == LOW) {
    write(fd, "0", 2);
  } else if(value == HIGH) {
    write(fd, "1", 2);
  } else {
    debug("Value can be either LOW or HIGH");
  }

  close(fd);
}

/**
 * Returns value of pin LOW or HIGH
 */
byte gpioGetValue(byte gpio) {
  int fd;
  char buf[MAX_BUF];
  char ch;

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

  fd = open(buf, O_RDONLY);
  if(fd < 0) {
    perror("gpioGetValue");
    return;
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
 * Set active_low of pin
 */
void gpioSetActiveLow(byte gpio, byte value) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/active_low", gpio);

  fd = open(buf, O_WRONLY);
  if(fd < 0) {
    perror("gpioSetActiveLow");
    return;
  }

  if(value == 0) {
    write(fd, "0", 2);
  } else if(value == 1) {
    write(fd, "1", 2);
  } else {
    debug("Value can be either 0 or 1");
  }

  close(fd);
}

/**
 * Returns active_low value of pin
 */
byte gpioGetActiveLow(byte gpio) {
  int fd;
  char buf[MAX_BUF];
  char ch;

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/active_low", gpio);

  fd = open(buf, O_RDONLY);
  if(fd < 0) {
    perror("gpioGetActiveLow");
    return;
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
 * Sets edge of pin
 */
void gpioSetEdge(byte gpio, byte edge) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

  fd = open(buf, O_WRONLY);
  if (fd < 0) {
    perror("gpioSetEdge");
    return;
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
  }

  close(fd);
}

/**
 * Returns edge of pin
 */
byte gpioGetEdge(byte gpio) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

  fd = open(buf, O_RDONLY);
  if (fd < 0) {
    perror("gpioGetEdge");
    return;
  }

  memset(buf, 0, MAX_BUF);
  read(fd, buf, MAX_BUF);
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
 * Sets user led trigger
 */
void ledSetTrigger(byte gpio, byte trigger) {
  if(!isLed(gpio)) {
    debug("GPIO %d does not refer an USER LED", gpio);
    return;
  }

  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_LEDS_DIR "/beaglebone:green:usr%d/trigger", gpio - 53);

  fd = open(buf, O_WRONLY);
  if(fd < 0) {
    perror("ledSetTrigger");
    return;
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
      write(fd, "hearbeat", 9);
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
      debug("Unknown trigger");
  }

  close(fd);
}

/**
 * Sets led value LOW or HIGH
 */
void ledSetValue(byte gpio, byte value) {
  if(!isLed(gpio)) {
    debug("GPIO %d does not refer an USER LED", gpio);
    return;
  }

  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_LEDS_DIR "/beaglebone:green:usr%d/brightness", gpio - 53);

  fd = open(buf, O_WRONLY);
  if(fd < 0) {
    perror("setLedValue");
    return;
  }

  if(value == LOW) {
    write(fd, "0", 2);
  } else if(value == HIGH) {
    write(fd, "1", 2);
  } else {
    debug("Value can be either INPUT or OUTPUT");
  }

  close(fd);
}

/**
 * Returns value of gpio-led LOW or HIGH
 */
byte ledGetValue(byte gpio) {
  if(!isLed(gpio)) {
    debug("GPIO %d does not refer an USER LED", gpio);
    return;
  }

  int fd;
  char buf[MAX_BUF];
  char ch;

  snprintf(buf, sizeof(buf), SYSFS_LEDS_DIR "/beaglebone:green:usr%d/brightness", gpio - 53);

  fd = open(buf, O_RDONLY);
  if(fd < 0) {
    perror("getLedValue");
    return;
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
void ledReset(byte gpio) {
  if(!isLed(gpio)) {
    debug("GPIO %d does not refer an USER LED", gpio);
    return;
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
  }
}



/**************************************************************************************************
 * 6.PWM
 *************************************************************************************************/

/**
 * PWM Initialization
 */
result_t pwmInit() {
  if(!pwmInitialized && loadDeviceTree("am33xx_pwm")) {
    buildPath("/sys/devices", "ocp", pathOcp, sizeof(pathOcp));
    pwmInitialized = 1;
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
 * Enables PWM for pin key
 */
result_t pwmEnable(const char *key) {
  if(pwmGetPin(key) != NULL) {
    // PWM pin already enabled
    return SUCCESS;
  }

  char slotsFragment   [16];
  char pwmTestFragment [16];
  char pathPwmTest     [64];
  pwmNode_t *aux;
  pwmNode_t *newNode;

  if(!pwmInitialized) {
    pwmInit();
  }

  snprintf(slotsFragment, sizeof(slotsFragment), "bone_pwm_%s", key);
  if (loadDeviceTree(slotsFragment) == ERROR) {
    debug("Could not load device tree %s", slotsFragment);
    return ERROR;
  }

  snprintf(pwmTestFragment, sizeof(pwmTestFragment), "pwm_test_%s", key);
  if(buildPath(pathOcp, pwmTestFragment, pathPwmTest, sizeof(pathPwmTest)) == ERROR) {
    debug("Could not build path to %s", pwmTestFragment);
    return ERROR;
  }

  newNode = (pwmNode_t*) malloc(sizeof(pwmNode_t));
  newNode->key = key;
  newNode->pathPwmTest = strdup(pathPwmTest);
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
 * TODO
 */
result_t pwmDisable(const char *key) {
  // TODO
  return ERROR;
}

/**
 * Sets PWM Period in nanoseconds
 */
result_t pwmSetPeriod(const char *key, ulong period) {
  pwmNode_t *aux;
  char buff[20];
  char pathPeriod[64];
  int fdPeriod;
  int len;

  if((aux = pwmGetPin(key)) == NULL) {
    debug("PWM %s not enabled yet", key);
    return ERROR;
  }

  snprintf(pathPeriod, sizeof(pathPeriod), "%s/period", aux->pathPwmTest);
  if((fdPeriod = open(pathPeriod, O_RDWR)) < 0) {
    debug("Coul dnot open file %s", pathPeriod);
    return ERROR;
  }

  len = snprintf(buff, sizeof(buff), "%lu", period);
  write(fdPeriod, buff, len);

  close(fdPeriod);
  return SUCCESS;
}

/**
 * TODO
 */
result_t pwmGetPeriod(const char *key) {
  // TODO
  return ERROR;
}

/**
 * Sets PWM pin duty in nanoseconds
 */
result_t pwmSetDuty(const char *key, ulong duty) {
  pwmNode_t *aux;
  char buff[20];
  char pathDuty[64];
  int fdDuty;
  int len;

  if((aux = pwmGetPin(key)) == NULL) {
    debug("PWM %s not enabled yet", key);
    return ERROR;
  }

  snprintf(pathDuty, sizeof(pathDuty), "%s/period", aux->pathPwmTest);
  if((fdDuty = open(pathDuty, O_RDWR)) < 0) {
    debug("Could not open file %s", pathDuty);
    return ERROR;
  }

  len = snprintf(buff, sizeof(buff), "%lu", duty);
  write(fdDuty, buff, len);

  close(fdDuty);
  return SUCCESS;
}

/**
 * TODO
 */
result_t pwmGetDuty(const char *key) {
  // TODO
  return ERROR;
}

/**
 * TODO
 */
result_t pwmSetPolarity(const char *key, byte polarity) {
  pwmNode_t *aux;
  char buff[20];
  char pathPolarity[64];
  int fdPolarity;
  int len;

  if((aux = pwmGetPin(key)) == NULL) {
    debug("PWM %s not enabled yet", key);
    return ERROR;
  }

  snprintf(pathPolarity, sizeof(pathPolarity), "%s/polarity", aux->pathPwmTest);
  if((fdPolarity = open(pathPolarity, O_RDWR)) < 0) {
    debug("Could not open file %s", pathPolarity);
    return ERROR;
  }

  len = snprintf(buff, sizeof(buff), "%d", polarity);
  write(fdPolarity, buff, len);

  close(fdPolarity);
  return SUCCESS;
}

/**
 * TODO
 */
result_t pwmGetPolarity(const char *key) {
  // TODO
  return ERROR;
}

/**
 * TODO
 */
result_t pwmSetRun(const char* key, byte run) {
  pwmNode_t *aux;
  char buff[20];
  char pathRun[64];
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

  len = snprintf(buff, sizeof(buff), "%d", run);
  write(fdRun, buff, len);

  close(fdRun);
  return SUCCESS;
}

/**
 * TODO
 */
result_t pwmGetRun(const char* key) {
  // TODO
  return ERROR;
}



#ifdef __cplusplus
}
#endif

#endif // BEAGLEBONE
