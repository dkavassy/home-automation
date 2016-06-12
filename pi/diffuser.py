#!/usr/bin/python
import serial, io, time, sys

humidifier = None

def connect_humidifier():
  global humidifier
  print 'Connecting humidifier... ',
  try:
    humidifier = serial.Serial('/dev/rfcomm1', 9600)
    print 'OK'
  except serial.SerialException:
    time.sleep(10)
    connect_humidifier()

def on():
  humidifier.write('1')

def off():
  humidifier.write('0')

connect_humidifier()
humidifier.write(' ')

print "Initialised"

while True:
  try:
    on()
    time.sleep(10*60)
    off()
    time.sleep(30*60)
  except serial.SerialException:
    connect_humidifier()
    time.sleep(10*60)
    continue
