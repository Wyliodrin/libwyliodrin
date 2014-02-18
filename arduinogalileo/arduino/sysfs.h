#ifndef __SYSFS_H__
#define __SYSFS_H__

#ifdef __cplusplus
extern "C" {
#endif

int sysfsPwmExport(unsigned pwm, int *handle_enable, int *handle_duty);
int sysfsPwmEnable(int handle_enable, int handle_duty, unsigned int ulValue);
int sysfsPwmDisable(int handle_enable);

int sysfsGpioSet(int ihandle, unsigned int value);
int sysfsGpioGet(int ihandle);
int sysfsGpioExport(unsigned int gpio, char *path, unsigned int max_path);
int sysfsGpioDirection(unsigned int gpio, int output, int outval);
int sysfsGpioEdgeConfig(unsigned int gpio, int mode);
int sysfsGpioLevelConfig(unsigned int gpio, int mode);
int sysfsGpioSetDrive(unsigned int gpio, unsigned int mode);

#ifdef __cplusplus
}
#endif


#endif /* __SYSFS_H__ */
