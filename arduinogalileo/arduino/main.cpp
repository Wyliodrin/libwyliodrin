// Arduino hooks
#include <Arduino.h>
#include <trace.h>
#include <interrupt.h>

/************************ Static *************************/
#define MY_TRACE_PREFIX __FILE__

/************************ Global *************************/

int arduinoSetup (int argc, char *argv[])
{
	// TODO: derive trace level and optional IP from command line
	trace_init(VARIANT_TRACE_LEVEL, 0);
	trace_target_enable(TRACE_TARGET_UART);

	// Init IRQ layer
	interrupt_init();

	// Call Arduino init
	init(argc, argv);

#if defined(USBCON)
	USBDevice.attach();
#endif
	return 0;	
}

extern "C" int wiringSetup ()
{
	return arduinoSetup (0, NULL);
}

#ifdef ARDUINO

int main(int argc, char * argv[])
{
	// TODO: disjoin platform ID defined in /sys/firmware/boad_data/plat_id with define PLATFORM_ID
	// and refuse to run if identifiers don't match

	// Install a signal handler

	// make ttyprintk at some point
	/*stdout = freopen("/tmp/log.txt", "w", stdout);
	if (stdout == NULL){
	    fprintf(stderr, "unable to remap stdout !\n");
	    exit(-1);
	}
	fflush(stdout);

	stderr = freopen("/tmp/log_er.txt", "w", stderr);
	if (stderr == NULL){
	    printf("Unable to remap stderr !\n");
	    exit(-1);
	}
	fflush(stderr);

	// debug for the user
	if (argc < 3){
		fprintf(stderr, "./sketch tty0 tty1\n");
		return -1;
	}*/
	// printf("started with binary=%s Serial=%s Serail1=%s\n", argv[0], argv[1], argv[2]);
	// fflush(stdout);

	arduinoSetup (argc, argv);
	
	setup();
	for (;;) {
		loop();
		//if (serialEventRun) serialEventRun();
	}
	return 0;
}

#endif

