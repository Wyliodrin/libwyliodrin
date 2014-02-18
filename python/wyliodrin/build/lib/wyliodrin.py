import os

try:
    from wiringpi2 import *
    wiringPiSetup ()
except:
    pass

try:
    from arduino import *
    wiringSetup ()
except:
    pass
