#!/usr/bin/env python

import serial
import RPi.GPIO as GPIO
import io
import time
import picamera
import cv2
import numpy as np
import smbus

# for RPI version 1, use "bus = smbus.SMBus(0)"
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
address = 0x04

# functions

def writeNumber(value):
	
    bus.write_byte(address, value)
    # bus.write_byte_data(address, 0, value)
    return -1


def inputs():

    GPIO.setmode(GPIO.BCM)
    GPIO.setup(23,GPIO.IN)
    GPIO.setup(24,GPIO.IN)

def LCDclear():
        port = serial.Serial('/dev/ttyAMA0',9600,serial.EIGHTBITS,serial.PARITY_NONE, 
	serial.STOPBITS_ONE,timeout=5,rtscts = False)
	clearscreen = [chr(254), chr(88)] 
	for i in clearscreen: 
		port.write(i)
	port.close()

def LCDmessage(message):
	port = serial.Serial('/dev/ttyAMA0',9600,serial.EIGHTBITS,serial.PARITY_NONE, 
	serial.STOPBITS_ONE,timeout=5,rtscts = False)
	clearscreen = [chr(254), chr(88)] 
	for i in clearscreen: 
		port.write(i)
	 
	port.write(message)
	port.close()

def scan():

# Create the in-memory stream

	stream = io.BytesIO()

	with picamera.PiCamera() as camera:
		camera.start_preview()
		time.sleep(2)
		camera.meter_mode = 'average'
		camera.exposure_compensation = 2
		camera.exposure_mode = 'auto'
		camera.awb_mode = 'auto'
		camera.image_effect = 'none'
		camera.resolution = (1920, 1080)
		camera.capture(stream, format='jpeg')
	
		# Construct a numpy array from the stream
	
		data = np.fromstring(stream.getvalue(), dtype=np.uint8)
	
		# "Decode" the image from the array, preserving colour
	
		image =cv2.imdecode(data, 1)

	# a = cv2.imread("./test.jpg")

	a = image

	# create an array of True if blue channel greater than red channel, False otherwise
	b = (cv2.split(a) [2] > cv2.split(a)[0])

	# count number of pixels where red is greater than blue
	raw_naughtiness = np.sum(b)

	# choose an arbitary threshold
	if raw_naughtiness > 1000000:
		print "Naughty" 
	else:
		print "Nice"
	print "(" + str((raw_naughtiness*100) / 5000000) + " percent naughty)"
	# cv2.imwrite("./greenOnly.jpg", b)
	raw_naughtiness = raw_naughtiness * 2
	return raw_naughtiness
	
# start of program	

# clear LCD

LCDclear()

# clear lights
	
writeNumber(1)
time.sleep(1)

inputs()


print("Booting")
i = 0
while (i<3):
	time.sleep(0.5)
	LCDmessage("Booting")
	time.sleep(0.5)
        LCDclear()
	i = i + 1
LCDmessage("Ready To Scan   <----- Press Me")
print("Ready")

writeNumber(4)

while True:
        if ( GPIO.input(23) == False ):
                LCDmessage("Initialising    Scanner")
                time.sleep(2)
                writeNumber(1)
                time.sleep(2)
                # writeNumber(2)
                # time.sleep(0.35)
		writeNumber(10)
                LCDmessage("Scanning        Please wait")
                time.sleep(3)
                writeNumber(2)
                time.sleep(0.5)
                naughtness = scan()
                naughtness = (naughtness*100) / 5000000
		#writeNumber(4)		
		time.sleep(0.7)
                writeNumber(1)
                time.sleep(0.7)
                # writeNumber(2)
                LCDmessage("Please Remove    Hand")
                time.sleep(3)   
		writeNumber(14)
		LCDmessage("Calculating     Naughtiness")
		i = 0
                while (i<4):
	                time.sleep(0.5)
	                LCDmessage("Calculating     Naughtiness")
	                time.sleep(0.5)
                        LCDclear()
                        i = i + 1
                writeNumber(4)
                print naughtness
                naughtness = int(naughtness)
                naughtness = (str(naughtness) + "% Naughty")
                LCDmessage(naughtness)
                inputs()
                writeNumber(1)
                time.sleep(0.35)
                writeNumber(4)
	        time.sleep(8)
	        LCDclear()
	        writeNumber(4)
		LCDmessage("Ready to Scan    <----- Press Me")
				
        if ( GPIO.input(24) == False ):

                LCDmessage("you had to press me, didn't you")
                writeNumber(7)
		time.sleep(2)
		LCDclear()
		writeNumber(4)
		LCDmessage("Ready to Scan    <----- Press Me")
		inputs()
                 
		
        time.sleep(0.1);
