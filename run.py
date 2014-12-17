import RPi.GPIO as GPIO
import time
import os

#adjust for where your switch is connected
buttonPin =23
GPIO.setmode(GPIO.BCM)
GPIO.setup(buttonPin,GPIO.IN)

while True:
  #assuming the script to call is long enough we can ignore bouncing
  if (not(GPIO.input(buttonPin))):
    #this is the script that will be called (as root)
    os.system("python /home/pi/NN-0-04-4.1.py")
