#include <Arduino.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <trace.h>
#include <interrupt.h>
//#include "wiring_private.h"

#define MY_TRACE_PREFIX "sysfs"

#define SYSFS_GPIO_ROOT		"/sys/class/gpio/"
#define SYSFS_PWM_ROOT		"/sys/class/pwm/pwmchip0/"
#define SYSFS_BUF		0x50

/*
 * Define period such that the PWM frequency is as close as possible to Arduino
 * Uno's one (~490Hz).
 * The following value gives a frequency of 489Hz (scope capture).  Do not
 * touch unless you know what you're doing.
 */
#define SYSFS_PWM_PERIOD_NS	1900000

#define WIRE_PWM_DUTY_MAX	255

/*
 * Initialise PWMs.
 *  - export PWMs to sysfs
 *  - set default frequency
 *  - save persistent handles to enable/duty_cycle
 *
 * This is way more coarse-grained than the GPIO version: Arduino code doesn't
 * require too much flexibility.
 */
int sysfsPwmExport(unsigned pwm, int *handle_enable, int *handle_duty)
{
	FILE *fp = NULL;
	int ret = 0;
	char export_value[16] = "";
	char fs_path[SYSFS_BUF] = SYSFS_PWM_ROOT "export";

	trace_debug("%s: pwm=%u", __func__, pwm);

	if (pwm > 99) {
		trace_error("err pwm > 99");
		return -1;
	}

	if (NULL == (fp = fopen(fs_path, "ab"))) {
		trace_error("can't open handle to %s", fs_path);
		return -1;
	}
	rewind(fp);

	memset(export_value, 0x0, sizeof(export_value));
	snprintf(export_value, sizeof(export_value), "%u", pwm);
	fwrite(&export_value, sizeof(char), sizeof(export_value), fp);
	fclose(fp);

	/* Set default frequency */
	memset(fs_path, 0x00, sizeof(fs_path));
	snprintf(fs_path, sizeof(fs_path), SYSFS_PWM_ROOT "pwm%u/period", pwm);
	if (NULL == (fp = fopen(fs_path, "ab"))) {
		trace_error("can't open handle to %s", fs_path);
		return -1;
	}
	rewind(fp);

	memset(export_value, 0x0, sizeof(export_value));
	snprintf(export_value, sizeof(export_value), "%u", SYSFS_PWM_PERIOD_NS);
	fwrite(&export_value, sizeof(char), sizeof(export_value), fp);
	fclose(fp);

	/* Open persistent handle to pwm/enable */
	memset(fs_path, 0x00, sizeof(fs_path));
	snprintf(fs_path, sizeof(fs_path), SYSFS_PWM_ROOT "pwm%u/enable", pwm);
	ret = open(fs_path,  O_RDWR);
	if (ret < 0) {
		trace_error("can't open handle to %s", fs_path);
		return ret;
	}
	*handle_enable = ret;

	/* Open persistent handle to pwm/duty_cycle */
	memset(fs_path, 0x00, sizeof(fs_path));
	snprintf(fs_path, sizeof(fs_path), SYSFS_PWM_ROOT "pwm%u/duty_cycle",
		 pwm);
	ret = open(fs_path,  O_RDWR);
	if (ret < 0) {
		trace_error("can't open handle to %s", fs_path);
		return ret;
	}
	*handle_duty = ret;

	trace_debug("%s: pwm=%u, handle_enable=%p, handle_duty=%p",
		    __func__, pwm, *handle_enable, *handle_duty);
	return 0;
}

int sysfsPwmEnable(int handle_enable, int handle_duty, unsigned int ulValue)
{
	char value[16] = "";
	char enable = '1';
	int ret = 0;
	unsigned int value_duty = 0;

	if (ulValue > WIRE_PWM_DUTY_MAX)
		return -1;

	value_duty = (ulValue * SYSFS_PWM_PERIOD_NS) / WIRE_PWM_DUTY_MAX;

	trace_debug("%s: handle_enable=%p, handle_duty=%p, ulValue=%u, "
		    "value_duty=%u", __func__, handle_enable, handle_duty,
		    ulValue, value_duty);

	memset(value, 0x0, sizeof(value));
	snprintf(value, sizeof(value), "%u", value_duty);
	lseek(handle_duty, 0, SEEK_SET);
	ret = write(handle_duty, &value, sizeof(value));
	if (sizeof(value) != ret) {
		trace_error("can't write to duty_cycle (ret %d)", ret);
		return -1;
	}

	lseek(handle_enable, 0, SEEK_SET);
	ret = write(handle_enable, &enable, sizeof(enable));
	if (sizeof(enable) != ret) {
		trace_error("can't write to enable (ret %d)", ret);
		return -1;
	}

	return 0;
}

int sysfsPwmDisable(int handle_enable)
{
	char enable = '0';
	int ret = 0;

	trace_debug("%s: handle_enable=%p", __func__, handle_enable);

	lseek(handle_enable, 0, SEEK_SET);
	ret = write(handle_enable, &enable, sizeof(enable));
	if (sizeof(enable) != ret) {
		trace_error("can't write to enable (ret %d)", ret);
		return -1;
	}

	return 0;
}

int sysfsGpioSet(int handle, unsigned int value)
{
	char set_value = 0;

	set_value = '0' + (value ? 1 : 0);
	lseek(handle, 0, SEEK_SET);

	/* Return 0 if success */
	return write(handle, &set_value, 1) != 1;
}

int sysfsGpioGet(int handle)
{
	char get_value = 0;

	lseek(handle, 0, SEEK_SET);
	read(handle, &get_value, 1);
	return (get_value == '1');
}

/* echo 'gpio' > /sys/class/gpio/export */
int sysfsGpioExport(unsigned int gpio, char *path, unsigned int max_path)
{
	FILE *fp = NULL;
	int ret = 0;
	char export_value[3] = "";
	char fs_path[SYSFS_BUF] = SYSFS_GPIO_ROOT "export";

	trace_debug("%s: gpio%u", __func__, gpio);

	if (path == NULL || max_path < sizeof(fs_path)){
		trace_error("gpio %d max_path %d -EINVAL", gpio, max_path);
		return -1;
	}

	if (gpio > 99) {
		trace_error("err gpio > 99");
		return -1;
	}

	if (NULL == (fp = fopen(fs_path, "ab"))) { //XXX why not rb+?
		trace_error("err export fs_path=%s", fs_path);
		return -1;
	}
	rewind(fp);

	snprintf(export_value, sizeof(export_value), "%u", gpio);
	fwrite(&export_value, sizeof(char), sizeof(export_value), fp);
	fclose(fp);

	/* Open persistent handle to GPIO */
	memset(path, 0x00, max_path);
	snprintf(path, max_path, SYSFS_GPIO_ROOT "gpio%u/value", gpio);
	ret = open(path, O_RDWR);

	return ret;
}

/* echo 'output' > /sys/class/gpio/gpio'gpio'/direction */
int sysfsGpioDirection(unsigned int gpio, int output, int outval)
{
	FILE *fp = NULL;
	int ret = 0;
	int handle = PIN_EINVAL;
	char dir_value[5] = "";
	char fs_path[SYSFS_BUF] = "";

	trace_debug("%s: gpio%u, output=%d, outval=%d", __func__, gpio, output, outval);

	/* Set GPIO direction  */
	snprintf(fs_path, sizeof(fs_path), SYSFS_GPIO_ROOT "gpio%u/direction", gpio);
	if (NULL == (fp = fopen(fs_path, "rb+"))) {
		trace_error("err direction fs_path=%s\n", fs_path);
		return -1;
	}
	rewind(fp);
	if (output) {
		strcpy(dir_value, outval ? "high" : "low");
	} else {
		strcpy(dir_value, "in");
	}
	fwrite(&dir_value, sizeof(char), sizeof(dir_value), fp);
	fclose(fp);

	/*
	 * Workaround for RTC #55197: some GPIO IPs/drivers won't apply a value
	 * before switching direction to output.
	 */
	if (output) {
		handle = gpio2gpiohandle(gpio);
		if (PIN_EINVAL == handle) {
			ret = handle;
			goto end;
		}
		ret = sysfsGpioSet(handle, outval);
	}

end:
	return ret;
}

int sysfsGpioSetDrive(unsigned int gpio, unsigned int mode)
{
	FILE *fp = NULL;
	int ret = 0;
	char value[8] = "";
	char fs_path[SYSFS_BUF] = "";

	trace_debug("%s: gpio%u, mode=%u", __func__, gpio, mode);

	/* Set GPIO direction  */
	snprintf(fs_path, sizeof(fs_path), SYSFS_GPIO_ROOT "gpio%u/drive", gpio);
	if (NULL == (fp = fopen(fs_path, "rb+"))) {
		trace_error("err set drive fs_path=%s\n", fs_path);
		return -1;
	}
	rewind(fp);

	switch(mode) {
	case GPIO_DRIVE_PULLUP:
		strcpy(value, "pullup");
		break;
	case GPIO_DRIVE_STRONG:
		strcpy(value, "strong");
		break;
	case GPIO_DRIVE_HIZ:
		strcpy(value, "hiz");
		break;
	default:
		trace_error("%s: unknown mode %u", __func__, mode);
		return -1;
		break;
	}

	fwrite(&value, sizeof(char), sizeof(value), fp);
	fclose(fp);

	return ret;
}

/* echo 'output' > /sys/class/gpio/gpio'gpio'/edge */
// mode - one of the modes defined for arudino IRQs CHANGE "both", RISING "rising", FALLING "falling", NONE "none"
int sysfsGpioEdgeConfig(unsigned int gpio, int mode)
{
	FILE *fp = NULL;
	int ret = 0, idx;
	char * edge_state [] = {
		"both",
		"rising",
		"falling",
		"none",
	};
	char fs_path[SYSFS_BUF] = "";

	switch(mode){
		case CHANGE:
			idx = 0;
			break;
		case RISING:
			idx = 1;
			break;
		case FALLING:
			idx = 2;
			break;
		case NONE:
			idx = 3;
			break;
		default:
			return -1;
	}

	trace_debug("%s: gpio%u, edge=%s", __func__, gpio, edge_state[idx]);

	/* Set GPIO direction  */
	snprintf(fs_path, sizeof(fs_path), SYSFS_GPIO_ROOT "gpio%u/edge", gpio);
	if (NULL == (fp = fopen(fs_path, "rb+"))) {
		trace_error("err edge fs_path=%s gpio not capable of edge config\n", fs_path);
		return -1;
	}
	rewind(fp);

	fwrite(edge_state[idx], sizeof(char), strlen(edge_state[idx]), fp);
	fclose(fp);

	return ret;
}

/* echo 'output' > /sys/class/gpio/gpio'gpio'/edge */
// mode - one of the modes defined for arudino IRQs HIGH "high", LOW "low"
int sysfsGpioLevelConfig(unsigned int gpio, int mode)
{
	FILE *fp = NULL;
	int ret = 0, idx;
	char * level_state [] = {
		"high",
		"low",
	};
	char fs_path[SYSFS_BUF] = "";

	switch(mode){
		case HIGH:
			idx = 0;
			break;
		case LOW:
			idx = 1;
			break;
		default:
			return -1;
	}

	trace_debug("%s: gpio%u, level_state=%s", __func__, gpio, level_state[idx]);

	/* Set GPIO direction  */
	snprintf(fs_path, sizeof(fs_path), SYSFS_GPIO_ROOT "gpio%u/level", gpio);
	if (NULL == (fp = fopen(fs_path, "rb+"))) {
		trace_error("err level fs_path=%s gpio not capable of level_state config\n", fs_path);
		return -1;
	}
	rewind(fp);

	fwrite(level_state[idx], sizeof(char), strlen(level_state[idx]), fp);
	fclose(fp);

	return ret;
}
