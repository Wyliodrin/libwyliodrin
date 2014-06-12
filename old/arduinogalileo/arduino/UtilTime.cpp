/*
 * Time.cpp
 *
 * Provide time functions
 */

#include <unistd.h>
#include <UtilTime.h>
#include <sys/sysinfo.h>

void delay(unsigned long ms)
{
	usleep(ms * 1000);
}

void delayMicroseconds(unsigned int us)
{
	usleep(us);
}

unsigned long millis( void )
{
   return micros() / 1000;
}


unsigned long micros( void )
{

  struct timespec t;
  t.tv_sec = t.tv_nsec = 0;

  /* Considering the system does not suspend CLOCK_REALTIME is welcome.
     However, if in the future our system suspend, we need to replace
     CLOCK_REALTIME by CLOCK_BOOTTIME and apply the proper patches in 
     the kernel. */ 

  clock_gettime(CLOCK_REALTIME, &t);
  return (unsigned long)(t.tv_sec)*1000000L + t.tv_nsec / 1000L ;

}
