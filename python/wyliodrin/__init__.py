import os

if os.environ['GADGET'] == 'RASPBERYPI':
    from wiringpi2 import *
    wiringPiSetup ()

elif os.environg['GADGET'] == 'ARDUINOGALILEO':
    from arduino import *
    wiringSetup ()
