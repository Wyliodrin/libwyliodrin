import os
try:
    from libwyliodrin import *
except:
    pass

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
