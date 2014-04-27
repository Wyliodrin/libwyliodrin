#include "Timer.h"

Timer::Timer(unsigned int Delay, void (*pToFunction)(Timer* timer))
        :itsDelay(Delay),itspToFunction(pToFunction),isActive(true)
{
    gettimeofday(&itsStart,0);
}

Timer::~Timer()
{
    itspToFunction=0;
}

bool Timer::Probe()
{
    timeval CurrentTime;
    gettimeofday(&CurrentTime,0);
    if(isActive==false || itsDelay > ((CurrentTime.tv_sec-itsStart.tv_sec)*1000 + (CurrentTime.tv_usec-itsStart.tv_usec)/1000))
    {
        return false;
    }
    else
    {
        itspToFunction(this);
        gettimeofday(&itsStart,0);
        return true;
    }
}

bool Timer::Probe(timeval& SomeTime)
{
    if(isActive==false || itsDelay > ((SomeTime.tv_sec-itsStart.tv_sec)*1000 + SomeTime.tv_usec-itsStart.tv_usec/1000))
    {
        return false;
    }
    else
    {
        itspToFunction(this);
        gettimeofday(&itsStart,0);
        return true;
    }
}
